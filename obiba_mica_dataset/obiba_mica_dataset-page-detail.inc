<?php
/**
 * Copyright (c) 2016 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Code for dataset detail page
 */

include_once 'includes/obiba_mica_dataset_resource.inc';

use Obiba\ObibaMicaClient\MicaClient\DrupalMicaClient as DrupalMicaClient;

/**
 * The dataset Detail page.
 */
function obiba_mica_dataset_page_detail($dataset_id, $token_key = NULL) {

  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  }
  else {
    obiba_mica_commons_datatables_external_source();
  }
  drupal_add_js(drupal_get_path('module', 'obiba_mica_commons') . '/js/ajax_getlist_items.js', 'file');
  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_datatable_init.js', 'file');
  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_toggle_harmo_table.js', 'file');
  drupal_add_js(array('datasetId' => $dataset_id), 'setting');

  $dataset_dto = NULL;
  $dataset_type_dto = NULL;
  $variables_dataset = NULL;
  $variables_table_output = NULL;
  $themed_attachment = NULL;
  $modals = NULL;
  $dataset_resource = new DrupalMicaDatasetResource();
  $localize = obiba_mica_commons_mica_server_localisation();

  $dataset_type = arg(1);

  if (!empty($token_key)) {
    obiba_mica_commons_is_published(FALSE);
    $dataset_dto = $dataset_resource->getDataset($dataset_id, $dataset_type,  $token_key);
  }
  else {
    $dataset_dto = $dataset_resource->getDataset($dataset_id);
  }

  if (!empty($dataset_dto)) {
    if ($dataset_type == DrupalMicaDatasetResource::COLLECTED_DATASET && !empty($dataset_dto->{'obiba.mica.CollectedDatasetDto.type'})) {
      $dataset_type_dto = $dataset_dto->{'obiba.mica.CollectedDatasetDto.type'};
    }
    elseif(!empty($dataset_dto->{'obiba.mica.HarmonizedDatasetDto.type'})) {
      $dataset_type_dto = $dataset_dto->{'obiba.mica.HarmonizedDatasetDto.type'};
    }

  }

  obiba_mica_app_angular_load_js_resources('obiba_mica_graphic');

  $acronym = obiba_mica_commons_get_localized_field($dataset_dto, 'acronym');
  $name = obiba_mica_commons_get_localized_field($dataset_dto, 'name');
  $page_title = (variable_get_value('dataset_title_acronym_setup') ? ($acronym . ' - ') : '') . $name;
  drupal_set_title($page_title);

  drupal_set_breadcrumb(array(
    l(t('Home'), '<front>'),
    l($localize->getTranslation('datasets'), DrupalMicaDatasetResource::DATASETS),
    l(($dataset_type == DrupalMicaDatasetResource::COLLECTED_DATASET ? $localize->getTranslation('collected-datasets') : $localize->getTranslation('harmonized-datasets')), DrupalMicaDatasetResource::DATASETS . '/' .  $dataset_type ),
  ));

  $dataset_theme = obiba_mica_commons_get_cache(__FUNCTION__ . 'dataset_theme-' . $dataset_id);
  if (!empty($dataset_theme)) {
    return $dataset_theme;
  }
  else {
    if (!empty($dataset_dto)) {
      $variables_dataset = $dataset_resource->getVariables($dataset_type, $dataset_id, 0, 1);
      if ($dataset_type == DrupalMicaDatasetResource::COLLECTED_DATASET && !empty($dataset_type_dto->studyTable->studySummary->published)) {
        $study_id = !empty($dataset_type_dto) ? $dataset_type_dto->studyTable->studyId : NULL;
        $modals = obiba_mica_dataset_get_population_dce_modals(
          $study_id,
          obiba_mica_dataset_get_population_dce($dataset_type_dto),
          $variables_dataset,
          NULL
        );
      }

      if (module_exists('obiba_mica_files')) {
        drupal_add_js(drupal_get_path('module', 'obiba_mica_files') .
          '/js/obiba_mica_files_attachment_download.js');
        $path_attachment = '/' . $dataset_type . '/' . $dataset_id;
        $has_files  = obiba_mica_files_get_flat_attachments($path_attachment, !empty($token_key)?$token_key:NULL);
          if(!empty($has_files)){
            $file_browser = theme('obiba_mica_files_browser', array(
              'doc_path' => '/' . $dataset_type,
              'doc_id' => $dataset_id,
              'token_key' => !empty($token_key) ? $token_key : NULL
            ));
          }
        obiba_mica_app_angular_load_js_resources('obiba_mica_files');
      }
      $permissionResource = new DrupalMicaClient\DrupalMicaClientPermission();
      $can_edit_draf_document = $permissionResource->canEditDraftDocument($dataset_dto);
      $dataset_theme = '';
      if (!empty($token_key)) {
        $dataset_theme = obiba_mica_commons_get_draft_link($dataset_type, $dataset_id, $dataset_dto->published);
      }
      $dataset_theme .= theme('obiba_mica_dataset-detail', array(
        'draft_view' => !empty($token_key) ? TRUE : FALSE,
        'can_edit_draf_document' => $can_edit_draf_document ? $can_edit_draf_document : FALSE,
        'dataset_dto' => $dataset_dto,
        'dataset_type_dto' => $dataset_type_dto,
        'dataset_type' => $dataset_type,
        'harmonization_table_legend' => theme('obiba_mica_dataset-harmonization-table-legend', array('clickable' => TRUE)),
        'show_harmonization_variables_table' =>
          _obiba_mica_dataset_show_harmonization_variables_table($dataset_type_dto, $dataset_type),
        'variables_dataset' => $variables_dataset,
        'modals' => $modals,
        'file_browser' => !empty($file_browser) ? $file_browser : NULL,
        'study_tables' => obiba_mica_dataset_study_tables($dataset_type_dto)
      ));
      obiba_mica_commons_set_cache(__FUNCTION__ . 'dataset_theme-' . $dataset_id, $dataset_theme);
      return $dataset_theme;
    }
    else {
      drupal_set_title(t('Dataset not found'));
      return '';
    }

  }
}

