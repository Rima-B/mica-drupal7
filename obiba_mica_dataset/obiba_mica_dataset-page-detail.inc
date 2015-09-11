<?php
/*
 * @fileCode for dataset detail page
 * */

include_once('includes/obiba_mica_dataset_resource.inc');

function obiba_mica_dataset_page_detail($dataset_type, $dataset_id) {
  global $language;

  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  }
  else {
    obiba_mica_commons_datatables_external_source();
  }
  drupal_add_js(drupal_get_path('module', 'obiba_mica_commons') . '/js/ajax_getlist_items.js', 'file');
  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_datatable_init.js', 'file');
  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_toggle_harmo_table.js', 'file');
  $dataset_resource = new MicaDatasetResource();
  $dataset_dto = NULL;
  $dataset_type_dto = NULL;
  $variables_dataset = NULL;
  $variables_table_output = NULL;
  $modals = NULL;
  if (!empty($_SESSION['ajax'])) {
    unset($_SESSION['ajax']);
  }
  $dataset_dto = $dataset_resource->get_dataset($dataset_type, $dataset_id);
  $_SESSION['ajax']['variable'] = serialize($dataset_dto);
  $coverage = obiba_mica_dataset_coverage($dataset_dto->id);
  if (!empty($dataset_dto)) {
    $variables_dataset = $dataset_resource->get_variables($dataset_type, $dataset_id, 0, 1);
    if ($dataset_type == 'study-dataset') {
      $dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.StudyDatasetDto.type');
      $has_coverage = !empty($coverage['has_coverage']) ? $coverage['has_coverage'] : NULL;
      $modals = obiba_mica_dataset_get_population_dce_modals(
        $dataset_type_dto->studyTable->studyId,
        obiba_mica_dataset_get_population_dce($dataset_type_dto),
        $variables_dataset,
        $has_coverage
      );
    }
    else {
      $dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.HarmonizationDatasetDto.type');
      obiba_mica_dataset_studies_datatable_init($dataset_type_dto);
    }
    $acronym = obiba_mica_commons_get_localized_field($dataset_dto, 'acronym');
    $name = obiba_mica_commons_get_localized_field($dataset_dto, 'name');
    $page_title = (variable_get_value('dataset_title_acronym_setup') ? ($acronym . ' - ') : '') . $name;
    drupal_set_title($page_title);

    drupal_set_breadcrumb(array(
      l(t('Home'), '<front>'),
      l(t('Datasets'), 'mica/datasets'),
      l(($dataset_type == 'study-dataset' ? t('Study Datasets') : t('Harmonization Datasets')), 'mica/datasets/' . $dataset_type . 's')
    ));

    return theme('obiba_mica_dataset-detail', array(
      'dataset_dto' => $dataset_dto,
      'coverage' => !empty($coverage) ? $coverage['charts'] : NULL,
      'dataset_type_dto' => $dataset_type_dto,
      'dataset_type' => $dataset_type,
      'harmonization_table_legend' => theme('obiba_mica_dataset-harmonization-table-legend', array('clickable' => TRUE)),
      'variables_dataset' => $variables_dataset,
      'modals' => $modals,
    ));
  }
  else {
    drupal_set_title(t('Dataset not found'));
    return '';
  }
}

function obiba_mica_dataset_get_population_dce($dataset_type_dto) {
  $data = array('study' => NULL, 'population' => NULL, 'dce' => NULL);
  $studyTable = $dataset_type_dto->studyTable;
  $study_resource = new MicaStudyResource();
  $study_dto = $study_resource->get_study($studyTable->studyId);

  if (!empty($study_dto)) {
    $data ['study'] = $study_dto;
    $population = array_filter($study_dto->populations, function ($population) use ($studyTable) {
      return $studyTable->populationId === $population->id;
    });

    if (!empty($population)) {
      $data['population'] = $population;
      $population = array_pop($population);
      if (!empty($population->dataCollectionEvents)) {
        $data['dce'] = array_filter($population->dataCollectionEvents, function ($dce) use ($studyTable) {
          return $studyTable->dataCollectionEventId === $dce->id;
        });
      }
    }
  }

  return $data;
}

