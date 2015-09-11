<?php

/**
 * @file
 * MicaStudyResource class
 */

$path_module_protobuf = drupal_get_path('module', 'obiba_protobuf');
$path_module_protos = drupal_get_path('module', 'obiba_mica_model');

include_once($path_module_protobuf . '/protobuf/Protobuf.php');
use \DrSlump\Protobuf;

Protobuf::autoload();

include_once($path_module_protos . '/protos/Mica.php');
include_once($path_module_protos . '/protos/MicaSearch.php');

/**
 * Class MicaStudyResource
 */
class MicaDatasetResource extends MicaClient {

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Json());
  }

  /**
   * Get the list of datasets.
   *
   * @param string $resource "datasets" or "harmonization-datasets" or "study-datasets"
   * @param null $study_id
   * @param null $query
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array
   */
  public function get_datasets($resource = 'datasets', $study_id = NULL, $query = NULL, $from = 0, $limit = 50, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=50' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . $query;
    $params .= empty($study_id) ? '' : '&study=' . rawurlencode($study_id);
    $url_datasets = $this->micaUrl . (empty($resource) ? '' : '/' . $resource) . '/_search?' . $params;
    $request = new HttpClientRequest($url_datasets, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        )
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return new DatasetJoinResponseWrapper(json_decode($data));
    }
    catch (HttpClientException $e) {
      watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Get a dataset by its ID.
   *
   * @param string $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @return array|Protobuf\Message|null
   */
  public function get_dataset($resource = 'dataset', $dataset_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id);

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
      $dtoObj = '\obiba\mica\DatasetDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the list of variables of the dataset.
   *
   * @param $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @param null $query
   * @return array|Protobuf\Message|mixed|null
   */
  public function get_variables($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL, $query = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=-1' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . urlencode($query);

    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id) . '/variables' . (empty($query) ? '' : '/_search') . '?' . $params;

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
      if (!empty($detail)) {
        return json_decode($data);
      }
      $dtoObj = '\obiba\mica\DatasetVariablesDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  public function cross_variables($resource, $dataset_id, $variable1, $variable2) {
    $this->setLastResponse(NULL);
    $params = empty($variable2) ? '' : 'by=' . $variable2;
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id) . '/variable/' . (empty($variable1) ? '' : $variable1) . '/contingency?' . $params;

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
      if (!empty($detail)) {
        return json_decode($data);
      }
      $dtoObj = ($resource == 'study-dataset') ? '\obiba\mica\DatasetVariableContingencyDto' : '\obiba\mica\DatasetVariableContingenciesDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }


  public function download_cross_variables($resource, $dataset_id, $variable1, $variable2, $doc_type) {
    $this->setLastResponse(NULL);
    $params = empty($variable2) ? '' : 'by=' . $variable2;
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id)
      . '/variable/' . (empty($variable1) ? '' : $variable1) . '/contingency/_export?' . $params;

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(
        $doc_type === 'csv'
          ? array('Accept' => array(parent::HEADER_CSV))
          : array('Accept' => array(parent::HEADER_EXCEL_SHEET))
      ),
    ));
    $client = $this->client();
    try {
      $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return $client->lastResponse;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the list variable harmonization status for a harmonization dataset.
   *
   * @param $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array|Protobuf\Message|null
   */
  public function get_variables_harmonizations($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL, $query = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=-1' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . $query;
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id) . '/variables/harmonizations?' . $params;
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
      $dtoObj = '\obiba\mica\DatasetVariablesHarmonizationsDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  public function download_variables_harmonizations($dataset_id, $locale) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/harmonization-dataset/' . rawurlencode($dataset_id) . '/variables/harmonizations/_export';
    if (!empty($locale)) {
      $url .= '?locale=' . $locale;
    }

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_CSV),
          )
        ),
    ));
    $client = $this->client();
    try {
      $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return $client->lastResponse;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the variable by its ID.
   *
   * @param $variable_id
   * @return array|Protobuf\Message|null
   */
  public function get_variable($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . rawurlencode($variable_id);
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
      $dtoObj = '\obiba\mica\DatasetVariableDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the facet (=summary from ES) of the variable.
   *
   * @param $variable_id
   * @return array|mixed
   */
  public function get_variable_facet($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . rawurlencode($variable_id) . '/facet';
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
      return json_decode($data);
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the aggregation (=summary from ES) of the variable.
   *
   * @param $variable_id
   * @return array|mixed
   */
  public function get_variable_aggregation($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . rawurlencode($variable_id) . '/aggregation';
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
      return json_decode($data);
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the harmonizations of a dataschema variable.
   *
   * @param $variable_id
   * @return array|Protobuf\Message|null
   */
  public function get_variable_harmonizations($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . rawurlencode($variable_id) . '/harmonizations';
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
      $dtoObj = '\obiba\mica\DatasetVariableHarmonizationDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

}