function _obiba_mica_dataset_show_harmonization_variables_table($dataset_type_dto, $dataset_type) {
  return NULL != $dataset_type_dto
    && (!empty($dataset_type_dto->studyTables) || !empty($dataset_type_dto->harmonizationTables))
    && $dataset_type != 'collected-dataset'
    && (!isset($draft_view) || $draft_view !== TRUE);
}

/**
 * Get population DCE.
 */
function obiba_mica_dataset_get_population_dce($dataset_type_dto = NULL) {
  $data = array('study' => NULL, 'population' => NULL, 'dce' => NULL);
  $study_table = !empty($dataset_type_dto->studyTable)?$dataset_type_dto->studyTable:NULl;
  if(!empty($study_table)){
    $study_resource = new DrupalMicaStudyResource();
    $study_dto = $study_resource->getStudy($study_table->studyId);
  }

  if (!empty($study_dto)) {
    $data['study'] = $study_dto;
    $population = array_filter($study_dto->populations, function ($population) use ($study_table) {
      return $study_table->populationId === $population->id;
    });

    if (!empty($population)) {
      $data['population'] = $population;
      $population = array_pop($population);
      if (!empty($population->dataCollectionEvents)) {
        $data['dce'] = array_filter($population->dataCollectionEvents, function ($dce) use ($study_table) {
          return $study_table->dataCollectionEventId === $dce->id;
        });
      }
    }
  }

  return $data;
}

/**
 * Get a dce population modal.
 */