function obiba_mica_dataset_get_population_dce_modals($studyId, $data, $variables_dataset = NULL, $has_coverage = NULL) {
  $modals = array();
  $populations = $data['population'];
  if (!empty($populations)) {
    foreach ($populations as $population) {

      foreach ($population->dataCollectionEvents as $dce) {
        $dce_uids[] = $studyId . ':' . $population->id . ':' . $dce->id;
      }

      $modals['population'][] = theme('mica_population_content_detail_modal', array(
        'population' => $population,
        'population_content' => theme('mica_population_content_detail', array(
          'dce_uids' => $dce_uids,
          'population' => $population,
          'hide_title' => TRUE
        ))
      ));

      $dces = $data['dce'];
      $themed_attachment = '';
      $populationId = $population->id;
      $dce_variables_nbr = array();
      foreach ($dces as $dce) {
        $study_dto = $data['study'];
        if (!empty($study_dto->attachments)) {
          foreach ($study_dto->attachments as $attachment) {
            $themed_attachment .= theme('obiba_mica_study_attachments', array(
              'study_id' => $study_dto->id,
              'attachment' => $attachment
            ));
          }
        }
        $dce_variables_nbr[obiba_mica_commons_get_localized_field($dce, 'name')] = $variables_dataset->total;
        $modals['dce'][] = theme('mica_dce_detail', array(
          'has_coverage' => $has_coverage,
          'dce_variables_nbr' => $dce_variables_nbr,
          'study_id' => $studyId,
          'population_id' => $populationId,
          'dce_uid' => $studyId . ':' . $populationId . ':' . $dce->id,
          'dce_id_target' => $studyId . '-' . $populationId . '-' . $dce->id,
          'dce' => $dce,
          'attachements' => $themed_attachment
        ));
      }
    }
  }

  return $modals;
}

/*
 * Ajax Variable table Call header
 * */
function obiba_mica_dataset_Variables_table_ajax_header($dataset_type) {
  $dataset_dto = unserialize($_SESSION['ajax']['variable']);
  if ($dataset_type == 'study-dataset') {
    // $dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.StudyDatasetDto.type');
    $header = obiba_mica_dataset_studies_Variables_table_ajax_header();
  }
  else {
    $dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.HarmonizationDatasetDto.type');
    $header = obiba_mica_dataset_harmonizations_Variables_table_ajax_header($dataset_type_dto);
  }

  $data['header'] = $header;
  return drupal_json_output($data);
}

function obiba_mica_dataset_get_sorted_by_column() {
  if (!empty($_GET['sColumns'])) {
    $_columns = explode(',', $_GET['sColumns']);
    foreach ($_columns as $key => $_column) {
      if (!empty($_GET['bSortable_' . $key])) {
        if ($_GET['iSortCol_0'] == $key) {
          $sorting['sort'] = $_column;
          $sorting['order'] = $_GET['sSortDir_0'];
          return $sorting;
        }
      }
    }
  }
}

/*
 * Ajax Variable table Call data
 * */

