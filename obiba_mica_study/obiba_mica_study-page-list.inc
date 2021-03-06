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
 * Mica client studies retrieve Studies lists
 */

function obiba_mica_study_harmonization_page_list(){
 return  obiba_mica_study_list(DrupalMicaStudyResource::HARMONIZATION_STUDY);
}

function obiba_mica_study_collection_page_list(){
  return obiba_mica_study_list(DrupalMicaStudyResource::INDIVIDUAL_STUDY);
}

function obiba_mica_study_page_list(){
  return obiba_mica_study_list(DrupalMicaStudyResource::ALL_STUDIES);
}

function obiba_mica_study_list($study_type = NULL) {
  $template_study = 'obiba_mica_study_list-page';
  $trimed_description =  variable_get_value('studies_list_show_trimmed_description_study');
  if (!$trimed_description) {
    drupal_add_js(drupal_get_path('module', 'obiba_mica_commons') . '/js/obiba-mica-commons-list-shorten-text.js', 'file');
    drupal_add_js(array(
      'linkRead' => array(
        'more' => t('Read more'),
        'less' => t('Read less'),
      )
    ), 'setting');
  }
  obiba_mica_commons_list_document_js('refresh_list_studies', $study_type);
  $study_query = NULL;
  $study_sort = NULL;
  $study_sort_order = NULL;
  $study_limits = NULL;

  $localize = obiba_mica_commons_mica_server_localisation();
  $node_rendered = obiba_mica_commons_get_translated_node(variable_get_value('study_list_page'));
  //drupal_set_title($node_rendered['#node']->title);

  $themed_studies = '';
  $node_rendered['#node']->title = '';

  if (!empty($_GET['search-query'])) {
    $study_query = $_GET['search-query'];
  }
  if (!empty($_GET['search-sort'])) {
    $study_sort = $_GET['search-sort'];
  }
  if (!empty($_GET['search-sort-order'])) {
    $study_sort_order = $_GET['search-sort-order'];
  }

  if (!empty($_GET['search-nbr-result'])) {
    $study_limits = $_GET['search-nbr-result'];
  }

  $response = obiba_mica_study_get_studies($study_query, $study_sort, $study_sort_order, $study_limits, $study_type);
  $total_studies = empty($response) ? 0 : $response->getTotalHits();
  $study_limits = !empty($study_limits) ? $study_limits : DrupalMicaClientResource::getResponsePageSizeSmall();

  pager_default_initialize($total_studies, $study_limits);
  $pager_output = theme('pager', array('quantity' => $study_limits));
  $summaries = empty($response) ? array() : $response->getSummaries();
  if (!empty($summaries)) {
    $i = 0;

    foreach ($summaries as $study) {
      $template_study_block = 'obiba_mica_study-list-page-block';
      if($study->studyResourcePath == DrupalMicaStudyResource::HARMONIZATION_STUDY){
        $template_study_block = 'obiba_mica_study-list-harmo-study-page-block';
      }
      if (!empty($study->logo->id)) {
        $logo_url = obiba_mica_commons_safe_expose_server_url($study->id, $study->logo, $study->studyResourcePath);
      }
      else {
        $logo_url = NULL;
      }

      $themed_studies .= theme($template_study_block, array(
        'study' => $study,
        'logo_url' => $logo_url,
        'network_digests' => $response->getNetworkDigests($study->id)
      ));
      $i++;
    }
  }
  else {
    $themed_studies = $localize->getTranslation('study.none');
  }
  $show_rapid_search_form = variable_get_value('studies_list_show_search_form');
  return theme($template_study, array(
    'study_type' => $study_type,
    "form_search" => !empty($show_rapid_search_form) ? drupal_get_form('obiba_mica_study_create_form', $study_type) : '',
    "node_page" => $node_rendered,
    "total_items" => $total_studies,
    "list_studies" => $themed_studies,
    "pager_wrap" => $pager_output
  ));
}

function obiba_mica_study_retrieve_studies($query, $sort = NULL, $sort_order = NULL, $size_response_param = NULL) {
  $study_type = drupal_get_query_parameters()['document_type'];
  $list_studies = obiba_mica_study_get_studies($query, $sort, $sort_order, $size_response_param, $study_type);
  $list_studies_rendered = '';
  if (!empty($list_studies)) {
    foreach ($list_studies->getSummaries() as $study) {
      if($study->studyResourcePath == DrupalMicaStudyResource::HARMONIZATION_STUDY)
      {
        $list_studies_rendered .= theme('obiba_mica_study-list-harmo-study-page-block', array(
          'study' => $study,
        ));
      }
    else{
        $list_studies_rendered .= theme('obiba_mica_study-list-page-block', array(
          'study' => $study,
        ));
      }
    }
    $data['list'] = $list_studies_rendered;
    $data['total'] = $list_studies->getTotalHits();
    drupal_json_output($data);
  }
  else {
    return FALSE;
  }
}