function obiba_mica_dataset_get_population_dce_modals($study_id, $data, $variables_dataset = NULL, $has_coverage = NULL) {
  $modals = array();
  $themed_population_attachment = NULL;
  $populations = $data['population'];
  if (!empty($populations)) {
    foreach ($populations as $population) {
      if (module_exists('obiba_mica_files')) {
        drupal_add_js(drupal_get_path('module', 'obiba_mica_files') .
          '/js/obiba_mica_files_attachment_download.js');
        $path_population_attachment = "/study/$study_id/population/$population->id";
        $has_files  = obiba_mica_files_get_flat_attachments($path_population_attachment);
        if(!empty($has_files)){
            $file_browser = theme('obiba_mica_files_browser', array(
              'doc_path' => "/study/$study_id/population",
              'doc_id' => $population->id));
          }
        }

      foreach ($population->dataCollectionEvents as $dce) {
        $dce_uids[] = $study_id . ':' . $population->id . ':' . $dce->id;
      }

      $modals['population'][] = theme('mica_population_content_detail_modal', array(
        'population' => $population,
        'population_content' => theme('mica_population_content_detail', array(
          'dce_uids' => $dce_uids,
          'population' => $population,
          'hide_title' => TRUE,
          'file_browser' => !empty($file_browser) ? $file_browser : NULL
        )),
      ));

      $dces = $data['dce'];
      $themed_attachment = NULL;
      $population_id = $population->id;
      $dce_variables_nbr = array();
      $modals['dce'] = '';
      foreach ($dces as $dce) {
        if (module_exists('obiba_mica_files')) {
          drupal_add_js(drupal_get_path('module', 'obiba_mica_files') .
            '/js/obiba_mica_files_attachment_download.js');
          $path_dce_attachment = "/study/$study_id/population/$population_id/data-collection-event/$dce->id";
          $has_files  = obiba_mica_files_get_flat_attachments($path_dce_attachment);
          if(!empty($has_files)){
            $file_browser = theme('obiba_mica_files_browser', array(
              'doc_path' => "/study/$study_id/population/$population_id" . '/data-collection-event',
              'doc_id' => $dce->id));
          }
        }
        $dce_variables_nbr[obiba_mica_commons_get_localized_field($dce, 'name')] = (!empty($variables_dataset->total)) ? $variables_dataset->total : '-';

        $modals['dce'] .= theme('mica_dce_detail', array(
          'has_coverage' => $has_coverage,
          'dce_variables_nbr' => $dce_variables_nbr,
          'study_id' => $study_id,
          'population_id' => $population_id,
          'dce_uid' => $study_id . ':' . $population_id . ':' . $dce->id,
          'dce_id_target' => MicaClientPathHelpers::normalizeModalTarget($study_id . '-' . $population_id . '-' . $dce->id),
          'dce' => $dce,
          'file_browser' => !empty($file_browser) ? $file_browser : NULL,
        ));
      }
      if (!empty($modals['dce'])) {
        drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_details.js', 'file');
      }
    }
  }

  return $modals;
}

/**
 * Ajax Variable table Call header.
 */
function obiba_mica_dataset_Variables_table_ajax_header($dataset_type, $dataset_id) {
  $cached_header_variable_table = obiba_mica_commons_get_cache(__FUNCTION__ . 'variable-table-header-cache-' . $dataset_type . '-' . $dataset_id);

  if (!empty($cached_header_variable_table)) {
    $data = $cached_header_variable_table;
  }
  else {
    $dataset_resource = new DrupalMicaDatasetResource();
    $dataset_dto = $dataset_resource->getDataset($dataset_id);
    if ($dataset_type == DrupalMicaDatasetResource::COLLECTED_DATASET) {
      $header = obiba_mica_dataset_studies_Variables_table_ajax_header();
    }
    else {
      $dataset_type_dto = $dataset_dto->{'obiba.mica.HarmonizedDatasetDto.type'};
      $header = obiba_mica_dataset_harmonizations_Variables_table_ajax_header($dataset_type_dto);
    }

    $data['header'] = $header;
    obiba_mica_commons_set_cache(__FUNCTION__ . 'variable-table-header-cache-' . $dataset_type . '-' . $dataset_id, $data);
  }

  return drupal_json_output($data);
}

/**
 * Get Sort by from url param.
 */
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

/**
 * Ajax Variable table Call data.
 */
