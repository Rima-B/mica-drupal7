<?php
/*
 * @file code for list networks page
 * */
include_once('includes/obiba_mica_network_resource.inc');

function obiba_mica_network_page_list() {
  $context['url'] = 'refresh_list_networks';
  drupal_add_js(drupal_get_path('module', 'obiba_mica_commons') . '/js/ajax_getlist_items.js', 'file');
  drupal_add_js(array('context' => $context), 'setting');

  $node_rendered = obiba_mica_commons_get_translated_node(variable_get('networks_page'));

  drupal_set_title($node_rendered['#node']->title);

  $query = NULL;
  $sort = NULL;
  $sort_by = NULL;
  $list_networks = '';
  $network_limits = NULL;
  if (!empty($_GET['search-query'])) {
    $query = $_GET['search-query'];
  }
  if (!empty($_GET['search-sort'])) {
    $sort = $_GET['search-sort'];
  }
  if (!empty($_GET['search-sort-order'])) {
    $sort_by = $_GET['search-sort-order'];
  }

  if (!empty($_GET['search-nbr-result'])) {
    $network_limits = $_GET['search-nbr-result'];
  }

  $response = obiba_mica_network_get_networks(NULL, $query, $sort, $sort_by, $network_limits);
  $total_networks = empty($response) ? 0 : $response->getTotalHits();
  $network_limits = !empty($network_limits) ? $network_limits : MicaClient::getResponsePageSizeSmall();
  pager_default_initialize($total_networks, $network_limits);
  $pager_output = theme('pager', array('quantity' => $network_limits));
  $summaries = empty($response) ? array() : $response->getSummaries();
  if (!empty($summaries)) {
    foreach ($summaries as $network) {
      if (!empty($network->logo->id)) {
        $logo_url = obiba_mica_commons_safe_expose_server_url($network->id, $network->logo, 'network');
      }
      else {
        $logo_url = NULL;
      }

      $list_networks .= theme('obiba_mica_network-list-page-block', array(
        'network' => $network,
        'logo_url' => $logo_url,
        'digests' => $network
      ));
    }
  }
  else {
    $list_networks = t(variable_get_value('networks_empty_label'));
  }

  return theme('obiba_mica_network-list', array(
    "form_search" => drupal_get_form('obiba_mica_network_create_form'),
    'node_page' => $node_rendered,
    "total_items" => $total_networks,
    'list_networks' => $list_networks,
    'pager_wrap' => $pager_output
  ));
}

function obiba_mica_network_get_networks($study_id, $query = NULL, $sort = NULL, $sort_by = NULL, $network_limits = NULL) {
  $from = 0;
  $study_resource = new MicaNetworkResource();
  $size_response = !empty($network_limits) ? $network_limits : MicaClient::getResponsePageSizeSmall();
  if (!empty($_GET['page'])) {
    $from = $study_resource->paginationListSearchParameters($_GET['page'], 'networks', $size_response);
  }
  $networks = $study_resource->get_networks($study_id, $query, $from, $size_response, $sort, $sort_by);
  if (!empty($networks)) {
    $_SESSION['networks']['aggregations']['total_hits'] = $networks->getTotalHits();
  }
  return $networks;
}

function obiba_mica_network_create_form() {
  $context =
    array(
      'options' => array('script' => t('Relevance')),
      'default_sort' => 'script',
      'default_sort_order' => 'desc'
    );

  return obiba_mica_commons_create_search_form($context);
}

function obiba_mica_network_networks_table($networks_result) {
  $rows = array();
  $total = $networks_result->getTotalHits();
  if ($total > 0) {
    $rows = obiba_mica_network_networks_table_rows($networks_result->getSummaries());
  }
  $headers = obiba_mica_network_networks_table_headers();
  $per_page = 20;

  if (empty($total)) {
    return NULL;
  }

  $args = array('quantity' => $total, 'element' => MicaNetworkResource::PAGINATE_NETWORKS);
  $current_page = pager_default_initialize($total, $per_page, MicaNetworkResource::PAGINATE_NETWORKS);
  $chunks = array_chunk($rows, $per_page, TRUE);
  return theme(
    'table',
    array(
      'header' => $headers,
      'rows' => empty($chunks) ? array() : $chunks[$current_page],
      'empty' => t(variable_get_value('networks_empty_label'))
    )
  ) . theme('pager', $args);
}

function obiba_mica_network_networks_table_rows($networks) {
  $rows = array();

  if (!empty($networks)) {
    foreach ($networks as $network) {
      $stats = $network->{'obiba.mica.CountStatsDto.networkCountStats'};
      $study_count = MicaClientAnchorHelper::networkStudies($stats->studies, $network->id);
      $rows[] = array(
        obiba_mica_commons_get_localized_field($network, 'acronym'),
        MicaClientAnchorHelper::network($network),
        $study_count,
      );
    }
  }
  return $rows;
}

function obiba_mica_network_retrieve_networks($query, $sort = NULL, $sort_order = NULL) {
  $list_networks = obiba_mica_network_get_networks(NULL, $query, $sort, $sort_order);
  $list_networks_rendered = '';
  foreach ($list_networks->getSummaries() as $network) {
    $list_networks_rendered .= theme('obiba_mica_network-list-page-block', array(
      'network' => $network
    ));
  }
  $data['list'] = $list_networks_rendered;
  $data['total'] = $list_networks->total;
  drupal_json_output($data);
}

function obiba_mica_network_networks_table_headers() {
  return array(
    t('Acronym'),
    t('Name'),
    t('Studies'),
  );
}




