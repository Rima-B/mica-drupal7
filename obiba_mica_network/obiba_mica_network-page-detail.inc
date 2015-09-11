<?php

/*
 * @file code for network detail pages
 * */
include_once('includes/obiba_mica_network_resource.inc');

$path_module_study = drupal_get_path('module', 'obiba_mica_study');
include_once($path_module_study . '/obiba_mica_study-page-detail.inc');

function obiba_mica_network_page_detail($network_id) {
  obiba_mica_network_load_charts_libs();
  drupal_add_js(drupal_get_path('module', 'obiba_mica_network') . '/js/mica-network-detail.js', 'file');
  drupal_add_js(array('network_url' => rawurlencode($network_id)), 'setting');

  $url_param = explode('?', request_uri());
  if (!empty($url_param[1])) {
    parse_str($url_param[1], $parts);
  }

  $network_resource = new MicaNetworkResource();
  $network_dto = $network_resource->get_network($network_id);

  if (!empty($network_dto)) {

    if (!empty($network_dto->studySummaries)) {
      obiba_mica_network_studies_datatable_init($network_dto->studySummaries);
    }

    $acronym = obiba_mica_commons_get_localized_field($network_dto, 'acronym');
    $name = obiba_mica_commons_get_localized_field($network_dto, 'name');
    drupal_set_title($acronym . ' - ' . $name);

    drupal_set_breadcrumb(array(
      l(t('Home'), '<front>'),
      l(t('Networks'), MicaClientPathProvider::NETWORKS),
    ));

    return theme('obiba_mica_network-detail', array(
      'investigators_modal' => obiba_mica_study_get_contact_detail('investigator', $network_dto->id, $network_dto->investigators),
      'contacts_modal' => obiba_mica_study_get_contact_detail('contact', $network_dto->id, $network_dto->contacts),
      'network_dto' => $network_dto,
      'has_variables' => _obiba_mica_network_has_variables($network_dto->studySummaries),
      'group_by' => !empty($parts['group-by']) ? $parts['group-by'] : 'studyIds',
    ));
  }
  else {
    drupal_set_title(t('Network not found'));
    return '';
  }
}

function obiba_mica_network_page_detail_coverage($network_id) {
  $network_resource = new MicaNetworkResource();
  $network_dto = $network_resource->get_network($network_id);
  $coverage = obiba_mica_network_coverage($network_dto->studyIds);

  if (empty($coverage)) {
    exit;
  }

  print theme('obiba_mica_network_detail_coverage', array('coverage' => $coverage));
}

/**
 * @param $network_dto
 */
function obiba_mica_network_studies_datatable_init($studySummaries) {
  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  }
  else {
    obiba_mica_commons_datatables_external_source();
  }

  drupal_add_js(drupal_get_path('module', 'obiba_mica_network') . '/js/obiba_mica_network_studies_datatable_init.js', 'file');
  drupal_add_js(array(
    'table_headers' => obiba_mica_network_study_table_headers(),
    'table_data' => obiba_mica_network_study_table_rows($studySummaries)
  ), 'setting');
}

function obiba_mica_network_study_table_rows($study_summaries) {
  $rows = array();
  if (!empty($study_summaries)) {
    foreach ($study_summaries as $study_summary) {
      $name = obiba_mica_commons_get_localized_field($study_summary, 'name');
      $rows[] = array(
        obiba_mica_commons_get_localized_field($study_summary, 'acronym'),
        $study_summary->getPublished() ? MicaClientAnchorHelper::study($study_summary) : t($name),
        implode(', ', obiba_mica_commons_clean_string($study_summary->designs)),
        isset($study_summary->targetNumber->noLimit) ? t('No Limit') :
          isset($study_summary->targetNumber->number) ? obiba_mica_commons_format_number($study_summary->targetNumber->number) : NULL,
        obiba_mica_commons_countries($study_summary->countries),
        empty($study_summary->variables)
          ? '-' : MicaClientAnchorHelper::studyVariables(obiba_mica_commons_format_number($study_summary->variables), $study_summary->id)
      );

    }
  }
  return $rows;
}

function obiba_mica_network_study_table_headers() {
  return array(
    array("title" => t('Acronym')),
    array("title" => t('Name'), 'width' => '45%'),
    array("title" => t('Study Design'), 'width' => '20%'),
    array("title" => t('Participants'), 'width' => '15%'),
    array("title" => t('Country')),
    array("title" => t('Variables'))
  );
}

function obiba_mica_network_coverage($study_ids) {
  return obiba_mica_search_query_charts(MicaClient::addParameterDtoQueryLink(array(
      'variables' =>
        array('terms' => array('studyIds' => $study_ids))
    )), NULL, NULL,
    array('group-by' => 'studyIds'));
}

function _obiba_mica_network_has_variables($study_summaries) {
  if (!empty($study_summaries)) {
    foreach ($study_summaries as $study_summary) {
      if (!empty($study_summary->variables)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