function obiba_mica_dataset_Variables_table_ajax_data($dataset_type, $id_dataset) {
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

  $variable_data = obiba_mica_commons_get_cache(__FUNCTION__ . 'Variable-dataset-' . $dataset_type . '-' . $id_dataset . '-' . $from . '-' . $size);
  if (!empty($variable_data)) {
    $data = $variable_data;
  }
  else {
    $dataset_resource = new DrupalMicaDatasetResource();
    $sort = obiba_mica_dataset_get_sorted_by_column();

    $dataset_dto = $dataset_resource->getDataset($id_dataset);

    if ($dataset_type == DrupalMicaDatasetResource::COLLECTED_DATASET) {
      $variables_dataset = $dataset_resource->getVariables($dataset_type,
        $id_dataset,
        $from,
        $size,
        !empty($sort['sort']) ? $sort['sort'] : 'index',
        !empty($sort['order']) ? $sort['order'] : NULL
      );

      $data_variable = obiba_mica_dataset_studies_table_data($variables_dataset);

    }
    else {
      $dataset_type_dto = $dataset_dto->{'obiba.mica.HarmonizedDatasetDto.type'};
      $variables_dataset = $dataset_resource->getVariablesHarmonizations($dataset_type,
        $id_dataset,
        $from,
        $size,
        !empty($sort['sort']) ? $sort['sort'] : 'index',
        !empty($sort['order']) ? $sort['order'] : NULL
      );
      $data_variable = obiba_mica_dataset_harmonizations_table_data($dataset_type_dto, $variables_dataset);
    }
    $data['iTotalRecords'] = $variables_dataset->total;
    $data['iTotalDisplayRecords'] = $variables_dataset->total;
    $data['aaData'] = $data_variable;
    obiba_mica_commons_set_cache(__FUNCTION__ . 'Variable-dataset-' . $dataset_type . '-' . $id_dataset . '-' . $from . '-' . $size, $data);
  }
  // Must be update in each request (DataTable requirement in ajx call context)
  $data['sEcho'] = !empty($_GET["sEcho"]) ? $_GET["sEcho"] : 1;
  return drupal_json_output($data);
}

/**
 * Studies dataset ajax variable table.
 */
function obiba_mica_dataset_studies_Variables_table_ajax_header() {
  $localize = obiba_mica_commons_mica_server_localisation();
  return array(
    array(
      "sTitle" => '',
      "data" => "index",
      "mDataProp" => "index",
      "name" => "index",
      "visible" => FALSE,
    ),
    array(
      "sTitle" => $localize->getTranslation('search.variable.facet-label'),
      "data" => "name",
      "mDataProp" => "name",
      "name" => "name",
      "width" => "25%",
    ),
    array(
      "sTitle" => $localize->getTranslation('search.variable.label'),
      "data" => "label",
      "mDataProp" => "label",
      "name" => "label",
      "orderable" => FALSE,
    ),
    array(
      "sTitle" => $localize->getTranslation('classifications.title'),
      "data" => "classifications",
      "mDataProp" => "classifications",
      "name" => "classifications",
      "orderable" => FALSE,
    ),
  );
}

/**
 * The study data in a table.
 */
function obiba_mica_dataset_studies_table_data($variables_dataset) {
  $rows = array();
  if (!empty($variables_dataset->variables)) {
    foreach ($variables_dataset->variables as $variable) {
      $var_row = l($variable->name, 'mica/variable/' . $variable->id);
      $domains = obiba_mica_dataset_variable_domains_list($variable);
      $rows[] = array(
        "index" => '',
        "name" => $var_row,
        "label" => obiba_mica_dataset_variable_attributes_detail($variable, 'label'),
        "classifications" => $domains,
      );
    }
  }
  return $rows;
}

/*********End Studies ajax header *************/

/**
 * Harmonization ajax variable table.
 */
