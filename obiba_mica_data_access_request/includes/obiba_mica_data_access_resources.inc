<?php

/**
 * @file
 * Data access request resources class
 */


/**
 * Class MicaDataAccessRequestResources
 */
class MicaDataAccessRequestResources extends MicaClient {

  protected $method;

  /**
   * Instance initialisation.
   *
   * @param string $mica_url
   *   The mica server url.
   * @param string $method
   *   THe method to query the server.
   */
  public function __construct($mica_url = NULL, $method = 'METHOD_POST') {
    parent::__construct($mica_url);
    $this->method = $method;
  }

  /**
   * Save a  data access request.
   *
   * @param string $data
   *   The data access request json data.
   * @param string $resource
   *   The server resource.
   * @param string $id_data_access
   *   The data access request id.
   *
   * @return array
   *   The data server response.
   */
  public function dataAccessSave($data, $resource, $id_data_access = NULL) {
    $constant_method = constant('HttpClientRequest::' . $this->method);
    $url_data_access = $this->micaUrl . $resource . ($id_data_access ? '/' . $id_data_access : '');
    $request = new HttpClientRequest($url_data_access, array(
      'method' => $constant_method,
      'headers' => $this->authorizationHeader(array(
          'Accept' => array(parent::HEADER_JSON),
          'Content-Type' => array(parent::HEADER_JSON),

        )
      ),
      'data' => $data,
    ));

    $this->execute($request);
    $headers = $this->getLastResponseHeaders();
    if (!empty($headers) && !empty($headers['Location'])) {
      return array('location' => $headers['Location'][0]);
    }

    return FALSE;
  }

  /**
   * A common method to execute client request.
   *
   * @param object $request
   *   The http client request..
   *
   * @return array
   *   The data server response.
   */
  protected function execute($request) {
    $client = $this->client();
    try {
      $response = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return json_decode($response);
    }
    catch (HttpClientException $e) {
      watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      /*  drupal_set_message(t('An error occurred and processing did not
      complete Mica Server error: @message',
      array('@message' => $e->getMessage())),
      'error');*/
      drupal_add_http_header('Status', $e->getCode());
      return empty($client->lastResponse->body) ? FALSE : $client->lastResponse->body;
    }

  }

}