function obiba_mica_dataset_Variables_table_ajax_data($dataset_type, $id_dataset) {
  $dataset_resource = new MicaDatasetResource();
  $from = 0;
  $size = 10;
  $dataset_sort_by = NULL;
  $dataset_query = NULL;

  if (isset($_GET['iDisplayStart'])) {
    $from = $_GET['iDisplayStart'];
  }
  if (isset($_GET['iDisplayLength'])) {
    $size = $_GET['iDisplayLength'];
  }

  $sort = obiba_mica_dataset_get_sorted_by_column($dataset_type);

  $dataset_dto = unserialize($_SESSION['ajax']['variable']);

  if ($dataset_type == 'study-dataset') {
    //$dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.StudyDatasetDto.type');
    $variables_dataset = $dataset_resource->get_variables($dataset_type,
      $id_dataset,
      $from,
      $size,
      !empty($sort['sort']) ? $sort['sort'] : 'index',
      !empty($sort['order']) ? $sort['order'] : NULL
    );

    $data_variable = obiba_mica_dataset_studies_table_data($variables_dataset);

  }
  else {
    $dataset_type_dto = $dataset_dto->getExtensionList('obiba.mica.HarmonizationDatasetDto.type');
    $variables_dataset = $dataset_resource->get_variables_harmonizations($dataset_type,
      $id_dataset,
      $from,
      $size,
      !empty($sort['sort']) ? $sort['sort'] : 'index',
      !empty($sort['order']) ? $sort['order'] : NULL
    );
    $data_variable = obiba_mica_dataset_harmonizations_table_data($dataset_type_dto, $variables_dataset);
  }

  $data['sEcho'] = $_GET["sEcho"] ? $_GET["sEcho"] : 1;
  $data['iTotalRecords'] = $variables_dataset->total;
  $data['iTotalDisplayRecords'] = $variables_dataset->total;
  $data['aaData'] = $data_variable;
  return drupal_json_output($data);
}

/*
 * studies dataset ajax variable table
 *
 * */
function obiba_mica_dataset_studies_Variables_table_ajax_header() {
  return array(
    array(
      "sTitle" => '',
      "data" => "index",
      "mDataProp" => "index",
      "name" => "index",
      "visible" => FALSE
    ),
    array(
      "sTitle" => t('Variable'),
      "data" => "name",
      "mDataProp" => "name",
      "name" => "name",
      "width" => "25%"
    ),
    array(
      "sTitle" => t('Label'),
      "data" => "label",
      "mDataProp" => "label",
      "name" => "label",
      "orderable" => FALSE
    ),
    array(
      "sTitle" => t('Classifications'),
      "data" => "classifications",
      "mDataProp" => "classifications",
      "name" => "classifications",
      "orderable" => FALSE
    )
  );
}

function obiba_mica_dataset_studies_table_data($variables_dataset) {
  $rows = array();
  if (!empty($variables_dataset->variables)) {
    foreach ($variables_dataset->variables as $variable) {
      $var_row = l($variable->name, 'mica/variable/' . $variable->id);
      $domains = _obiba_mica_variable_domains_list($variable);
      $rows[] = array(
        "index" => '',
        "name" => $var_row,
        "label" => _obiba_mica_variable_attributes_detail($variable, 'label'),
        "classifications" => $domains
      );
    }
  }
  return $rows;
}

/*********End Studies ajax header *************/

/*
 * Harmonization ajax variable table
 * */
function obiba_mica_dataset_harmonizations_Variables_table_ajax_header($dataset_type_dto) {
  $nbr_studies = count($dataset_type_dto->studyTables);
  $witdh_column_study = (100 - (1 + 25)) / $nbr_studies;
  if (!empty($dataset_type_dto->studyTables)) {
    $header[] = array(
      "sTitle" => '',
      "data" => "index",
      "name" => "index",
      "mDataProp" => "index",
      "visible" => FALSE
    );

    $header[] = array(
      "sTitle" => t('Variable'),
      "data" => "variable",
      "name" => "name",
      "mDataProp" => "name",
      "width" => "25%",
      "class" => "study-title",
    );
    foreach ($dataset_type_dto->studyTables as $study_table) {
      $study_summary = $study_table->studySummary;
      $acronym = obiba_mica_commons_get_localized_field($study_summary, 'acronym');
      $title_header = l($acronym, 'mica/study/' . $study_summary->id);
      $title_header = str_replace('"', '\'', $title_header);
      if (!empty($study_table->name)) {
        $name = ' <small title="' . obiba_mica_commons_get_localized_field($study_table, 'description') . '">' . obiba_mica_commons_get_localized_field($study_table, 'name') . '</small>';
        $name = str_replace('"', '\'', $name);
        $header[] = array(
          "data" => str_replace(' ', '_', $acronym . obiba_mica_commons_get_localized_field($study_table, 'name')),
          "sTitle" => $title_header . $name,
          "name" => str_replace(' ', '_', $acronym . obiba_mica_commons_get_localized_field($study_table, 'name')),
          "orderable" => FALSE,
          "width" => $witdh_column_study . "%",
          "class" => "text-center study-title"
        );
      }
      else {
        $header[] = array(
          "data" => $acronym,
          "sTitle" => $title_header,
          "name" => $acronym,
          "orderable" => FALSE,
          "width" => $witdh_column_study . "%",
          "class" => "text-center study-title"
        );
      }
    }
  }
  return $header;
}

