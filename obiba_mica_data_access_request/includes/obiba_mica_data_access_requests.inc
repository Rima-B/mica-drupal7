<?php

/**
 * @file
 * Data access request resources class
 */

$path_module_data_access = drupal_get_path('module', 'obiba_mica_data_access_request');

include_once $path_module_data_access . '/includes/obiba_mica_data_access_resources.inc';

/**
 * Class MicaDataAccessRequests
 */
class MicaDataAccessRequests extends MicaDataAccessRequestResources {

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
   * Get data access requests list.
   *
   * @return array
   *   The data server response.
   */
  public function getRequests() {
    $this->setLastResponse(NULL);
    $url_requests = $this->micaUrl . '/data-access-requests';

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
