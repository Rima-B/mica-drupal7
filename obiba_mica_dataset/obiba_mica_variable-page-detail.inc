<?php

/*
 * @file code for variable details page
 * */

include_once('includes/obiba_mica_dataset_resource.inc');
include_once('includes/obiba_mica_variable_statistics.inc');

function obiba_mica_variable_page_detail($variable_id) {
  $library_chart = obiba_mica_variable_load_chart_libraries();
  drupal_add_js(
    drupal_get_path('module', 'charts') .
    '/modules/charts_' . $library_chart .
    '/charts_' . $library_chart . '.js', 'file');
  if ($library_chart == 'google') {
    drupal_add_js('https://www.google.com/jsapi', 'file');
  }

  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_variable_harmo_algo_datatable_init.js', 'file');
  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_get_ajax_variables.js', 'file');
  drupal_add_js(array('obiba_mica_variable' => array('library' => $library_chart)), 'setting');

  $url_param = explode('?', request_uri());
  $url_query = NULL;
  if (!empty($url_param[1])) {
    parse_str($url_param[1], $url_query);
  }

  $variable_resource = new MicaDatasetResource();
  $variable = $variable_resource->get_variable($variable_id);
  if (empty($variable)) {
    drupal_set_title(t('Variable not found'));
    return '';
  }
  if ($variable->variableType == "Dataschema") {
    // $algos = obiba_mica_dataset_get_variable_algorithm($variable_id);
    $algos = TRUE;
  }
  $variable->label = _obiba_mica_variable_attributes_detail($variable, 'label');
  $variable->description = _obiba_mica_variable_attributes_detail($variable, 'description');
  $variable->comment = _obiba_mica_variable_attributes_detail($variable, 'comment');

  $title = $variable->name;
  if ($variable->variableType == 'Harmonized') {
    if (!empty($url_query['title'])) {
      $title = $url_query['title'];
    }
    else {
      $title .= ' -- ' . (obiba_mica_commons_get_localized_field($variable->studySummaries[0], 'acronym'));
    }
  }

  drupal_set_title($title);

  $dataset_type = $variable->variableType == 'Study' ? 'study-dataset' : 'harmonization-dataset';

  drupal_set_breadcrumb(array(
    l(t('Home'), '<front>'),
    l(t('Datasets'), 'mica/datasets'),
    l(($dataset_type == 'study-dataset' ? t('Study Datasets') : t('Harmonization Datasets')), 'mica/datasets/' . $dataset_type . 's'),
    l(obiba_mica_commons_get_localized_field($variable, 'datasetName'), 'mica/' . obiba_mica_variable_dataset_type($variable)
      . '/' . $variable->datasetId)
  ));

  _obiba_mica_dataset_remove_harmonization_taxonomy($variable);
  return theme('obiba_mica_variable-detail', array(
    'harmonization_table_legend' => theme('obiba_mica_dataset-harmonization-table-legend', array('clickable' => $variable->variableType != 'Harmonized')),
    'variable_dto' => $variable,
    'variable_harmonization_algorithms' => !empty($algos) ? $algos : NULL,
    'variable_harmonization' => obiba_mica_variable_get_harmonization($variable),
  ));
}

function obiba_mica_dataset_get_variable_algorithm($variable_id, $sorted_variable_names) {
  $sorted_variables = json_decode($sorted_variable_names, true);
  $variable_resource = new MicaDatasetResource();
  $harmonization_variables = $variable_resource->get_variable_harmonizations($variable_id);
  foreach ($harmonization_variables->datasetVariableSummaries as $harmonization_variable) {
    $variable_resolver = $harmonization_variable->resolver;
    $study_id = $variable_resolver->studyId;

    $variable_title = $variable_resolver->name . ' -- ' . $study_id;
    if (!empty($harmonization_variable->studyTable->name)) {
      $variable_title = $variable_title . ' (' . obiba_mica_commons_get_localized_field($harmonization_variable->studyTable, 'name') . ')';
    }
    $algorithm = _obiba_mica_variable_attributes_detail($harmonization_variable, 'algorithm', array(
      'maelstrom',
      'Mlstr_harmo'
    ));
    if (empty($algorithm)) {
      // no need to render
      unset($sorted_variables[$variable_title]);
    } else {
      if (array_key_exists($variable_title, $sorted_variables)) {
        $sorted_variables[$variable_title] = array(
          'var_id' => $variable_resolver->id,
          'var_detail' => obiba_mica_commons_markdown($algorithm)
        );
      }
    }
  }

  $themed_algo['algo'] = theme(
    'obiba_mica_variable_harmonization_algorithm',
    array('variable_harmonization_algorithms' => $sorted_variables)
  );

  drupal_json_output($themed_algo);
}