function obiba_mica_dataset_harmonizations_table_data($dataset_type_dto, $dataset_harmonizations_dto) {
  $rows = array();

  if (!empty($dataset_type_dto->studyTables)) {

    if (!empty($dataset_harmonizations_dto->variableHarmonizations)) {

      foreach ($dataset_harmonizations_dto->variableHarmonizations as $variable_harmonization) {

        $row = array(
          "index" => '',
          "variable" =>
            l($variable_harmonization->resolver->name, 'mica/variable/' . $variable_harmonization->resolver->id)
        );

        foreach ($dataset_type_dto->studyTables as $study_table) {
          $found = FALSE;
          /*********************/
          $study_summary = $study_table->studySummary;
          $acronym = obiba_mica_commons_get_localized_field($study_summary, 'acronym');
          if (!empty($study_table->name)) {
            $key_row = $acronym . obiba_mica_commons_get_localized_field($study_table, 'name');
          }
          else {
            $key_row = $acronym;
          }
          $key_row = str_replace(' ', '_', $key_row);
          /*********************/
          foreach ($variable_harmonization->datasetVariableSummaries as $variable_summary) {
            if ($variable_summary->resolver->studyId == $study_table->studyId
              && $variable_summary->resolver->project == $study_table->project
              && $variable_summary->resolver->table == $study_table->table
            ) {
              $status = trim(strtolower(_obiba_mica_variable_attributes_detail($variable_summary, 'status', array(
                'maelstrom',
                'Mlstr_harmo'
              ))));
              $harmonized_variable_id = $variable_summary->resolver->id;
              $variable_title = $variable_summary->resolver->name . ' -- ' . obiba_mica_commons_get_localized_field($study_table->studySummary, 'acronym');
              if (!empty($study_table->name)) {
                $variable_title = $variable_title . ' (' . obiba_mica_commons_get_localized_field($study_table, 'name') . ')';
              }
              if (empty($status)) {
                $icon_alert_no_status = '<span data-toggle="tooltip" class="glyphicon glyphicon-question-sign alert-warning" data-original-title="' . t('No status') . '"></span>';
                $row[$key_row] = l($icon_alert_no_status, 'mica/variable/' . $harmonized_variable_id,
                  array(
                    'html' => TRUE,
                    'query' => array(
                      'title' => $variable_title
                    )
                  ));
              }
              elseif ($status == 'complete') {
                $icon_success_complete = '<span data-toggle="tooltip" class="glyphicon glyphicon-ok alert-success" data-original-title="' . t('Complete') . '"></span>';
                $row[$key_row] = l($icon_success_complete, 'mica/variable/' . $harmonized_variable_id,
                  array(
                    'html' => TRUE,
                    'query' => array(
                      'title' => $variable_title
                    )
                  ));
              }
              elseif ($status == 'impossible') {

                $icon_danger_impossible = '<span data-toggle="tooltip" class="glyphicon ' .
                  ObibaDatasetConstants::getIcon()
                  . '" data-original-title="' . t(variable_get('dataset_harmonization_impossible_label')) . '"></span>';
                $row[$key_row] = l($icon_danger_impossible, 'mica/variable/' . $harmonized_variable_id,
                  array(
                    'html' => TRUE,
                    'query' => array(
                      'title' => $variable_title
                    )
                  ));
              }
              elseif ($status == 'undetermined') {
                $icon_warning_undetermined = '<span data-toggle="tooltip" class="glyphicon glyphicon-question-sign alert-warning" data-original-title="' . t('Undetermined') . '"></span>';
                $row[$key_row] = l($icon_warning_undetermined, 'mica/variable/' . $harmonized_variable_id,
                  array(
                    'html' => TRUE,
                    'query' => array(
                      'title' => $variable_title
                    )
                  ));
              }
              else {
                $no_value = '-';
                $row[$key_row] = l($no_value, 'mica/variable/' . $harmonized_variable_id);
              }
              $found = TRUE;
              break;
            }
          }
          if (!$found) {
            $row[$key_row] = '<span data-toggle="tooltip"  class="glyphicon glyphicon-question-sign alert-warning" data-original-title="' . t('No status') . '"></span>';
          }
        }

        $rows[] = $row;
      }
    }
  }

  return $rows;
}

