<?php

/**
 * @file
 * MicaStudyResource class
 */

$path_module_protobuf = drupal_get_path('module', 'obiba_protobuf');
$path_module_protos = drupal_get_path('module', 'obiba_mica_model');

include_once $path_module_protobuf . '/protobuf/Protobuf.php';
use \DrSlump\Protobuf;

Protobuf::autoload();

include_once $path_module_protos . '/protos/Mica.php';
include_once $path_module_protos . '/protos/MicaSearch.php';

/**
 * Class MicaStudyResource
 */
class MicaTaxonomyResource extends MicaClient {

  /**
   * Instance initialisation.
   *
   * @param string $mica_url
   *   The Mica server url.
   */
  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Binary());
  }

  /**
   * Get taxonomy summaries.
   *
   * @return object
   *   The taxonomy summaries wrapper.
   */
  public function getTaxonomySummaries() {
    $this->setLastResponse(NULL);
    $url_studies = $this->micaUrl . '/config/taxonomies/summaries';
    $request = new HttpClientRequest($url_studies, array(
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
      watchdog('MicaTaxonomyResource', 'Connection to server fail,  Error serve code : @code, message: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array();
    }
  }

}
