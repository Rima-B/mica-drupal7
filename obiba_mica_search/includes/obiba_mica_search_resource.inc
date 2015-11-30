<?php

/**
 * @file
 * MicaStudyResource class
 */

/**
 * Class MicaSearchResource
 */
class MicaSearchResource extends MicaClient {

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    DrSlump\Protobuf::setDefaultCodec(new DrSlump\Protobuf\Codec\Json());
  }

  public function download_taxonomies_coverage($query_param_parts, $default_dto_search = NULL) {
    $response_data =
      $this->_aggregation_resource_internal(parent::HEADER_CSV,
        'coverage',
        NULL,
        $default_dto_search,
        $query_param_parts
      );

    if (!empty($response_data)) {
      return $response_data['last_response'];
    }
  }

  public function taxonomies_coverage($parameters = NULL, $default_dto_search = NULL, $query_param_parts = NULL) {
    $coverage_result = $this->_aggregation_resource('coverage', $parameters, $default_dto_search, $query_param_parts);

    if (!empty($coverage_result) && !empty($coverage_result->queryResult)) {
      $_SESSION['request-search-response'] = 'no-empty';
      $this->sessionFacetsBuilder($coverage_result->queryResult);
    }
    else {
      $_SESSION['request-search-response'] = 'empty';
    }

    return $coverage_result;
  }

  private function _search_resource($parameters = NULL) {
    return $this->_aggregation_resource('search', $parameters);
  }

  private function _get_dto_query_param_from_url($parts = NULL, $type, $parameters = NULL) {
    $from = empty($parts['page']) ? 0 : $this->paginationListSearchParameters($_GET['page'], $type, MicaClient::getResponsePageSize());
    $q = empty($parts['query']) ? (empty($parameters) ? NULL : $parameters) : $parts['query'];
    $group_by = empty($parts['group-by']) ? NULL : $parts['group-by'];
    $withFacets = empty($parts['with-facets']) ? TRUE : strtolower($parts['with-facets']) === "true";

    return $this->dtoFilterConstruct($type, $q, $from, $group_by, $withFacets);
  }

  /*
   * $parameters : json query hard coded
   * */
  private function  _aggregation_resource($aggregation_type = 'search', $parameters = NULL, $default_dto_search = NULL, $query_param_parts = NULL) {
    $response_data =
      $this->_aggregation_resource_internal(parent::HEADER_JSON,
        $aggregation_type,
        $parameters,
        $default_dto_search,
        $query_param_parts
      );

    if (!empty($response_data)) {
      return json_decode($response_data['data']);
    }
  }

  private function _aggregation_resource_internal($accept_type,
                                                  $aggregation_type = 'search',
                                                  $parameters = NULL,
                                                  $default_dto_search = NULL,
                                                  $query_param_parts = NULL) {
    if (!empty($_SESSION['errorsQuery'])) {
      unset($_SESSION['errorsQuery']);
    }

    $parts = $query_param_parts;
    $type = 'variables';

    if (empty($parts)) {
      $url_parm = explode('?', request_uri());

      if (!empty($url_parm[1])) {
        parse_str($url_parm[1], $parts);
        if (!empty($parts['type'])) {
          $type = $parts['type'];
        }
      }
    }

    if (!empty($default_dto_search)) {
      if (empty($parts)) {
        $parts = $default_dto_search;
      }
      else {
        $parts = array_merge($default_dto_search, $parts);
      }
    }
    if (empty($parameters) || $parameters != 'empty_request') {
      $dto_param = $this->_get_dto_query_param_from_url($parts, $type, $parameters);
    }

    $url = $this->micaUrl . '/' . $type . '/_' . $aggregation_type;

    $request_parameters = array();
    if ($aggregation_type == 'coverage') {
      // unless explicitly specified, set the strictness of the coverage as configured
      if (!empty($parts['strict'])) {
        $request_parameters['strict'] = $parts['strict'];
      } else {
        $request_parameters['strict'] = variable_get_value('variables_strict_coverage', array('default' => TRUE)) ? 'true' : 'false';
      }
    }

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array($accept_type),
            'Content-Type' => array(parent::HEADER_JSON)
          )
        ),
      'parameters' => $request_parameters,
      'data' => !empty($dto_param) ? $dto_param : $this->dtoFilterConstruct($type, NULL, 0, NULL),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return array('data' => $data, 'last_response' => $client->lastResponse);
    } catch (HttpClientException $e) {
      if ($e->getCode() == 403) {
        drupal_set_message('<i class="glyphicon glyphicon-info-sign"></i> Please set correct credentials access to mica-server', 'warning');
      }
      watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
        array('@code' => $e->getCode(), '@message' => $e->getMessage()), WATCHDOG_WARNING);
      return array();
    }

  }

  public function get_stats_entities_count($dto, $dto_data_response) { //dpm($dto_data_response);
    return array(
      'data' => $dto['data'],
      'study_total_hits' => empty($dto_data_response->studyResultDto) ? 0 : $dto_data_response->studyResultDto->totalHits,
      'study_total_count' => empty($dto_data_response->studyResultDto) ? 0 : $dto_data_response->studyResultDto->totalCount,
      'variable_total_hits' => empty($dto_data_response->variableResultDto) ? 0 : $dto_data_response->variableResultDto->totalHits,
      'variable_total_count' => empty($dto_data_response->variableResultDto) ? 0 : $dto_data_response->variableResultDto->totalCount,
      'network_total_hits' => !empty($dto_data_response->networkResultDto->totalHits) ?
          $dto_data_response->networkResultDto->totalHits : 0,
      'network_total_count' => !empty($dto_data_response->networkResultDto->totalCount) ?
          $dto_data_response->networkResultDto->totalCount : 0,
      'dataset_total_hits' => !empty($dto_data_response->datasetResultDto->totalHits) ?
          $dto_data_response->datasetResultDto->totalHits : 0,
      'dataset_total_count' => !empty($dto_data_response->datasetResultDto->totalCount) ?
          $dto_data_response->datasetResultDto->totalCount : 0
    );
  }

  public function search_variables() {
    return $this->search_documents(JoinQueryResponseWrapper::VARIABLES);
  }

  public function search_studies() {
    return $this->search_documents(JoinQueryResponseWrapper::STUDIES);
  }

  public function search_networks() {
    return $this->search_documents(JoinQueryResponseWrapper::NETWORKS);
  }

  public function search_datasets() {
    return $this->search_documents(JoinQueryResponseWrapper::DATASETS);
  }

  private function search_documents($type) {
    $join_query_response = new JoinQueryResponseWrapper($this->_search_resource());
    $response = $join_query_response->getResponseWrapper($type);

    if (!$response->isEmpty()) {
      $_SESSION['request-search-response'] = 'no-empty';
      $this->sessionFacetsBuilder($join_query_response->getResponseDto());
    }
    else {
      $_SESSION['request-search-response'] = 'empty';
    }

    return $join_query_response;
  }

}