/*End harmonization variable ajax table */

function obiba_mica_dataset_coverage($dataset_id) {
  $coverage['charts'] = obiba_mica_search_query_charts(
    MicaClient::addParameterDtoQueryLink(
      array('variables' => array('terms' => array('datasetId' => $dataset_id)))));
  if (!empty($coverage['charts'])) {
    $coverage['has_coverage'] = TRUE;
    return $coverage;
  }
  else {
    return FALSE;
  }
}

/**
 * @param $dataset_type_dto
 */
function obiba_mica_dataset_studies_datatable_init($dataset_type_dto) {
  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  }
  else {
    obiba_mica_commons_datatables_external_source();
  }

  $study_tables = array();
  if (!empty($dataset_type_dto->studyTables)) {
    foreach ($dataset_type_dto->studyTables as $study_table) {
      array_push($study_tables, $study_table);
    }
  }

  if (!empty($dataset_type_dto->studyTable)) {
    array_push($study_tables, $dataset_type_dto->studyTable);
  }

  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_harmo_studies_datatable_init.js', 'file');
  $data = array(
    'table_headers' => obiba_mica_study_study_summaries_table_headers(),
    'table_data' => obiba_mica_dataset_study_tables_rows($study_tables)
  );

  drupal_add_js($data, 'setting');
}

function obiba_mica_dataset_study_tables_table($dataset_type_dto) {
  $study_tables = array();
  if (!empty($dataset_type_dto->studyTables)) {
    foreach ($dataset_type_dto->studyTables as $study_table) {
      array_push($study_tables, $study_table);
    }
  }

  if (!empty($dataset_type_dto->studyTable)) {
    array_push($study_tables, $dataset_type_dto->studyTable);
  }

  $rows = obiba_mica_dataset_study_tables_rows($study_tables);
  $headers = obiba_mica_study_study_summaries_table_headers();

  return theme(
    'table',
    array(
      'header' => $headers,
      'rows' => $rows,
      'empty' => t(variable_get_value('studies_empty_label')),
      'sticky' => FALSE
    )
  );
}

function obiba_mica_dataset_study_tables_rows($study_tables) {
  $rows = array();
  if (!empty($study_tables)) {
    $study_ids = array();
    foreach ($study_tables as $study_table) {
      if (!in_array($study_table->studyId, $study_ids)) {
        $study_ids[] = $study_table->studyId;
        $row = obiba_mica_study_study_summaries_table_row($study_table->studySummary);
        $rows[] = $row;
      }
    }
  }
  return $rows;
}

/**
 * @param $dataset_id
 * @return array
 */
