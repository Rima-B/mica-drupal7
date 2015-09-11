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
class MicaStudyResource extends MicaClient {

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Binary);
  }

  /**
   * Get the list of studies.
   *
   * @param null $query
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array|mixed
   */
  public function get_studies($query = NULL, $from = 0, $limit = 5, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=5' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . $query;
    $url_studies = $this->micaUrl . '/studies/_search?' . $params;
    $request = new HttpClientRequest($url_studies, array(
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
      return new StudyJoinResponseWrapper(json_decode($data));
    }
    catch (HttpClientException $e) {
      watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Get a study by its ID.
   *
   * @param $study_id
   * @return array|Protobuf\Message|null
   */
  public function get_study($study_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/study/' . rawurlencode($study_id);

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_BINARY),
          )
        ),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\StudyDto';
      Protobuf::setDefaultCodec(new Protobuf\Codec\Binary);
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

  public function studies_search($spec_dto_params = NULL) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/studies/_search/?' . 'size=' . MicaClient::getResponsePageSize();
    $url_parm = explode('?', request_uri());
    if (empty($url_parm[1])) {
      $dto_param = $this->dtoFilterConstruct('studies', NULL, NULL, $spec_dto_params);
      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_GET,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
              )
            ),
        ));
      }
      else {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_POST,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
                'Content-Type' => array(parent::HEADER_JSON)
              )
            ),
          'data' => $dto_param,
        ));
      }

    }
    else {
      $dto_param = $this->dtoFilterConstruct($url_parm[1], array('child' => 'Variable'), 'study', $spec_dto_params);
      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
        $url = $this->micaUrl . '/studies/_search/?from=' . json_decode($dto_param)->from . '&size=' . MicaClient::getResponsePageSize();

        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_GET,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
              )
            ),
        ));
      }
      else {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_POST,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
                'Content-Type' => array(parent::HEADER_JSON)
              )
            ),
          'data' => $dto_param,
        ));
      }
    }

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dto_data_response = json_decode($data);
      $this->sessionFacetsBuilder($dto_data_response);

      $summaries_studies = !empty($dto_data_response->{'obiba.mica.StudyResultDto.result'}->summaries) ?
        $dto_data_response->{'obiba.mica.StudyResultDto.result'}->summaries : NULL;
      $dtoObj = '\obiba\mica\StudySummaryDto';
      $dtos = array();
      $summaries_data = !empty($summaries_studies) ? $summaries_studies : NULL;
      if ($summaries_data) {
        foreach ($summaries_studies as $study) {
          $dtos ['data'][] = Protobuf::decode($dtoObj, json_encode($study));
        }
      }
      $dtos ['total_hits'] = $dto_data_response->totalHits;
      return $dtos;
    }
    catch (HttpClientException $e) {
      return array();
    }
  }

}