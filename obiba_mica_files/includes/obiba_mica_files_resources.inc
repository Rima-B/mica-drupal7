<?php

/**
 * @file
 * MicaFilesResource class
 */

/**
 * Class MicaStudyResource
 */
class MicaFilesResource extends MicaClient {

  const POPULATION = 'population';
  const DCE = 'data-collection-event';

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);

  }

  /**
   * Get attachments by path resource.
   *
   * @param $entity_type
   *  study/network/population/data-collection-event/dataset
   * @param  $entity_id
   *  The entity Id
   *
   * @return array
   */
  public function getAttachment($entity_type, $entity_id) {
    $this->setLastResponse(NULL);
    $path_attachments = '/' . $entity_type . '/' . $entity_id;
    $url = $this->micaUrl . '/files-search' . $path_attachments .
      '?order=asc&query=&recursively=true&sort=name';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
          'Accept' => array(parent::HEADER_JSON),
        )
      ),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return $this->wrapAttachments($entity_type, $entity_id, json_decode($data));
    }
    catch (HttpClientException $e) {
      watchdog('Mica Client', 'Parsing attachments, Connection to server fail,
      Error serve code : @code, message: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return $this->wrapAttachments($entity_type, $entity_id,array());
    }
  }

  /**
   * Wrap the attachments and return a tree folders.
   *
   * @param $entity_type
   *  study/network/population/data-collection-event/dataset
   * @param  $entity_id
   *  The entity Id
   * @param $flat_attachments
   *
   * @return array
   */
  protected function wrapAttachments($entity_type, $entity_id, $flat_attachments) {
    $wrapped_attachments = array();
    foreach ($flat_attachments as $key => $attachment) {
      $wrapped_attachments[$attachment->attachment->path][$key] =
        $attachment->attachment;
    }
    ksort($wrapped_attachments, SORT_NATURAL);
    $tree = new Tree($entity_type, $entity_id);
    foreach ($wrapped_attachments as $path => $data) {
      $tree->addNode($path, $data);
    }
    return $tree;
  }

  /**
   * Deal with downloadable resources from server (Images, attachments).
   *
   * @param string $entity_type
   *   Study, dce, network ...
   * @param string $entity_id
   *   The id of the entity.
   * @param string $file_id
   *   The id of the stored file on the server.
   *
   * @return array
   *   containing :
   *    data : The raw file to download
   *    filename : The real file name of the file
   *    raw_header_array : the raw of header response
   *    or in case of error
   *    code : the error code
   *    message : the error message
   */
  public function downloadAttachmentFile($file_path, $file_name) {
    $url = $this->micaUrl . "/file-dl/" . $file_path . "/" . rawurlencode($file_name);
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => self::authorizationHeader(array(
          'Accept' => array(
            'text/html',
            'application/xhtml+xml',
            'application/xml;q=0.9',
            'image/webp',
            '*/*;q=0.8'
          ),
        )
      ),
    ));

    $client = $this->client();

    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $file_name = parent::getPropertyValueFromHeaderArray(parent::parseHeaders($client->lastResponse->headers),
        'filename',
        'Content-Disposition'
      );
      $raw_data = array(
        'extension' => parent::getFileExtension($file_name),
        'data' => $data,
        'filename' => $file_name,
        'raw_header_array' => parent::parseHeaders($client->lastResponse->headers),
      );
      return $raw_data;
    }
    catch (HttpClientException $e) {
      watchdog('MicaClient', 'Connection to server fail,  Error serve code : @code, message: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return $raw_data = array(
        'code' => $e->getCode(),
        'message' => $e->getMessage(),
      );
    }

  }

}

/**
 * Node instanced represent the tree structure
 */
class TreeNode {
  public $children;
  private $path;
  private $fullPath;
  private $data;

  public function __construct($path, $fullPath) {
    $this->path = $path;
    $this->fullPath = $fullPath;
  }

  public function getPath() {
    return $this->path;
  }

  public function getFullPath() {
    return $this->fullPath;
  }

  public function getData() {
    return $this->data;
  }

  public function setData($value) {
    $this->data = $value;
  }

  public function hasChildren() {
    return !empty($this->children);
  }

  public function getChildren() {
    return $this->children;
  }

  public function addChild($child) {
    if (empty($this->children)) {
      $this->children = array();
    }
    array_push($this->children, $child);
  }

  public function findChildFullPath($full_path) {
    return $this->findChildInternal($this, $full_path, function ($node, $full_path) {
      return $node->fullPath === $full_path;
    });
  }