function _obiba_mica_dataset_remove_harmonization_taxonomy($variable) {

  if (!empty($variable->termAttributes)) {
    $variable->termAttributes = array_filter($variable->termAttributes, function ($term_attribute) {
      return $term_attribute->taxonomy->name !== 'Mlstr_harmo';
    });
  }

}

function obiba_mica_variable_load_chart_libraries() {
  if (module_exists('charts')) {
    $chart_param = variable_get('charts_default_settings');
    if (!empty($chart_param)) {
      $charts_lib = libraries_load($chart_param['library']);
      if (!empty($charts_lib)) {
        foreach ($charts_lib as $folder) {
          if (!empty($folder['js'])) {
            foreach ($folder as $key_file => $param_files) {
              if ($key_file == 'js') {
                drupal_add_js($charts_lib['library path'] . '/' . key($param_files));
              }
            }
          }
        }
        return $chart_param['library'];
      }
    }
    return 'google';
  }
  return 'google';
}

function obiba_mica_variable_get_harmonizations($variable) {
  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  } else {
    obiba_mica_commons_datatables_external_source();
  }

  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_variable_harmo_datatable_init.js', 'file');
  drupal_add_js(array(
    'table_headers' => obiba_mica_variable_get_harmonization_table_headers(),
    'table_data' => obiba_mica_variable_get_harmonization_table_rows($variable)
  ), 'setting');
}

function obiba_mica_variable_get_harmonization_table_headers() {
  return array(
    array("title" => t('Harmonized Variable'), 'width' => '20%'),
    array("title" => t('Study')),
    array("title" => t('Data Collection Event'), 'width' => '40%'),
    array("title" => t('Status'), 'width' => '15%'),
    array("title" =>t('Comment'))
  );
}

function obiba_mica_variable_get_harmonization_table_rows($variable) {
  $rows = array();
  $variable_resource = new MicaDatasetResource();

  $variable_harmonizations = $variable_resource->get_variable_harmonizations($variable->id);

  if (!empty($variable_harmonizations->datasetVariableSummaries)) {
    foreach ($variable_harmonizations->datasetVariableSummaries as $variable_harmonization) {
      $variable_resolver = $variable_harmonization->resolver;
      $harmonized_variable_id = $variable_resolver->id;
      $study_id = $variable_resolver->studyId;
      $study_summary = _obiba_mica_variable_study_summary($variable, $study_id);
      $study_acronym = obiba_mica_commons_get_localized_field($study_summary, 'acronym');
      $variable_title = $variable->name . ' -- ' . $study_acronym;
      $variable_title_key = $variable->name . ' -- ' . $study_id;
      if (!empty($variable_harmonization->studyTable->name)) {
        $variable_title = $variable_title . ' (' . obiba_mica_commons_get_localized_field($variable_harmonization->studyTable, 'name') . ')';
        $variable_title_key .= ' (' . obiba_mica_commons_get_localized_field($variable_harmonization->studyTable, 'name') . ')';
      }

      $status = trim(strtolower(_obiba_mica_variable_attributes_detail($variable_harmonization, 'status', array(
        'maelstrom',
        'Mlstr_harmo'
      ))));

      if (empty($status)) {
        $status = '<span class="glyphicon glyphicon-question-sign alert-warning" title="' . t('No status') . '"></span>';
      }
      elseif ($status == 'complete') {
        $status = '<span class="glyphicon glyphicon-ok alert-success" title="' . t('Complete') . '"></span>';
      }
      elseif ($status == 'impossible') {
        $status = '<span class="glyphicon ' . ObibaDatasetConstants::getIcon() . '" title="' . t(variable_get('dataset_harmonization_impossible_label')) . '"></span>';
      }
      elseif ($status == 'undetermined') {
        $status = '<span class="glyphicon glyphicon-question-sign alert-warning" title="' . t('Undetermined') . '"></span>';
      }
      else {
        $status = '<span class="glyphicon glyphicon-question-sign alert-warningr" title="' . t('No status') . '"></span>';
      }

      $rows[] = array(
        '<p>' .
        l($variable_title, 'mica/variable/' . $harmonized_variable_id, array(
          'query' => array(
            'title' => $variable_title
          )
        )) . '</p><p class="help-block">' . obiba_mica_commons_get_localized_field($variable_harmonization->studyTable, 'description') . '</p>',
        l($study_acronym, 'mica/study/' . $study_id),
        obiba_mica_dataset_get_dce_name_from_dataset($variable_harmonization->studyTable, $study_summary),
        //$variable_harmonization->studyTable->populationId . ':' . $variable_harmonization->studyTable->dataCollectionEventId,
        l($status, 'mica/variable/' . $harmonized_variable_id, array(
          'html' => TRUE,
          'query' => array(
            'title' => $variable_title
          )
        )),
        _obiba_mica_variable_html_truncated(
          _obiba_mica_variable_attributes_detail($variable_harmonization, 'comment', array(
            'maelstrom',
            'Mlstr_harmo'
          )), t('comment'), 200, FALSE),
        $variable_title_key
      );

    }
  }

  return $rows;
}

