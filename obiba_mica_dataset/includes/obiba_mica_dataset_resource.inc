<?php

/**
 * @file
 * MicaStudyResource class
 */


/**
 * Class MicaStudyResource
 */
class MicaDatasetResource extends MicaClient {

  /**
   * Instance initialisation.
   *
   * @param string $mica_url
   *   The mica server url.
   */
  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    DrSlump\Protobuf::setDefaultCodec(new DrSlump\Protobuf\Codec\Json());
  }

  /**
   * Get the list of datasets.
   *
   * @param string $resource
   *   The resource to query.
   * @param int $study_id
   *   The study id.
   * @param string $query
   *   The query in http request.
   * @param int $from
   *   The from item.
   * @param int $limit
   *   The size response server.
   * @param string $sort
   *   Field name to sort by.
   * @param string $order
   *   the sort "asc" or "desc".
   *
   * @return array
   *   Server response.
   */
  public function getDatasets($resource = 'datasets', $study_id = NULL, $query = NULL, $from = 0, $limit = 50, $sort = NULL, $order = NULL) {
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
        ),
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
   * @param string $dataset_id
   *   The dataset id.
   * @param string $resource
   *   The resource to query.
   *
   * @return array
   *   The server response.
   */
  public function getDataset($dataset_id, $resource = 'dataset') {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . $dataset_id;

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
      return $data ? DrSlump\Protobuf::decode('\obiba\mica\DatasetDto', $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the list of variables of the dataset.
   *
   * @param string $resource
   *   The resource to query.
   * @param string $dataset_id
   *   The dataset id.
   * @param int $from
   *   The from item.
   * @param int $limit
   *   The size response server.
   * @param string $sort
   *   Field name to sort by.
   * @param string $order
   *   the sort "asc" or "desc".
   * @param string $query
   *   The query in http request.
   *
   * @return array
   *   The server response.
   */
  public function getVariables($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL, $query = NULL) {
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
      return $data ? DrSlump\Protobuf::decode('\obiba\mica\DatasetVariablesDto', $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Query the cross variables.
   *
   * @param string $resource
   *   The resource to query.
   * @param string $dataset_id
   *   The dataset id.
   * @param string $variable1
   *   The variable 1 id.
   * @param string $variable2
   *   The variable 2 id.
   *
   * @return array
   *   The server response.
   */
  public function crossVariables($resource, $dataset_id, $variable1, $variable2) {
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
      $dto_object = ($resource == 'study-dataset') ? '\obiba\mica\DatasetVariableContingencyDto' : '\obiba\mica\DatasetVariableContingenciesDto';
      return $data ? DrSlump\Protobuf::decode($dto_object, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Download the cross variables.
   *
   * @param string $resource
   *   The resource to query.
   * @param string $dataset_id
   *   The dataset id.
   * @param string $variable1
   *   The variable 1 id.
   * @param string $variable2
   *   The variable 2 id.
   * @param string $doc_type
   *   The type download file.
   *
   * @return array
   *   The server response.
   */
  public function downloadCrossVariables($resource, $dataset_id, $variable1, $variable2, $doc_type) {
    $this->setLastResponse(NULL);
    $params = empty($variable2) ? '' : 'by=' . $variable2;
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . rawurlencode($dataset_id)
      . '/variable/' . (empty($variable1) ? '' : $variable1) . '/contingency/_export?' . $params;

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(
        $doc_type === 'csv' ? array('Accept' => array(parent::HEADER_CSV)) : array('Accept' => array(parent::HEADER_EXCEL_SHEET))
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
   * @param string $resource
   *   The resource to query.
   * @param string $dataset_id
   *   The dataset id.
   * @param int $from
   *   The from item.
   * @param int $limit
   *   The size response server.
   * @param string $sort
   *   Field name to sort by.
   * @param string $order
   *   the sort "asc" or "desc".
   * @param string $query
   *   The query in http request.
   *
   * @return array
   *   The server response.
   */
  public function getVariablesHarmonizations($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL, $query = NULL) {
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
      return $data ? DrSlump\Protobuf::decode('\obiba\mica\DatasetVariablesHarmonizationsDto', $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Download the variables harmonizations.
   *
   * @param string $dataset_id
   *   The dataset id.
   * @param string $locale
   *   The local of variable.
   *
   * @return array
   *   The server response.
   */
  public function downloadVariablesHarmonizations($dataset_id, $locale) {
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
   * @param string $variable_id
   *   The variable id.
   *
   * @return array
   *   The server response.
   */
  public function getVariable($variable_id) {
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
      return $data ? DrSlump\Protobuf::decode('\obiba\mica\DatasetVariableDto', $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the aggregation (=summary from ES) of the variable.
   *
   * @param string $variable_id
   *   The variable id.
   *
   * @return array
   *   The server response.
   */
  public function getVariableAggregation($variable_id) {
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
   * @param string $variable_id
   *   The variable id.
   *
   * @return array
   *   The server response.
   */
  public function getVariableHarmonizations($variable_id) {
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
      return $data ? DrSlump\Protobuf::decode('\obiba\mica\DatasetVariableHarmonizationDto', $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

}