  public function findChild($path) {
    return $this->findChildInternal($this, $path, function ($node, $path) {
      return $node->path === $path;
    });
  }

  public function findImmediateChild($path) {
    if (!empty($this->children)) {
      foreach ($this->children as $child) {
        if ($child->path === $path) {
          return $child;
        }
      }
    }

    return NULL;
  }

  /**
   * La recherche recursive parmis les enfants
   */
  private function findChildInternal($node, $path, $compare_function) {
    if ($compare_function($node, $path)) {
      return $node;
    }
    else {
      if (!empty($node->children)) {
        // go through all children and try to find the path
        foreach ($node->children as $child) {
          $result = $this->findChildInternal($child, $path, $compare_function);
          if ($result !== NULL) {
            return $result;
          }
        }
      }
    }

    // never found the path
    return NULL;
  }

}

class Tree {
  private $root;
  private $entity_type;
  private $id_entity;

  public function __construct($entity_type, $id_entity) {
    $this->entity_type = $entity_type;
    $this->id_entity = $id_entity;
    $this->root = new TreeNode(NULL, NULL);
  }

  /**
   * Takes a raw path, breaks into parts and assigns the data to the last node
   * For example the following data:
   *
   * '/a/b/c/d' => DATA
   *
   * Will become a tree of this form:
   *
   * - a
   *   - b
   *     - c
   *       -d => DATA
   */
  public function addNode($path, $data) {
    $parts = explode('/', trim($path, '/'));
    $node = $this->root;
    $fullPath = '';
    foreach ($parts as $path) {
      $target = $node->findImmediateChild($path);
      $fullPath .= '/' . $path;
      if (empty($target)) {
        $target = new TreeNode($path, $fullPath);
        $node->addChild($target);
      }
      $node = $target;
    }
    $node->setData($data);
  }

  public function findNode($path) {
    return $this->root->findChild($path);
  }

  public function findNodeFullPath($path) {
    return $this->root->findChildFullPath($path);
  }


  public function themeFolders($root_node = NULL, $exclude_path = NULL, $id_entity) {
    $root_node = $root_node ? $root_node : $this->root;
    $themed = $this->processThemeFolders($root_node, $exclude_path, $id_entity, 0);
    return !empty($themed) ? theme('obiba_mica_files_documents_section',
      array('attachments' => $themed)) : NULL;
  }

  private function processThemeFolders($node, $exclude_path = NULL, $id_entity, $indent = 0, &$has_files = NULL) {
    $folder_theme = NULL;
    $full_path = $node->getFullPath();
    if (!empty($full_path) && (empty($exclude_path) || !strstr($full_path, $exclude_path))) {

      $data = $node->getData();
      if (!empty($data)) {
        $folder_theme .= theme('obiba_mica_files_themed_folders_list', array(
          'indent' => $indent == 2 ? $indent - 2 : $indent,
          'folder_path' => $node->getPath() !== $id_entity ? $node->getPath() : NULL,
          'list_files' => $this->themeListFiles($data, $indent == 0 ? 0 : $indent + 2)
        ));
        $has_files++;
      }
      else{
        $folder_theme .= theme('obiba_mica_files_themed_folders_list', array(
          'indent' => $indent == 2 ? $indent - 2 : $indent,
          'folder_path' => $node->getPath() !== $id_entity ? $node->getPath() : NULL,
          'list_files' => ''
        ));
      }

      if ($node->hasChildren()) {
        $children = $node->getChildren();
        foreach ($children as $child) {
          $folder_theme .= $this->processThemeFolders($child, $exclude_path, $id_entity, $indent + 2, $has_files);
        }
      }
//      return $folder_theme ;
      return !empty($has_files) ? $folder_theme : NULL;
    }

  }

  function themeListFiles($data, $indent) {
    $files = '';
    foreach ($data as $file) {
      $files .= theme('obiba_mica_files_themed_files_list', array(
        'indent' => $indent,
        'entity_type' => $this->entity_type,
        'id_entity' => $this->id_entity,
        'attachment' => $file
      ));
    }
    return $files;
  }

  /**
   * For debugging
   */
  public function dump_tree() {
    $this->dump_treeInternal($this->root);
  }

  private function dump_treeInternal($node, $indent = 0) {
    $data = $node->getData();
    print str_repeat('&nbsp;', $indent)
      . $node->getPath()
      . '&nbsp'
      . (empty($data) ? '' : $data)
      . '<br/>';

    if ($node->hasChildren()) {
      $children = $node->getChildren();
      // array of folder templates
      foreach ($children as $child) {
        $this->dump_treeInternal($child, $indent + 2);
      }
    }
  }

}