function obiba_mica_dataset_page_detail_download($dataset_id) {
  global $language;
  $dataset_resource = new MicaDatasetResource();
  $response = $dataset_resource->download_variables_harmonizations($dataset_id, $language->language);
  if (!empty($response)) {
    $headers = $dataset_resource->getLastResponseHeaders();
    drupal_add_http_header('Content-Type', $headers['Content-Type'][0] . '; utf-8');
    drupal_add_http_header('Content-Disposition', $headers['Content-Disposition'][0]);
    ob_clean();
    $copyright_notice = variable_get_value('mica_copyright_notice');
    $csv_response = !empty($copyright_notice) ? '"' . t($copyright_notice) . '"' . chr(13) : '';
    $csv_response .= $response->body;
    $csv_altered = module_invoke_all('harmonization_csv_alter', $csv_response);
    print empty($csv_altered) ? $csv_response : $csv_altered[0];
    exit;
  }
}

/**
 * Get the facets of the variables for this dataset.
 * @param $dataset_id
 * @return array
 */
function obiba_mica_dataset_get_variables($dataset_id) {
  $dataset_resource = new MicaDatasetResource();
  return $dataset_resource->variables_search(array(
    0 => array(
      'field' => 'datasetId',
      'terms' => array($dataset_id)
    )
  ), 0);
}

function obiba_mica_dataset_variables_search($dataset_type, $dataset_id, $query = NULL, $nature = NULL) {
  $dataset_resource = new MicaDatasetResource();
  $nature_query = empty($nature) ? '(nature:categorical OR nature:continuous)' : 'nature:' . $nature;
  $query_string = empty($query) ? NULL : $query . ' AND ' . $nature_query;

  $variables = $dataset_resource->get_variables($dataset_type, $dataset_id, 0, 10, NULL, NULL, $query_string);
  drupal_json_output($variables);
}

function obiba_mica_dataset_variables_crosstab($dataset_type, $dataset_id, $variable1, $variable2) {
  $dataset_resource = new MicaDatasetResource();

  $variables = $dataset_resource->cross_variables($dataset_type, $dataset_id, $variable1, $variable2);
  drupal_json_output($variables);
}

function obiba_mica_dataset_variables_crosstab_download_csv($dataset_type, $dataset_id, $variable1, $variable2) {
  _obiba_mica_dataset_variables_crosstab_download('csv', $dataset_type, $dataset_id, $variable1, $variable2);
}

function obiba_mica_dataset_variables_crosstab_download_excel($dataset_type, $dataset_id, $variable1, $variable2) {
  _obiba_mica_dataset_variables_crosstab_download('excel', $dataset_type, $dataset_id, $variable1, $variable2);
}

function _obiba_mica_dataset_variables_crosstab_download($doc_type, $dataset_type, $dataset_id, $variable1, $variable2) {
  $dataset_resource = new MicaDatasetResource();

  $response = $dataset_resource->download_cross_variables($dataset_type, $dataset_id, $variable1, $variable2, $doc_type);

  if (!empty($response)) {
    $headers = $dataset_resource->getLastResponseHeaders();
    drupal_add_http_header('Content-Type', $headers['Content-Type'][0] . '; utf-8');
    drupal_add_http_header('Content-Disposition', $headers['Content-Disposition'][0]);
    ob_clean();
    $csv_response = $response->body;
    print $csv_response;
    exit;
  }
}

function obiba_mica_dataset_variable_info($variable_id) {
  $dataset_resource = new MicaDatasetResource();
  $variable_dto = $dataset_resource->get_variable($variable_id);
  drupal_json_output($variable_dto);
}

function obiba_mica_dataset_info($dataset_type, $dataset_id) {
  $dataset_resource = new MicaDatasetResource();
  $dataset_dto = $dataset_resource->get_dataset($dataset_type, $dataset_id);
  drupal_json_output($dataset_dto);
}