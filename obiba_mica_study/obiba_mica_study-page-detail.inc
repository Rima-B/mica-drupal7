<?php
/**
 * @file
 * Mica studies file
 */

if (module_exists('obiba_mica_dataset')) {
  $path_module_dataset = drupal_get_path('module', 'obiba_mica_dataset');
  include_once($path_module_dataset . '/includes/obiba_mica_dataset_resource.inc');
}

function obiba_mica_study_page_detail_coverage($study_id) {
  if (!variable_get_value('study_show_coverage_graphs')) {
    exit;
  }

  $coverage = obiba_mica_study_coverage($study_id);

  if (empty($coverage['charts'])) {
    exit;
  }

  $_SESSION['fixed_menu']['coverage'] = $coverage['charts'];
  print theme('obiba_mica_study_detail_coverage', array('coverage' => $coverage['charts']));
}

function obiba_mica_study_page_detail_networks($study_id) {
  if (!variable_get_value('study_show_networks')) {
    exit;
  }

  $network_output = (module_exists('obiba_mica_network')) ?
    obiba_mica_network_networks_list($study_id) : NULL;

  $_SESSION['fixed_menu']['networks'] = $network_output;

  if (empty($network_output)) {
    exit;
  }

  print theme('obiba_mica_study_detail_networks', array('networks' => $network_output));
}

function obiba_mica_study_page_detail_datasets($study_id) {
  if (!variable_get_value('study_show_datasets')) {
    exit;
  }

  $dataset_output = (module_exists('obiba_mica_dataset')) ? obiba_mica_dataset_get_all_datasets_studies($study_id) : NULL;

  if (empty($dataset_output)) {
    exit;
  }

  $_SESSION['fixed_menu']['datasets'] = $dataset_output;
  print theme('obiba_mica_study_detail_datasets', array('datasets' => $dataset_output));
}

function obiba_mica_study_page_detail($study_id) {
  obiba_mica_study_load_libs();
  drupal_add_js(drupal_get_path('module', 'obiba_mica_study') . '/js/mica-study-detail.js', 'file');
  drupal_add_js(array('study_url' => rawurlencode($study_id)), 'setting');

  $url_param = explode('?', request_uri());
  if (!empty($url_param[1])) {
    parse_str($url_param[1], $parts);
  }

  $page_render_study = NULL;
  $themed_attachment = NULL;
  $flat_study_attachments = NULL;
  $study_resource = new MicaStudyResource();
  $study_dto = $study_resource->get_study($study_id);
  if (!empty($study_dto)) {
    $acronym = obiba_mica_commons_get_localized_field($study_dto, 'acronym');
    $name = obiba_mica_commons_get_localized_field($study_dto, 'name');
    $title_order_option = variable_get_value('study_title_acronym_display');
    if (!empty($title_order_option)) {
      if ($title_order_option == 1) {
        drupal_set_title($acronym . ' - ' . $name);
      }
      if ($title_order_option == 2) {
        drupal_set_title($name);
      }
    }
    else {
      drupal_set_title($name . ' - ' . $acronym);
    }

    drupal_set_breadcrumb(array(
      l(t('Home'), '<front>'),
      l(t('Studies'), 'mica/studies'),
    ));

    $opal_output = !empty($study_dto->opal) ? obiba_mica_study_get_opal_url($study_dto->opal) : NULL;

    $_SESSION['fixed_menu'] = array(
      'marker_paper' => !empty($study_dto->markerPaper),
      'pubmed_id' => !empty($study_dto->pubmedId),
      'info' => !empty($study_dto->info),
      'attachments' => !empty($study_dto->attachments),
      'coverage' => NULL,
      'networks' => NULL,
      'datasets' => NULL,
      'study_variables_aggs' => NULL,
      //obiba_mica_study_get_variables($study_id),
    );
    if (module_exists('obiba_mica_persons') && !empty($study_dto)) {
      $investigator_detail_modal = obiba_mica_persons_get_person_detail('investigator', $study_dto->id, $study_dto->investigators);
      $contact_detail_modal = obiba_mica_persons_get_person_detail('contact', $study_dto->id, $study_dto->contacts);
    }
    if (module_exists('obiba_mica_files')) {
      drupal_add_js(drupal_get_path('module', 'obiba_mica_files') . '/js/obiba_mica_files_attachment_download.js');
      $flat_study_attachments = obiba_mica_files_get_flat_attachments('study', $study_id);
      $filtered_tree_path = $flat_study_attachments->findNodeFullPath('/study/'.$study_id);
      if (!empty($filtered_tree_path)) {
        $themed_attachment = $flat_study_attachments->themeFolders($filtered_tree_path, MicaFilesResource::POPULATION,$study_id);
      }
    }
    $populations = obiba_mica_study_get_populations_detail($study_dto, NULL, NULL, $flat_study_attachments);
    return theme('obiba_mica_study_detail',
      array(
        'investigators_modal' => !empty($investigator_detail_modal) ? $investigator_detail_modal : NULL,
        'contacts_modal' => !empty($contact_detail_modal) ? $contact_detail_modal : NULL,
        'study_dto' => $study_dto,
        'attachments' => !empty($themed_attachment) ? $themed_attachment : NULL,
        'timeline' => obiba_mica_study_get_timeline_detail($study_dto),
        'populations' => $populations,
        'opal' => $opal_output,
        'study_variables_aggs' => NULL,
        //obiba_mica_study_get_variables($study_id),
        'group_by' => !empty($parts['group-by']) ? $parts['group-by'] : NULL,
      ));
  }
  else {
    drupal_set_title(t('Study not found'));
    return '';
  }
}