function obiba_mica_dataset_harmonizations_Variables_table_ajax_header($dataset_type_dto) {
  $nbr_studies = count($dataset_type_dto->studyTables);
  $witdh_column_study = (100 - (1 + 25)) / $nbr_studies;
  $opal_tables = _obiba_mica_dataset_harmonizations_get_all_opal_tables($dataset_type_dto);

  if (!empty($opal_tables)) {
    $localize = obiba_mica_commons_mica_server_localisation();
    $header[] = array(
      "sTitle" => '',
      "data" => "index",
      "name" => "index",
      "mDataProp" => "index",
      "visible" => FALSE,
    );

    $header[] = array(
      "sTitle" => $localize->getTranslation('search.variable.facet-label'),
      "data" => "variable",
      "name" => "name",
      "mDataProp" => "name",
      "width" => "25%",
      "class" => "study-title",
    );

    foreach ($opal_tables as $opal_table) {
      $entity_summary = $opal_table->studySummary;
      $entity_url = 'mica/' . $entity_summary->studyResourcePath . '/' . $entity_summary->id;

      $acronym = obiba_mica_commons_get_localized_field($entity_summary, 'acronym');
      $title_header = !empty($entity_summary->published) ? l($acronym, $entity_url) : $acronym;
      $title_header = str_replace('"', '\'', $title_header);
      if (!empty($opal_table->name)) {
        $name = ' <small title="' . obiba_mica_commons_get_localized_field($opal_table, 'description') . '">' . obiba_mica_commons_get_localized_field($opal_table, 'name') . '</small>';
        $name = str_replace('"', '\'', $name);
        $header[] = array(
          "data" => str_replace(' ', '_', $acronym . obiba_mica_commons_get_localized_field($opal_table, 'name')),
          "sTitle" => $title_header . $name,
          "name" => str_replace(' ', '_', $acronym . obiba_mica_commons_get_localized_field($opal_table, 'name')),
          "orderable" => FALSE,
          "width" => $witdh_column_study . "%",
          "class" => "text-center study-title",
        );
      } else {
        $header[] = array(
          "data" => str_replace(' ', '_', $acronym),
          "sTitle" => $title_header,
          "name" => $acronym,
          "orderable" => FALSE,
          "width" => $witdh_column_study . "%",
          "class" => "text-center study-title",
        );
      }
    }
  }

  return $header;
}

/**
 * @param $dataset_type_dto
 * @return array
 */
function _obiba_mica_dataset_harmonizations_get_all_opal_tables($dataset_type_dto) {
  $opal_tables = array_merge(@$dataset_type_dto->studyTables ?: array(), @$dataset_type_dto->harmonizationTables ?: array());

  usort($opal_tables, function ($a, $b) {
    return $a->weight - $b->weight;
  });

  return $opal_tables;
}

/**
 * Get harmonization dataset table.
 */
function obiba_mica_dataset_harmonizations_table_data($dataset_type_dto, $dataset_harmonizations_dto) {
  $rows = array();

  if (!empty($dataset_type_dto->studyTables) || !empty($dataset_type_dto->harmonizationTables)) {

    if (!empty($dataset_harmonizations_dto->variableHarmonizations)) {

      $opal_tables = _obiba_mica_dataset_harmonizations_get_all_opal_tables($dataset_type_dto);

      foreach ($dataset_harmonizations_dto->variableHarmonizations as $variable_harmonization) {
        $row = array(
          "index" => '',
          "variable" => l($variable_harmonization->resolver->name, 'mica/variable/' . $variable_harmonization->resolver->id),
        );

        $found = !empty($variable_harmonization->datasetVariableSummaries);

        foreach ($opal_tables as $opal_table) {
          $entity_summary = $opal_table->studySummary;
          $acronym = obiba_mica_commons_get_localized_field($entity_summary, 'acronym');
          $key_row = empty($opal_table->name)
            ? str_replace(' ', '_', $acronym)
            : str_replace(
              ' ',
              '_',
              $acronym . obiba_mica_commons_get_localized_field($opal_table, 'name')
            );

          if ($found) {
            foreach ($variable_harmonization->datasetVariableSummaries as $variable_summary) {
              if (($variable_summary->resolver->studyId == $opal_table->studyId)
                && $variable_summary->resolver->project == $opal_table->project
                && $variable_summary->resolver->table == $opal_table->table
              ) {
                $status = obiba_mica_dataset_variable_attributes_detail($variable_summary, 'status', array(
                  'Mlstr_harmo',
                ));
                if ($status == "impossible") {
                  $overridden_status = variable_get_value('dataset_harmonization_impossible_label');
                }
                else {
                  $overridden_status = trim(strtolower($status));
                }
                $status_detail = trim(strtolower(obiba_mica_dataset_variable_attributes_detail($variable_summary, 'status_detail', array(
                  'Mlstr_harmo',
                ))));
                $status_detail = empty($status_detail) ? ucfirst(preg_replace('/[-_]+/', ' ', $overridden_status)) : ucfirst(preg_replace('/[-_]+/', ' ', $status_detail));
                $harmonized_variable_id = $variable_summary->resolver->id;
                $variable_title = $variable_summary->resolver->name . ' -- ' . $acronym;

                if (!empty($opal_table->name)) {
                  $variable_title = $variable_title . ' (' . obiba_mica_commons_get_localized_field($opal_table, 'name') . ')';
                }

                switch ($status) {
                  case 'complete':
                    $icon_success_complete = '<span data-toggle="tooltip" class="glyphicon glyphicon-ok alert-success" title="' . $status_detail . '"></span>';
                    $row[$key_row] = l($icon_success_complete, 'mica/variable/' . $harmonized_variable_id,
                      array(
                        'html' => TRUE,
                        'query' => array(
                          'title' => $variable_title,
                        ),
                      ));
                    break;
                  case 'impossible':
                    $icon_danger_impossible = '<span data-toggle="tooltip" class="glyphicon ' .
                      ObibaDatasetConstants::getIcon()
                      . '" title="' . $status_detail . '"></span>';
                    $row[$key_row] = l($icon_danger_impossible, 'mica/variable/' . $harmonized_variable_id,
                      array(
                        'html' => TRUE,
                        'query' => array(
                          'title' => $variable_title,
                        ),
                      ));
                    break;
                  case 'undetermined':
                    $icon_warning_undetermined = '<span data-toggle="tooltip" class="glyphicon glyphicon-question-sign alert-warning" title="' . $status_detail . '"></span>';
                    $row[$key_row] = l($icon_warning_undetermined, 'mica/variable/' . $harmonized_variable_id,
                      array(
                        'html' => TRUE,
                        'query' => array(
                          'title' => $variable_title,
                        ),
                      ));
                    break;
                  default:
                    $row[$key_row] = '-';
                    break;
                }
              }
            }
          }
          else {
            $row[$key_row] = '-';
          }
        }

        $rows[] = $row;
      }
    }
  }

  return $rows;
}

