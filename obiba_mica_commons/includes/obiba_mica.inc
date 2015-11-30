<?php

/**
 * @file
 * MicaClient class
 */

include_once "obiba_mica_commons_query_dto_builders.inc";
include_once "obiba_mica_commons_query_response_wrappers.inc";
include_once "obiba_mica_commons_resource_paths.inc";


/**
 * Class MicaClient
 */
class MicaClient {
  const AUTHORIZATION_HEADER = 'Authorization';
  const COOKIE_HEADER = 'Cookie';
  const SET_COOKIE_HEADER = 'Set-Cookie';
  const OBIBA_COOKIE = 'obibaid';
  const MICA_COOKIE = 'micasid';
  const HEADER_BINARY = 'application/x-protobuf';
  const HEADER_JSON = 'application/json';
  const HEADER_CSV = 'text/csv';
  const HEADER_EXCEL_SHEET = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet';
  const HEADER_TEXT = 'text/plain';
  const PAGINATE_STUDIES = 0;
  const PAGINATE_VARIABLES = 1;
  const PAGINATE_DATASETS = 2;
  const PAGINATE_NETWORKS = 3;

  protected $micaUrl;

  private $lastResponse;
  private static $responsePageSizeSmall = 10;
  private static $responsePageSize = 20;


  /**
   * Instance initialisation.
   *
   * Mica client from a given url or from the one retrieved from 'mica_url'
   * variable.
   *
   * @param string $mica_url
   *   IF given,  the Mica server url, the  default value is a a setting
   *   parameter
   */
  public function __construct($mica_url = NULL) {
    $this->micaUrl = (isset($mica_url) ? $mica_url : variable_get_value('mica_url')) . '/ws';
    self::$responsePageSize = variable_get_value('mica_response_page_size');
    self::$responsePageSizeSmall = variable_get_value('mica_response_page_size_small');
  }

  /**
   * Make sure we are not using previous session state.
   */
  public static function clear() {
    unset($_SESSION[self::MICA_COOKIE]);
  }

  /**
   * Get the last response (if any).
   *
   * @return array
   *   The response server formatted as an array.
   */
  public function getLastResponse() {
    return $this->lastResponse;
  }

  /**
   * Set the last response cookies after a http call.
   *
   * @param array $last_response
   *   The server response.
   */
  protected function setLastResponse($last_response) {
    $this->lastResponse = $last_response;
    if (isset($last_response)) {
      $this->setLastResponseCookies();
    }
  }

  /**
   * Get the last response headers (if any).
   *
   * @return array
   *   The header response server.
   */
  public function getLastResponseHeaders() {
    if ($this->lastResponse != NULL) {
      $result = array();
      foreach (explode("\r\n", $this->lastResponse->headers) as $header) {
        $h = explode(":", $header, 2);
        if (count($h) == 2) {
          if (!array_key_exists($h[0], $result)) {
            $result[$h[0]] = array();
          }
          array_push($result[$h[0]], trim($h[1]));
        }
      }
      return $result;
    }
    return '';
  }