function obiba_mica_study_get_opal_url($opal_url) {
  return theme('obiba_mica_study_opal', array('opal_url' => $opal_url));
}

/**
 * Implements hook_library().
 */
function obiba_mica_study_library() {
  $libraries = array();
  $version_lib = variable_get('obiba-time-line-version', '1.0');
  $lib_path = libraries_get_path('mica-study-timeline-' . $version_lib);
  // Make sure we have a valid library path before returning library load info.
  if (!empty($lib_path)) {
    $libraries['mica-study-timeline'] = array(
      'title' => 'MicaTimeLine',
      'website' => 'http://obiba.org/',
      'version' => '1.0',
      'js' => array(
        $lib_path . '/dist/d3.min.js' => array(),
        $lib_path . '/dist/mica-study-timeline.min.js' => array()
      ),
      'css' => array(
        $lib_path . '/dist/mica-study-timeline.min.css' => array(),
      )
    );
  }

  return $libraries;
}

/*
 * Format time-line schema
 * */
function obiba_mica_study_get_timeline_detail($study_detail) {
  $study_json = json_encode($study_detail);
  drupal_add_css(drupal_get_path('module', 'obiba_mica_study') . '/css/mica_studies.css', 'file');
  drupal_add_library('obiba_mica_study', 'mica-study-timeline');
  $page_render_timeline = '<div id="vis" style="width=100%"></div>';
  drupal_add_js(drupal_get_path('module', 'obiba_mica_study') . '/js/obiba-mica-study-timeline.js', 'file');
  drupal_add_js(array('obiba_mica_study' => array('study_json' => $study_json)), 'setting');

  return $page_render_timeline;
}

/*
 * Format populations schema
 * */

function obiba_mica_study_get_populations_detail($study_detail, $dce_variables_nbr = NULL, $has_coverage = NULL, $flat_study_attachments = NULL) {
  $page_render_populations = array();
  $themed_attachment = NULL;
  if (!empty($study_detail->populations)) {
    foreach ($study_detail->populations as $population) {
      if (!empty($flat_study_attachments) && module_exists('obiba_mica_files')) {
          $path_population_attachment = "/study/$study_detail->id/population/$population->id";
          $filtered_tree_path = $flat_study_attachments->findNodeFullPath($path_population_attachment);
          $themed_attachment = $flat_study_attachments->themeFolders($filtered_tree_path, MicaFilesResource::DCE,$population->id );
      }
      $dce_uids = array();
      if (!empty($population->dataCollectionEvents)) {
        $dce_rows = obiba_mica_study_get_dce_table($population->dataCollectionEvents, $study_detail->id, $population->id);
        uasort($dce_rows, 'obiba_mica_study_sort_dce');
        obiba_mica_commons_normalize_rows($dce_rows);

        $population['dce-tab'] = theme(
          'table',
          array(
            'header' => array(
              t('Name'),
              t('Description'),
              t('Start'),
              t('End')
            ),
            'rows' => $dce_rows,
            'empty' => t('No data collection event found'),
            'sticky' => FALSE,
            'attributes' => array('id' => 'variables_overview')
          )
        );
        $population['dce-modal'] = obiba_mica_study_get_dce_detail(
          $study_detail->id, $population->id,
          $population->dataCollectionEvents,
          $dce_variables_nbr,
          $has_coverage,
          $flat_study_attachments
        );

        foreach ($population->dataCollectionEvents as $dce) {
          $dce_uids[] = $study_detail->id . ':' . $population->id . ':' . $dce->id;
        }
      }

      $page_render_populations[$population->id] = array(
        'data' => $population,
        'html' => theme('mica_population_detail', array(
          'population_content' => theme('mica_population_content_detail', array(
            'dce_uids' => $dce_uids,
            'population' => $population,
            'attachments' => !empty($themed_attachment) ? $themed_attachment : NULL,
          )),
          'population' => $population,
        ))
      );
    }
  }

  return $page_render_populations;
}