function obiba_mica_dataset_ng_coverage($dataset_id) {
  $query = DrupalMicaClient\RqlQueryBuilder::dataset_query_charts($dataset_id);
  return obiba_mica_graphics_nvd3_query_charts($query, $dataset_id);
}

/**
 * Studies dataTable list initialisation.
 */
function obiba_mica_dataset_studies_datatable_init($dataset_type_dto) {
  if (drupal_get_library('obiba_mica_dataset', 'datatables')) {
    drupal_add_library('obiba_mica_dataset', 'datatables');
  }
  else {
    obiba_mica_commons_datatables_external_source();
  }

  $study_tables = array();
  $harmo_study_tables = array();
  if (!empty($dataset_type_dto->studyTables)) {
    foreach ($dataset_type_dto->studyTables as $study_table) {
      array_push($study_tables, $study_table);
    }
  }

  if (!empty($dataset_type_dto->studyTable)) {
    array_push($study_tables, $dataset_type_dto->studyTable);
  }

  if (!empty($dataset_type_dto->harmonizationTables)) {
    foreach ($dataset_type_dto->harmonizationTables as $harmo_study_table) {
      array_push($harmo_study_tables, $harmo_study_table);
    }
  }

  drupal_add_js(drupal_get_path('module', 'obiba_mica_dataset') . '/js/obiba_mica_dataset_harmo_studies_datatable_init.js', 'file');
  $data = array(
    'collection_studies_table_headers' => obiba_mica_study_study_summaries_table_headers(DrupalMicaStudyResource::INDIVIDUAL_STUDY),
    'collection_studies_table_data' => obiba_mica_dataset_study_tables_rows($study_tables, DrupalMicaStudyResource::INDIVIDUAL_STUDY),
    'harmonization_studies_table_headers' => obiba_mica_study_study_summaries_table_headers(DrupalMicaStudyResource::HARMONIZATION_STUDY),
    'harmonization_studies_table_data' => obiba_mica_dataset_study_tables_rows($harmo_study_tables, DrupalMicaStudyResource::HARMONIZATION_STUDY),
  );
  drupal_add_js($data, 'setting');
}

/**
 * Get studies table.
 */
function obiba_mica_dataset_study_tables_table($dataset_type_dto) {
  $localize = obiba_mica_commons_mica_server_localisation();
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
      'empty' => $localize->getTranslation('study.none'),
      'sticky' => FALSE,
    )
  );
}

/**
 * Parse study rows.
 */
