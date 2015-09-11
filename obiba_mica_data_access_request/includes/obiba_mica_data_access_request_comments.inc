<?php

/**
 * @file
 * Data access request resources class
 */

$path_module_data_access = drupal_get_path('module', 'obiba_mica_data_access_request');

include_once $path_module_data_access . '/includes/obiba_mica_data_access_resources.inc';

/**
 * Class MicaDataAccessRequestComments
 */
class MicaDataAccessRequestComments extends MicaDataAccessRequestResources {

  const RESOURCE_PATH = '%s/%s/comments';
  /**
   * Instance initialisation.
   *
   * @param string $mica_url
   *   The mica server url.
   * @param string $method
   *   THe method to query the server.
   */
  public function __construct($mica_url = NULL, $method = 'METHOD_POST') {
    parent::__construct($mica_url, $method);
  }

  /**
   * Create a comment.
   *
   * @param string $comment
   *   The content text content.
   * @param string $request_id
   *   The data access request id.
   * @param string $resource
   *   The path server resource.
   *
   * @return array
   *   The data server response.
   */
  public function createComment($comment, $request_id, $resource) {
    $this->setLastResponse(NULL);
    $url_requests = $this->micaUrl . sprintf(self::RESOURCE_PATH, $resource, $request_id);
    $request = new HttpClientRequest($url_requests, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => $this->authorizationHeader(array(
          'Accept' => array(parent::HEADER_JSON),
          'Content-Type' => array(parent::HEADER_TEXT),
        )
      ),
      'data' => $comment,
    ));

    return $this->execute($request);
  }

  /**
   * Get a comment.
   *
   * @param string $request_id
   *   The data access request id.
   * @param string $resource
   *   The path server resource.
   *
   * @return array
   *   The data server response.
   */
  public function getComments($request_id, $resource) {
    $this->setLastResponse(NULL);
    $url_requests = $this->micaUrl . sprintf(self::RESOURCE_PATH, $resource, $request_id);
    $request = new HttpClientRequest($url_requests, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
          'Accept' => array(parent::HEADER_JSON),
        )
      ),
    ));

    return $this->execute($request);
  }

}