/**
 * Create modal DOM for data collection events
 *
 * @param $study_id
 * @param $data_collection_events
 *
 * @return string
 */
function obiba_mica_study_get_dce_detail($study_id, $population_id, $data_collection_events, $dce_variables_nbr = NULL, $has_coverage = NULL, $flat_study_attachments = NULL) {
  $page_render_dces = '';
  $themed_dce_attachment = NULL;
  foreach ($data_collection_events as $dce) {
    if (module_exists('obiba_mica_files')) {
        $path_dce_attachment = "/study/$study_id/population/$population_id/data-collection-event/$dce->id";
        $filtered_dce_tree_path = $flat_study_attachments->findNodeFullPath($path_dce_attachment);
      if($population_id==1 && $dce->id==2){
//        dvm($filtered_dce_tree_path);
      }
        $themed_dce_attachment = $flat_study_attachments->themeFolders($filtered_dce_tree_path,NULL,  $dce->id);
    }
    $page_render_dces .= theme('mica_dce_detail', array(
      'has_coverage' => $has_coverage,
      'dce_variables_nbr' => $dce_variables_nbr,
      'study_id' => $study_id,
      'population_id' => $population_id,
      'dce_uid' => $study_id . ':' . $population_id . ':' . $dce->id,
      'dce_id_target' => MicaClientPathHelpers::replacePlusInUrl($study_id . '-' . $population_id . '-' . $dce->id),
      'dce' => $dce,
      'attachments' => !empty($themed_dce_attachment) ? $themed_dce_attachment : NULL,
    ));
  }
  return $page_render_dces;
}

/*
 * retrieve Data Collection event
 * */
function obiba_mica_study_get_dce_table($data_collection_events, $study_id, $population_id) {
  $dce_rows = array();
  foreach ($data_collection_events as $key_dce => $dce_wrapper) {
    $dce_description = NULL;
    if (!empty($dce_wrapper->description)) {
      $dce_description = truncate_utf8(strip_tags(obiba_mica_commons_get_localized_field($dce_wrapper, 'description')), 50, TRUE, TRUE);
    }
    // Hide DCE that are not published on study published page

    $dce_rows[$key_dce] = array(
      'data' => array(
        'label' => l(obiba_mica_commons_get_localized_field($dce_wrapper, 'name'), '#', array(
          'external' => TRUE,
          'attributes' => array(
            'data-toggle' => 'modal',
            'data-target' => MicaClientPathHelpers::replacePlusInUrl('#dce-' . $study_id . '-' . $population_id . '-' . $dce_wrapper->id)
          )
        )),
        'desc' => $dce_description,
        'start_year' => $dce_wrapper->startYear,
        'start_month' => $dce_wrapper->startMonth,
        'end_year' => $dce_wrapper->endYear,
        'end_month' => $dce_wrapper->endMonth,
      )
    );
  }
  return $dce_rows;
}

/**
 * Get the facets of the variables for this study.
 *
 * @param $dataset_id
 *
 * @return array
 */
function obiba_mica_study_get_variables($study_id) {
  $dataset_resource = new MicaDatasetResource();
  return $dataset_resource->variables_search(array(
    0 => array(
      'field' => 'studyId',
      'terms' => array($study_id)
    )
  ), 0);
}

function obiba_mica_study_coverage($study_id) {
  $coverage['charts'] = obiba_mica_search_query_charts(
    MicaClient::addParameterDtoQueryLink(array('variables' => array(
      'terms' => array(
        'studyIds' => $study_id)))),
    function ($b, $study_id) {
      // filter the dces of the current study
      return strpos($b->value, ':') === FALSE || strpos($b->value, $study_id . ':') === 0;
    },
    $study_id,
    array('group-by' => 'dceIds', 'with-facets' => 'false'));
  if (!empty($coverage['charts'])) {
    $coverage['has_coverage'] = TRUE;
    return $coverage;
  }
  else {
    return FALSE;
  }
}