  /**
   * Send a logout request to Mica and clean drupal client cookies.
   */
  public function logout() {
    $url = $this->micaUrl . '/auth/session/_current';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_DELETE,
      'headers' => $this->authorizationHeader(array(
        'Accept' => array(HEADER_JSON),
      )),
    ));

    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();
      unset($_SESSION[self::MICA_COOKIE]);
    }
    catch (HttpClientException $e) {
      // Clear anyway.
      watchdog('MicaClient', 'Connection to server fail,  Error serve code : @code, message: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();
      unset($_SESSION[self::MICA_COOKIE]);
    }
  }

  /**
   * Get the header value(s) from the last response.
   *
   * @param string $header_name
   *   The header information to extract.
   *
   * @return array
   *   The value of header name.
   */
  protected function getLastResponseHeader($header_name) {
    $headers = $this->getLastResponseHeaders();

    if (array_key_exists($header_name, $headers)) {
      return $headers[$header_name];
    }
    return array();
  }

  /**
   * Forwards the 'Set-Cookie' directive(s) to the drupal client.
   *
   * If the user was authenticated by Agate.
   */
  private function setLastResponseCookies() {
    foreach ($this->getLastResponseHeader(self::SET_COOKIE_HEADER) as $cookie_str) {
      $cookie = $this->parseCookie($cookie_str);
      $keys = array_keys($cookie);
      $name = $keys[0];
      $value = $cookie[$name];
      watchdog('Mica Client', 'Cookie: name=@name, value=@value',
        array('@name' => $name, '@value' => $value), WATCHDOG_DEBUG);
      if (empty($value)) {
        if (!empty($_SESSION[$name])) {
          unset($_SESSION[$name]);
        }
      }
      else {
        $_SESSION[$name] = $value;
      }
    }
  }

  /**
   * Add authorization headers.
   *
   * @param array $headers
   *   A header array.
   *
   * @return array
   *   The transformed initial headers.
   */
  protected function authorizationHeader(array $headers) {
    if (isset($_SESSION[self::OBIBA_COOKIE])) {
      // Authenticate by cookies coming from request (case of regular user
      // logged in via Agate).
      watchdog('obiba_mica', 'Auth by cookies from request');
      $headers = $this->addCookieHeader($headers, $_SESSION[self::OBIBA_COOKIE],
        isset($_SESSION[self::MICA_COOKIE]) ?
          $_SESSION[self::MICA_COOKIE] : NULL);
    }
    else {
      watchdog('obiba_mica', 'Auth by anonymous credentials');
      $current_anonymous_pass = variable_get_value('mica_anonymous_password');
      if ($current_anonymous_pass == variable_get_value('mica_anonymous_password_saved')) {

        if (isset($_SESSION[self::MICA_COOKIE])) {
          $headers = $this->addCookieHeader($headers, NULL,
            $_SESSION[self::MICA_COOKIE]);
        }

        $credentials = variable_get_value('mica_anonymous_name') . ':'
          . variable_get_value('mica_anonymous_password');
        $headers[self::AUTHORIZATION_HEADER] = array(
          'Basic '
          . base64_encode($credentials),
        );
      }
      else {
        $credentials = variable_get_value('mica_anonymous_name') . ':'
          . variable_get_value('mica_anonymous_password');
        $headers[self::AUTHORIZATION_HEADER] = array(
          'Basic '
          . base64_encode($credentials),
        );
        session_unset();
        variable_set_value('mica_anonymous_name_saved', variable_get_value('mica_anonymous_name'));

      }
    }
    return $headers;
  }

  /**
   * Add the cookie Id/value to the header.
   *
   * @param array $headers
   *   The server response header.
   * @param string $obibaid
   *   The obibaid cookie id.
   * @param string $micasid
   *   The micaid cookie id.
   *
   * @return array
   *   The transformed initial headers.
   */
  private
  function addCookieHeader(array $headers, $obibaid, $micasid) {
    $cookie = $this->cookieHeaderValue($obibaid, $micasid);

    if (isset($headers[self::COOKIE_HEADER])) {
      array_push($headers[self::COOKIE_HEADER], $cookie);
    }
    else {
      $headers[self::COOKIE_HEADER] = array($cookie);
    }
    return $headers;
  }

  /**
   * Add authorization by cookies header.
   *
   * @param string $obibaid
   *   The obibaid cookie id.
   * @param string $micasid
   *   The micaid cookie id.
   *
   * @return string
   *   The Value of the cooke.
   */
  private
  function cookieHeaderValue($obibaid, $micasid) {
    $cookie_parts = array();

    if (isset($obibaid)) {
      $cookie_parts[] = self::OBIBA_COOKIE . '=' . $obibaid;
    }

    if (isset($micasid)) {
      $cookie_parts[] = self::MICA_COOKIE . '=' . $micasid;
    }

    return implode("; ", $cookie_parts);
  }

  /**
   * Explode a cookie string in a array.
   *
   * @param string $cookie_str
   *   The cookie string.
   *
   * @return array
   *   The cookie array.
   */
  private
  function parseCookie($cookie_str) {
    $cookie = array();
    foreach (explode(';', $cookie_str) as $entry_str) {
      $entry = explode('=', $entry_str);
      $cookie[$entry[0]] = $entry[1];
    }
    return $cookie;
  }

  /**
   * The client object construction overriding the httpClient->client() method.
   *
   * @return object
   *   The overridden object.
   */
  protected
  function client() {
    $default_headers_setter = function ($request) {
      $request->setHeader('Accept-Encoding', 'gzip, deflate');
    };

    $client = new HttpClient(NULL, NULL, $default_headers_setter);

    if (!isset($client->options['curlopts'])) {
      $client->options['curlopts'] = array();
    }

    $client->options['curlopts'] += array(
      CURLOPT_SSL_VERIFYHOST => FALSE,
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_ENCODING => TRUE,
    );

    DrSlump\Protobuf::setDefaultCodec(new DrSlump\Protobuf\Codec\Json());

    return $client;
  }

  /**
   * The construction of the session facet buckets.
   *
   * @param object $facets_bucket
   *   An array of facet buckets.
   *
   * @return array
   *   An array of constructed facet bucket to store in session.
   */
  protected
  function sessionFacetsBucketConstruction($facets_bucket) {
    $facets_buckets = array();
    if (!empty($facets_bucket)) {
      foreach ($facets_bucket as $bucket) {
        $facets_buckets[$bucket->aggregation] = array(
          'title' => obiba_mica_commons_get_localized_field($bucket, 'title'),
          'key' => $bucket->aggregation,
          'aggs' => $bucket->aggregation,
          'dto' => $bucket,
        );

        if (!empty($bucket->children)) {
          $children = array();
          foreach ($bucket->children as $child) {
            array_push($children, array(
              'title' => obiba_mica_commons_get_localized_field($child, 'title'),
              'key' => $child->aggregation,
              'aggs' => $child->aggregation,
              'dto' => $child,
            ));
          }

          $facets_buckets[$bucket->aggregation]['children'] = $children;
        }
      }
      return $facets_buckets;
    }
    return NULL;
  }

  /**
   * Facets session builder.
   *
   * @param object $aggregations
   *   The object aggregation returned by Mica server.
   */
  protected
  function sessionFacetsBuilder($aggregations) {
    $network_buckets = array();
    $study_buckets = array();
    $dataset_buckets = array();
    $variable_buckets = array();
    unset($_SESSION['variables']);
    unset($_SESSION['studies']);
    unset($_SESSION['datasets']);
    unset($_SESSION['networks']);
    $network_aggregations = NULL;
    foreach ($aggregations as $type_aggregations => $aggregation) {
      if ($type_aggregations == 'networkResultDto') {
        $_SESSION['networks']['aggregations'] = !empty($aggregation->aggs) ? $aggregation->aggs : array();
        $_SESSION['networks']['aggregations']['total_hits'] = $aggregation->totalHits;
        $_SESSION['networks']['aggregations']['total_count'] = $aggregation->totalCount;
        if (!empty($aggregation->aggs)) {
          $network_aggregations = array_filter($aggregation->aggs, function ($agg) {
            return $agg->aggregation === "networkId";
          });
          $network_buckets = $this->sessionFacetsBucketConstruction($aggregation->aggs);
        }
      }

      if ($type_aggregations == 'datasetResultDto') {
        if (!empty($aggregation->aggs)) {
          $dataset_buckets = $this->sessionFacetsBucketConstruction($aggregation->aggs);
          $_SESSION['datasets']['aggregations'] = $aggregation->aggs;
        }
        $_SESSION['datasets']['aggregations']['total_hits'] = $aggregation->totalHits;
        $_SESSION['datasets']['aggregations']['total_count'] = $aggregation->totalCount;
      }

      if ($type_aggregations == 'variableResultDto') {
        if (!empty($aggregation->aggs)) {
          $filtered = self::filterVariableAggregations($aggregation->aggs);
          self::orderFacetTaxonomies($filtered, function ($value) {
            return empty($value->aggregation) ? $value : $value->aggregation;
          });
          $variable_buckets = $this->sessionFacetsBucketConstruction($filtered);
        }
        $_SESSION['variables']['studies'] = array();
        $study_result
          = isset($aggregations->studyResultDto->{'obiba.mica.StudyResultDto.result'}) ?
          $aggregations->studyResultDto->{'obiba.mica.StudyResultDto.result'} :
          NULL;

        if (!empty($study_result) &&
          (!empty($study_result->summaries) ||
            !empty($study_result->digests))
        ) {
          $_SESSION['variables']['studies']
            = empty($study_result->summaries) ?
            $study_result->digests :
            $study_result->summaries;
        }

        $dataset_result
          = isset($aggregations->datasetResultDto->{'obiba.mica.DatasetResultDto.result'}) ?
          $aggregations->datasetResultDto->{'obiba.mica.DatasetResultDto.result'} :
          NULL;

        $_SESSION['variables']['datasets'] = array();
        if (!empty($dataset_result) && (!empty($dataset_result->datasets) ||
            !empty($dataset_result->digests))
        ) {
          $_SESSION['variables']['datasets']
            = empty($dataset_result->datasets) ? $dataset_result->digests : $dataset_result->datasets;
        }

        $_SESSION['variables']['aggregations']
          = !empty($aggregation->aggs) ? $aggregation->aggs : array();
        $_SESSION['variables']['aggregations']['total_hits']
          = $aggregation->totalHits;
        $_SESSION['variables']['aggregations']['total_count']
          = $aggregation->totalCount;
      }
      if ($type_aggregations == 'studyResultDto') {
        if (!empty($aggregation->aggs)) {
          $study_buckets = $this->sessionFacetsBucketConstruction(self::filterStudyAggregations($aggregation->aggs));
        }
        if (!empty($aggregations->networkResultDto)) {
          $network_result = isset($aggregations->networkResultDto->{'obiba.mica.NetworkResultDto.result'}) ? $aggregations->networkResultDto->{'obiba.mica.NetworkResultDto.result'} : NULL;
          if (!empty($network_result) && (!empty($network_result->networks) || !empty($network_result->digests))) {
            $_SESSION['studies']['networks'] = array();
            $_SESSION['studies']['networks']
              = empty($network_result->digests) ? $network_result->networks : $network_result->digests;
          }
        }

        if (!empty($aggregations->variableResultDto->aggs)) {
          $variable_studies_id_aggregations
            = $this->getAggregationsByEntityDto($aggregations->variableResultDto->aggs, 'studyIds');
          $_SESSION['studies']['variables'] = !empty($variable_studies_id_aggregations) ?
            $variable_studies_id_aggregations : NULL;
        }

        if (!empty($aggregations->networkResultDto->aggs)) {
          $networks_studies_id_aggregations
            = $this->getAggregationsByEntityDto($aggregations->networkResultDto->aggs, 'studyIds');
          $_SESSION['studies']['networks-studyId'] = !empty($networks_studies_id_aggregations) ?
            $networks_studies_id_aggregations : NULL;
        }

        if (!empty($aggregations->datasetResultDto) && !empty($aggregations->datasetResultDto->aggs)) {
          $datasets_study_tables_study_id_aggregations
            = $this->getAggregationsByEntityDto($aggregations->datasetResultDto->aggs, 'studyTable-studyId');
          $_SESSION['studies']['studyTable-studyId'] = !empty($datasets_study_tables_study_id_aggregations) ?
            $datasets_study_tables_study_id_aggregations : NULL;
        }

        $_SESSION['studies']['aggregations'] = !empty($aggregation->aggs) ? $aggregation->aggs : array();
        $_SESSION['studies']['aggregations']['total_hits'] = $aggregation->totalHits;
        $_SESSION['studies']['aggregations']['total_count'] = $aggregation->totalCount;
      }
    }

    // Move some facets.
    $_SESSION['variables']['facets-buckets'] = array_merge($dataset_buckets, $variable_buckets);
    $moved_facets = array_filter($_SESSION['variables']['facets-buckets'], function ($facet) {
      return $facet['key'] === 'studyIds' || $facet['key'] === 'dceIds';
    });

    foreach ($moved_facets as $key => $facet) {
      unset($_SESSION['variables']['facets-buckets'][$key]);
    }

    $network_moved_facets = array_merge($network_buckets, $moved_facets);
    $_SESSION['studies']['facets-buckets'] = array_merge(
      $network_moved_facets,
      $study_buckets);

    if (!empty($network_aggregations)) {
      foreach ($network_aggregations as $network_aggregation) {
        array_push($_SESSION['studies']['aggregations'], $network_aggregation);
      }
    }
  }

  /**
   * Get aggregations by entity Dto.
   *
   * @param object $entity_aggr
   *   The entity aggregation.
   * @param string $aggregations_term
   *   The aggregation term.
   *
   * @return object
   *   The aggregation term object.
   */
  protected
  function getAggregationsByEntityDto($entity_aggr, $aggregations_term) {
    if (!empty($entity_aggr)) {
      foreach ($entity_aggr as $agg) {
        if ($agg->aggregation == $aggregations_term && !empty($agg->{'obiba.mica.TermsAggregationResultDto.terms'})) {
          return $agg->{'obiba.mica.TermsAggregationResultDto.terms'};
        }
      }
    }
    return NULL;
  }

  /**
   * GEt the setting of the samll page size response (for pagination).
   *
   * @return int
   *   The page size.
   */
  public
  static function getResponsePageSizeSmall() {
    return self::$responsePageSizeSmall;
  }

  /**
   * GEt the setting of the page size response (for pagination).
   *
   * @return int
   *   The page size.
   */
  public
  static function getResponsePageSize() {
    $size = empty($_GET['size']) ? self::$responsePageSize : $_GET['size'];
    return $size;
  }

  /**
   * Removes the flagged aggregations from the UI only.
   *
   * @param object $aggs
   *   The aggregations.
   *
   * @return array
   *   filtered aggregations.
   */
  private
  static function filterVariableAggregations($aggs) {
    $filters = NULL;
    if (variable_get_value('search_datasets_autocomplete') == ObibaDatasetConstants::FACET_TYPE_HIDE) {
      $filters[] = "datasetId";
    }
    if (!variable_get_value('search_studies_dce_autocomplete')) {
      $filters[] = "dceIds";
    }
    return empty($filters) ? $aggs : array_filter($aggs, function ($agg) use ($filters) {
      return FALSE === array_search($agg->aggregation, $filters);
    });
  }

  /**
   * Removes the flagged aggs from the UI only.
   *
   * @param object $aggs
   *   The aggregations.
   *
   * @return array
   *   filtered aggregations.
   */
  private
  static function filterStudyAggregations($aggs) {
    $filters = NULL;
    if (!variable_get_value('search_general_population_sources_autocomplete')) {
      $filters[] = "populations-recruitment-generalPopulationSources";
    }
    return empty($filters) ? $aggs : array_filter($aggs, function ($agg) use ($filters) {
      return FALSE === array_search($agg->aggregation, $filters);
    });
  }

  /**
   * GEt taxonomies count.
   *
   * @return int
   *   The number of taxonomies.
   */
  public
  static function getTaxonomiesCount() {
    return count(self::getTaxonomiesAsArray(variable_get_value('mica_taxonomy_order')));
  }

  /**
   * Orders the matching taxonomies.
   *
   * By based on the order defind in 'mica_taxonomy_order'.
   *
   * @param object $taxonomies
   *   The taxonomies object.
   * @param string $getter
   *   The getter method.
   */
  public
  static function orderTaxonomies(&$taxonomies, $getter) {
    $order_taxonomy_variable = variable_get_value('mica_taxonomy_order');
    if (!empty($taxonomies) && !empty($order_taxonomy_variable)) {
      $ordered_taxonomies = self::getTaxonomiesAsArray($order_taxonomy_variable);
      $taxonomies = array_map(
        function ($n) use ($taxonomies, $getter) {
          $tmp = array_filter($taxonomies,
            function ($t) use ($n, $getter) {
              return $getter($t) === $n;
            }
          );
          return reset($tmp);
        }
        , $ordered_taxonomies);
      $taxonomies = array_filter($taxonomies, function ($t) {
        return !empty($t);
      }
      );
    }
  }

  /**
   * Order the facet taxonomy object.
   *
   * @param object $taxonomies
   *   The taxonomies object.
   * @param string $getter
   *   The getter method.
   */
  public
  static function orderFacetTaxonomies(&$taxonomies, $getter) {
    $order_taxonomy_variable = variable_get_value('mica_taxonomy_order');

    if (!empty($taxonomies) && !empty($order_taxonomy_variable)) {
      $ordered_taxonomies = self::getTaxonomiesAsArray($order_taxonomy_variable);

      $taxonomies = array_filter($taxonomies, function ($taxonomy) use ($ordered_taxonomies, $getter) {
        return empty($taxonomy->children) || FALSE !== array_search($getter($taxonomy), $ordered_taxonomies);
      });

      $max_size = count($ordered_taxonomies);
      $indexes = array_fill(0, $max_size, NULL);
      $values = array_fill(0, $max_size, NULL);

      foreach ($taxonomies as $index => $value) {
        $position = array_search($getter($value), $ordered_taxonomies);
        if ($position !== FALSE) {
          $indexes[$position] = $index;
          $values[$position] = $value;
        }
      }

      // Compact the index and value arrays by removing padding value -9999.
      $indexes = array_filter($indexes, function ($e) {
        return $e !== NULL;
      });

      $values = array_filter($values, function ($e) {
        return $e !== NULL;
      });

      asort($indexes);

      // Place the values at the proper position.
      while (count($indexes) > 0) {
        $taxonomies[array_pop($indexes)] = array_pop($values);
      }
    }
  }

  /**
   * Get taxonomies as array.
   *
   * @param string $taxonomies
   *   A string taxonomies.
   *
   * @return array
   *   The taxonomies as an array.
   */
  static private function getTaxonomiesAsArray($taxonomies) {
    return empty($taxonomies) ? array() : explode(',', preg_replace('/\s*,\s*/', ',', trim($taxonomies)));
  }

  /**
   * Check if the entity has aggregations.
   *
   * @param object $entity
   *   The entity to check (variable, study, network, ...).
   *
   * @return bool
   *   If yes or no has aggregations.
   */
  public static function hasEntityAggregations($entity) {
    return !empty($entity->{'obiba.mica.TermsAggregationResultDto.terms'})
    || !empty($entity->{'obiba.mica.StatsAggregationResultDto.stats'});
  }

  /**
   * Check if the entity has a facet in aggregations.
   *
   * @param string $facet
   *   The facet name.
   *
   * @return bool
   *   If yes or no has aggregations.
   */
  public static function hasEntityByIdAggregation($facet) {
    if (!empty($_SESSION['studies']['aggregations'])) {
      foreach ($_SESSION['studies']['aggregations'] as $entity) {
        if (!empty($entity->aggregation) && $entity->aggregation == $facet) {
          if (!empty($entity->{'obiba.mica.TermsAggregationResultDto.terms'}) ||
            !empty($entity->{'obiba.mica.StatsAggregationResultDto.stats'})
          ) {
            return TRUE;
          }
          else {
            return FALSE;
          }
        }
      }
    }
    if (!empty($_SESSION['variables']['aggregations'])) {
      foreach ($_SESSION['variables']['aggregations'] as $entity) {
        if (!empty($entity->aggregation) && $entity->aggregation == $facet) {
          if (!empty($entity->{'obiba.mica.TermsAggregationResultDto.terms'})) {
            return TRUE;
          }
          else {
            return FALSE;
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * The search list pagination parameters.
   *
   * @param int $current_page
   *   The current page number
   * @param string $type
   *   The entity type.
   * @param int $size
   *   The size server response.
   *
   * @return float|int
   *   The next page offset.
   */
  public function paginationListSearchParameters($current_page = 0, $type = NULL, $size = NULL) {
    return self::nextPageOffset(
      empty($current_page) ? 0 : $current_page,
      empty($size) ? MicaClient::getResponsePageSize() : $size,
      @$_SESSION[strtolower($type)]['aggregations']['total_hits']
    );
  }

  /**
   * Calculate the next offset page.
   *
   * @param int $current_page
   *   The current page number.
   * @param int $size
   *   The size server response.
   * @param int $total
   *   The total in server.
   *
   * @return int
   *   The next page offset.
   */
  public static function nextPageOffset($current_page = 0, $size = NULL, $total = NULL) {
    $nb_pages = ceil($total / $size);
    $from = 0;
    if (!empty($current_page)) {
      $actual_page = intval($current_page);
      if ($actual_page > $nb_pages) {
        $actual_page = $nb_pages;
      }
      $from = ($actual_page) * $size;
    }

    return $from;
  }

  /**
   * Dto filter construction.
   *
   * @param string $type
   *   Entity type.
   * @param object $request_param
   *   The request parameters.
   * @param int $from
   *   The next page offset.
   * @param string $group_by
   *   Group by parameters.
   * @param bool $with_facets
   *   With facet or not.
   *
   * @return string
   *   A json of constructed filter request.
   */
  protected function dtoFilterConstruct($type, $request_param = NULL, $from = 0, $group_by = NULL, $with_facets = TRUE) {
    global $language;
    $lang = $language->language;
    $page_size = MicaClient::getResponsePageSize();
    $query_dto_builder = JoinQueryDtoBuilder::newBuilder(json_decode($request_param), $page_size, $lang, $with_facets);
    DrSlump\Protobuf::setDefaultCodec(new DrSlump\Protobuf\Codec\Json());
    $json_query_encoded
      = DrSlump\Protobuf::encode($query_dto_builder->documentQuery($type, $from, $page_size, $group_by)
      ->build());

    return $json_query_encoded;
  }

  /**
   * Merge any associative array query parameters to current json parameter.
   *
   * @param array $param_to_add
   *   Associative $param_to_add associative array of param to add
   *   array('variables' =>array('terms'=>array('studyIds'=>'clsa-elcv-1'))).
   *
   * @return string
   *   The merged json DTO parameters.
   */
  public static function addParameterDtoQueryLink(array $param_to_add) {
    $url_parm = explode('?', request_uri());
    if (!empty($url_parm[1])) {
      parse_str($url_parm[1], $parts);
      if (!empty($parts['query'])) {
        $array_current_query = json_decode($parts['query']);
      }
      else {
        $array_current_query = new stdClass();
      }
      MicaClient::addParameterTerms($array_current_query, $param_to_add);
      return json_encode($array_current_query);
    }
    else {
      $array_current_query = new stdClass();
    }
    MicaClient::addParameterTerms($array_current_query, $param_to_add);
    return json_encode($array_current_query);
  }

  /**
   * Create a json query dto.
   *
   * @param array $param_to_add
   *   An array parameters ex :
   *   <code>
   *    array("variables" => array("terms" => array("dceIds" => $dce_id))).
   *   <code>
   *
   * @return string
   *   A Json string.
   */
  public static function createQueryDtoAsString(array $param_to_add) {
    return json_encode(MicaClient::createQueryDto($param_to_add));
  }

  /**
   * Create query Dto.
   *
   * @param array $param_to_add
   *   The parameters to add.
   *
   * @return object
   *   The Dto object query.
   */
  public static function createQueryDto(array $param_to_add) {
    $query = new stdClass();
    MicaClient::addParameterTerms($query, $param_to_add);
    return $query;
  }

  /**
   * Add parameters term to Dto query.
   *
   * @param object $array_current_query
   *   The current query.
   * @param array $param_to_add
   *   The parameters to add.
   */
  public static function addParameterTerms($array_current_query, array $param_to_add) {
    foreach ($param_to_add as $entity_name => $entity) {
      foreach ($entity['terms'] as $term_name => $term_value) {
        $term_values = array($term_value);
        if (is_array($term_value)) {
          $term_values = $term_value;
        }
        foreach ($term_values as $value) {
          if (empty($array_current_query->{$entity_name}->terms->{$term_name})
            || !in_array($value, $array_current_query->{$entity_name}->terms->{$term_name}->values)
          ) {
            $array_current_query->{$entity_name}->terms->{$term_name}->values[] = $value;
          }
        }

        if (!empty($array_current_query->{$entity_name}->terms->{$term_name}) &&
          empty($array_current_query->{$entity_name}->terms->{$term_name}->{'op'})
        ) {
          $array_current_query->{$entity_name}->terms->{$term_name}->{'op'} = 'and';
        }
      }
    }
  }

  /**
   * Concatenate terms parameters.
   *
   * @param array $array_query
   *   The query parameters.
   * @param array $param_to_add
   *   The parameters to add.
   *
   * @return array
   *   Concatenated query array.
   */
  public static function concatenateParameterTerms($array_query, array $param_to_add) {
    foreach ($param_to_add as $entity_name => $entity) {
      foreach ($entity['terms'] as $term_name => $term_value) {
        $term_values = array($term_value);
        if (is_array($term_value)) {
          $term_values = $term_value;
        }
        foreach ($term_values as $value) {
          if (empty($array_query->{$entity_name}->terms->{$term_name})
            || !in_array($value, $array_query->{$entity_name}->terms->{$term_name}->values)
          ) {
            $array_query->{$entity_name}->terms->{$term_name}->values[] = $value;
          }
        }
        if (!empty($array_query->{$entity_name}->terms->{$term_name}) &&
          empty($array_query->{$entity_name}->terms->{$term_name}->{'op'})
        ) {
          $array_query->{$entity_name}->terms->{$term_name}->{'op'} = 'and';
        }
      }
    }
    return $array_query;
  }

  /**
   * The query builder for charts.
   *
   * @param string $query
   *   The query for the charts.
   * @param object $bucket
   *   The bucket.
   * @param string $taxonomy_name
   *   The taxonomy name.
   * @param string $vocabulary_name
   *   The vocabulary name.
   * @param array $terms
   *   The terms.
   *
   * @return string
   *   A json query.
   */
  public static function chartQueryBuilders($query = NULL, $bucket = NULL, $taxonomy_name = NULL, $vocabulary_name = NULL, array $terms = NULL) {
    $parameters = array();
    $current_path_alias = drupal_get_path_alias();
    $current_path_alias = explode('/[', $current_path_alias)[0];
    $common_param
      = array(
      'variables' => array(
        'terms' => array(
          'attributes-'
          . $taxonomy_name . '__' .
          $vocabulary_name . '-und' => $terms,
        ),
      ),
    );
    if (!empty($query)) {
      return json_encode(MicaClient::concatenateParameterTerms(json_decode($query), $common_param));
    }
    if (strpos($current_path_alias, 'network')) {
      $parameters = array(
        'studies' => array(
          'terms' => array(
            $bucket->field => $bucket->value,
          ),
        ),
        'networks' => array(
          'terms' => array(
            'networkId' => arg(2, $current_path_alias),
          ),
        ),
      );
    }
    if (strpos($current_path_alias, 'study')) {
      $parameters = array(
        'studies' => array(
          'terms' => array(
            'studyIds' => arg(2, $current_path_alias),
          ),
        ),
      );
      $common_param
        = array(
        'variables' => array(
          'terms' => array(
            'attributes-'
            . $taxonomy_name . '__' .
            $vocabulary_name . '-und' => $terms,
            $bucket->field => $bucket->value,
          ),
        ),
      );
    }
    $term_param = array_merge($parameters, $common_param);
    return MicaClient::addParameterDtoQueryLink($term_param);

  }

  /**
   * Parse a string header response and return an key/value array attributes.
   *
   * @param string $raw_headers
   *   The raw header.
   *
   * @return array
   *   The parsed header on an array.
   */
  protected function parseHeaders($raw_headers) {
    $headers = [];

    foreach (explode("\n", $raw_headers) as $i => $h) {
      $h = explode(':', $h, 2);

      if (isset($h[1])) {
        $headers[$h[0]] = trim($h[1]);
      }
    }
    return $headers;
  }

  /**
   * Get property by given attribute.
   *
   * @param string $attribute
   *   The Attribute to get.
   * @param string $property
   *   The property.
   *
   * @return string
   *   The property value.
   */
  protected function parseHeader($attribute, $property) {
    $attributes_array = explode(';', $attribute);
    foreach ($attributes_array as $property_string) {
      if (strstr($property_string, $property)) {
        return $property_value = str_replace('"', '', explode('=', $property_string)[1]);
      }
    }
    return FALSE;
  }

  /**
   * Return the value of a given key header attribute  (filename, charset).
   *
   * ToDo may be need to be extended to retrieve more attributes(User-Agent,..).
   *
   * @param array $header
   *   The header to parse.
   * @param string $property
   *   The property to find.
   * @param string $attribute
   *   The attribute.
   *
   * @return string
   *   The value of property.
   */
  protected function getPropertyValueFromHeaderArray(array $header, $property, $attribute = NULL) {
    if (!empty($attribute)) {
      return $this->parseHeader($header[$attribute], $property);
    }
    else {
      foreach ($header as $attributes) {
        $find_property_value = $this->parseHeader($attributes, $property);
        if (!empty($find_property_value)) {
          return $find_property_value;
        }
      }
    }
    return NULL;
  }

  /**
   * Get the file extension from file resource.
   *
   * @param string $file_resource
   *   The file name.
   *
   * @return string
   *   The file extension.
   */
  protected function getFileExtension($file_resource) {
    $file_array = explode('.', $file_resource);
    $extension_file = count($file_array);

    return $file_array[$extension_file - 1];
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
  public function downloadFile($entity_type, $entity_id, $file_id) {
    $url = $this->micaUrl . "/" . $entity_type . "/" . $entity_id . "/file/" . $file_id . "/_download";
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => self::authorizationHeader(array(
          'Accept' => array(self::HEADER_BINARY),
        )
      ),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $file_name = $this->getPropertyValueFromHeaderArray($this->parseHeaders($client->lastResponse->headers),
        'filename',
        'Content-Disposition'
      );

      $raw_data = array(
        'extension' => $this->getFileExtension($file_name),
        'data' => $data,
        'filename' => $file_name,
        'raw_header_array' => $this->parseHeaders($client->lastResponse->headers),
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