function obiba_mica_study_create_form($form, &$form_state, $studies_type) {
  $localize = obiba_mica_commons_mica_server_localisation();
  $context =
    array(
      'options' =>
        array(
          'model.numberOfParticipants.participant.number' => $localize->getTranslation('client.label.study.target-number')
        ),
      'default_sort' => variable_get_value('study_list_name_acronym_sort'),
      'default_sort_order' => variable_get_value('study_list_default_order_sort'),
    );
  if ($studies_type == DrupalMicaStudyResource::HARMONIZATION_STUDY) {
    unset($context['options']['model.numberOfParticipants.participant.number']);
  }
  return obiba_mica_commons_create_search_form($context);
}

function obiba_mica_study_get_studies($query = NULL, $sort = NULL, $order = NULL, $study_limits = NULL, $study_type = NULL) {
  $from = 0;
  $study_resource = new DrupalMicaStudyResource();
  $size_response = intval($study_limits) > 0 ? $study_limits : DrupalMicaClientResource::getResponsePageSizeSmall();
  prepopulate_session_with_studies_count_if_needed($study_type, $study_resource);

  if (!empty($_GET['page'])) {
    $from = $study_resource->paginationListSearchParameters($_GET['page'], $study_type, $size_response);
  }

  if(empty($order)){
    $study_order = variable_get_value('study_list_default_order_sort');
    $order = !empty($study_order) ? $study_order : '';
  }

  if (empty($sort)) {
    $study_list_sort_option = variable_get_value('study_list_name_acronym_sort');
    if (!empty($study_list_sort_option)) {
        $sort = $study_list_sort_option;
    }
  }

  $fields = array("logo", "objectives.*", "acronym.*", "name.*", "model.methods.design", "model.numberOfParticipants.participant");
  $response = $study_resource->getStudies($query, $from, $size_response, $sort, $order, $study_type, $fields);

  populate_session_with_studies_count($response, $study_type);

  return $response;
}

function prepopulate_session_with_studies_count_if_needed($type, DrupalMicaStudyResource $study_resource) {

  if (empty(@$_SESSION[$type])) {
    $studies = $study_resource->getStudies(NULL, 0, 1, NULL, NULL, array());
    populate_session_with_studies_count($studies, $type);
  }
}

function populate_session_with_studies_count($response, $type) {
  if (!empty($response)) {
    $_SESSION[$type]['aggregations']['total_hits'] = $response->getTotalHits();
  }
}

function obiba_mica_study_studies_table($studies) {
  $localize = obiba_mica_commons_mica_server_localisation();
  $rows = array();
  $total = 0;
  if (!empty($studies['total_hits']) && $studies['total_hits'] > 0) {
    $total = $studies['total_hits'];
    $rows = obiba_mica_study_study_summaries_table_rows($studies['data']);
  }
  $headers = obiba_mica_study_study_summaries_table_headers();
  $per_page = 20;

  $args = array(
    'quantity' => $total,
    'element' => DrupalMicaStudyResource::PAGINATE_STUDIES
  );
  $current_page = pager_default_initialize($total, $per_page, DrupalMicaStudyResource::PAGINATE_STUDIES);
  $chunks = array_chunk($rows, $per_page, TRUE);
  return theme(
    'table',
    array(
      'header' => $headers,
      'rows' => empty($chunks) ? array() : $chunks[$current_page],
      'empty' => $localize->getTranslation('study.none')
    )
  ) . theme('pager', $args);
}

/**
 * Format column study table depending in studies type
 */
function obiba_mica_study_study_table_column($row, $type) {
  switch ($type) {
    case DrupalMicaStudyResource::HARMONIZATION_STUDY:
      array_splice($row, 2, -1);
      return $row;
      break;
    default:
      return $row;
  }
}


function obiba_mica_study_study_summaries_table_headers($study_type = DrupalMicaStudyResource::INDIVIDUAL_STUDY) {
  $localize = obiba_mica_commons_mica_server_localisation();
  $header = array(
    array("title" => $localize->getTranslation('study.acronym')),
    array("title" => $localize->getTranslation('study.name')),
    array("title" => $localize->getTranslation('study.design')),
    array("title" => $localize->getTranslation('client.label.dataset.participants')),
    array("title" => $localize->getTranslation('client.label.countries')),
    array("title" => $localize->getTranslation('variables')),
  );
  return obiba_mica_study_study_table_column($header, $study_type);
}

function obiba_mica_study_study_summaries_table_rows($study_summaries) {
  $rows = array();
  if (!empty($study_summaries)) {
    foreach ($study_summaries as $study_summary) {
      $rows[] = obiba_mica_study_study_summaries_table_row($study_summary);
    }
  }
  return $rows;
}

function obiba_mica_study_study_summaries_table_row($study_summary) {
  $localize = obiba_mica_commons_mica_server_localisation();
  $name = obiba_mica_commons_get_localized_field($study_summary, 'name');
  return array(
    DrupalMicaStudyResource::anchorStudy($study_summary),
    $name,
    isset($study_summary->design) ? obiba_mica_study_translate_study_design_summary($study_summary->design) : '',
    isset($study_summary->targetNumber->noLimit) ? $localize->getTranslation('numberOfParticipants.no-limit') :
      isset($study_summary->targetNumber->number) ? obiba_mica_commons_format_number($study_summary->targetNumber->number) : NULL,
    isset($study_summary->countries) ? obiba_mica_commons_countries($study_summary->countries) : NULL,
    isset($study_summary->variables) ?obiba_mica_commons_format_number($study_summary->variables) : NULL,
  );
}