function obiba_mica_dataset_study_tables_rows($study_tables, $study_type = DrupalMicaStudyResource::INDIVIDUAL_STUDY) {
  $rows = array();
  if (!empty($study_tables)) {
    $study_ids = array();
    foreach ($study_tables as $study_table) {
      if (!in_array($study_table->studyId, $study_ids)) {
        $study_ids[] = $study_table->studyId;
        $row = obiba_mica_study_study_summaries_table_row($study_table->studySummary);
        $rows[] =  obiba_mica_study_study_table_column($row, $study_type);;
      }
    }
  }
  return $rows;
}

/**
 * Download harmonization variables table.
 */
function obiba_mica_dataset_page_detail_download($dataset_id) {
  global $language;
  $dataset_resource = new DrupalMicaDatasetResource();
  $response = $dataset_resource->downloadVariablesHarmonizations($dataset_id, $language->language);
  if (!empty($response)) {
    $headers = $dataset_resource->getLastResponseHeaders();
    drupal_add_http_header('Content-Type', $headers['Content-Type'][0] . '; utf-8');
    drupal_add_http_header('Content-Disposition', $headers['Content-Disposition'][0]);
    ob_clean();
    $csv_response = !empty($copyright_notice) ? '"' . variable_get_value('mica_copyright_notice') . '"' . chr(13) : '';
    $csv_response .= $response->body;
    $csv_altered = module_invoke_all('harmonization_csv_alter', $csv_response);
    print empty($csv_altered) ? $csv_response : $csv_altered[0];
    exit;
  }
}

/**
 * Search variables.
 */
function obiba_mica_dataset_variables_search($dataset_type, $dataset_id, $query = NULL, $nature = NULL) {
  $dataset_resource = new DrupalMicaDatasetResource();
  $nature_query = empty($nature) ? '(nature:CATEGORICAL OR nature:CONTINUOUS)' : 'nature:' . strtoupper($nature);
  $query_string = empty($query) ? NULL : $query . ' AND ' . $nature_query;

  $variables = $dataset_resource->getVariables($dataset_type, $dataset_id, 0, 10, NULL, NULL, $query_string);
  drupal_json_output($variables);
}

/**
 * Variables cross tab.
 */
function obiba_mica_dataset_variables_crosstab($dataset_type, $dataset_id, $variable1, $variable2) {
  $dataset_resource = new DrupalMicaDatasetResource();

  $variables = $dataset_resource->crossVariables($dataset_type, $dataset_id, $variable1, $variable2);
  drupal_json_output($variables);
}

/**
 * Variables cross tab download CSV.
 */
function obiba_mica_dataset_variables_crosstab_download_csv($dataset_type, $dataset_id, $variable1, $variable2) {
  _obiba_mica_dataset_variables_crosstab_download('csv', $dataset_type, $dataset_id, $variable1, $variable2);
}

/**
 * Variables cross tab download Excel.
 */
function obiba_mica_dataset_variables_crosstab_download_excel($dataset_type, $dataset_id, $variable1, $variable2) {
  _obiba_mica_dataset_variables_crosstab_download('excel', $dataset_type, $dataset_id, $variable1, $variable2);
}

/**
 * Variables cross tab download.
 */
function _obiba_mica_dataset_variables_crosstab_download($doc_type, $dataset_type, $dataset_id, $variable1, $variable2) {
  $dataset_resource = new DrupalMicaDatasetResource();

  $response = $dataset_resource->downloadCrossVariables($dataset_type, $dataset_id, $variable1, $variable2, $doc_type);

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

/**
 * Variables information.
 */
function obiba_mica_dataset_get_variable_info($variable_id) {
  $dataset_resource = new DrupalMicaDatasetResource();
  $variable_dto = $dataset_resource->getVariable($variable_id);
  drupal_json_output($variable_dto);
}

/**
 * Dataset information.
 */
function obiba_mica_dataset_info($dataset_type, $dataset_id) {
  $dataset_resource = new DrupalMicaDatasetResource();
  $dataset_dto = $dataset_resource->getDataset($dataset_id, $dataset_type);
  drupal_json_output($dataset_dto);
}

function obiba_mica_dataset_study_tables($dto) {
  obiba_mica_dataset_studies_datatable_init($dto);
  return theme('obiba_mica_dataset-study-tables', array('dataset_type_dto' => $dto));
}