function obiba_mica_variable_get_harmonization($harmonized_variable) {
  return array(
    'status' => _obiba_mica_variable_attributes_detail($harmonized_variable, 'status', array(
      'maelstrom',
      'Mlstr_harmo'
    )),
    'comment' => _obiba_mica_variable_attributes_detail($harmonized_variable, 'comment', array(
      'maelstrom',
      'Mlstr_harmo'
    )),
    'algorithm' => obiba_mica_commons_markdown(_obiba_mica_variable_attributes_detail($harmonized_variable, 'algorithm', array(
      'maelstrom',
      'Mlstr_harmo'
    ))),
    'script' => _obiba_mica_variable_attributes_detail($harmonized_variable, 'script'),
  );
}

function _obiba_mica_variable_html_truncated($text, $label, $size = 200, $is_code = FALSE, $pre_class = '') {
  if (!empty($text)) {
    $truncate = strlen($text) > $size;
    $html_truncate = $truncate //
      ? check_markup(truncate_utf8($text, $size, FALSE)) . "[" . $label . " " . t("truncated") . "]" //
      : check_markup($text); //
  }
  else {
    $html_truncate = '-';
  }
  return $html_truncate;
}

function obiba_mica_variable_get_categories($categories = NULL) {
  $rows = array();
  if (!empty($categories)) {
    foreach ($categories as $category) {
      $rows[] = array(
        $category->name,
        _obiba_mica_variable_attributes_detail($category, 'label'),
        $category->missing ? '<span class="glyphicon glyphicon-ok"></span>' : NULL
      );
    }
    return theme(
      'table',
      array(
        'header' => array(t('Name'), t('Label'), t('Missing')),
        'rows' => $rows,
        'empty' => t('No category found'),
        'sticky' => FALSE,
      )
    );
  }

  return FALSE;
}

function obiba_mica_dataset_variable_get_ajax_statistics($variable_id = NULL) {
  if (!variable_get_value('mica_statistics')) return;
  $stat_tab = FALSE;
  $variable_resource = new MicaDatasetResource();
  $variable = $variable_resource->get_variable($variable_id);
  $variable_stat = $variable_resource->get_variable_aggregation($variable_id);
  if (!empty($variable_stat)) {
    $statistics = new VariableStatistics($variable, $variable_stat);
    $stat_tab['table'] = $statistics->as_table();
    $stat_tab['chart'] = $statistics->as_chart();
    return drupal_json_output($stat_tab);
  }
  return FALSE;
}

function obiba_mica_dataset_variable_get_ajax_statistics_test($variable_id = NULL) {
  if (!variable_get_value('mica_statistics')) return;
  $stat_tab = '';
  $variable_resource = new MicaDatasetResource();
  $variable = $variable_resource->get_variable($variable_id);
  $variable_stat = $variable_resource->get_variable_aggregation($variable_id);
//  dpm($variable);
//  dpm($variable_stat);
  if (!empty($variable_stat)) {
    $statistics = new VariableStatistics($variable, $variable_stat);
    $stat_tab .= $statistics->as_table();
    $stat_tab .= $statistics->as_chart();
  }

  return $stat_tab;
}

/**
 * Get the study summary found in the variable. If not found, fetch the study summary directly.
 * @param $variable
 * @param $study_id
 * @return mixed
 */
function _obiba_mica_variable_study_summary($variable, $study_id) {
  foreach ($variable->studySummaries as $study) {
    if ($study->id == $study_id) {
      return $study;
    }
  }
  // not found in the dto, then fetch it
  $study_resource = new MicaStudyResource();
  return $study_resource->study($study_id);
}

/**
 * Get the study acronym from the study summary found in the variable. If not found, fetch the
 * study summary directly.
 * @param $variable
 * @param $study_id
 * @return mixed
 */
function _obiba_mica_variable_study_acronym($variable, $study_id) {
  return obiba_mica_commons_get_localized_field(_obiba_mica_variable_study_summary($variable, $study_id), 'acronym');
}

function obiba_mica_variable_dataset_type($variable) {
  return $variable->variableType == 'Study' ? 'study-dataset' : 'harmonization-dataset';
}