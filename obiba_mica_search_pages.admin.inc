<?php

/**
 * @file
 * Obiba Mica client module settings UI.
 */

$path_module_commons = drupal_get_path('module', 'obiba_mica_commons');
include_once($path_module_commons . '/includes/obiba_mica_commons_taxonomy_resource.inc');

/**
 * Provides settings pages.
 */
function obiba_mica_search_pages_admin_settings() {
  $form['search_display_settings'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Search page display settings'),
    '#collapsible' => FALSE,
  );

  _obiba_mica_settings_search($form);
  _obiba_mica_settings_search_networks_form($form);
  _obiba_mica_settings_search_studies_form($form);
  _obiba_mica_settings_search_datasets_form($form);
  _obiba_mica_settings_search_variables_form($form);

  return system_settings_form($form);
}

/**
 * settings of search page
 * @param $form
 */
function _obiba_mica_settings_search(&$form) {
  $form['search_display_settings']['page_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search Settings'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('help_text_search_page');
  $form['search_display_settings']['page_settings']['help_text_search_page'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#required' => FALSE,
    '#default_value' => variable_get_value('help_text_search_page'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('help_text_harmonization_potential_page');
  $form['search_display_settings']['page_settings']['help_text_harmonization_potential_page'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#required' => FALSE,
    '#default_value' => variable_get_value('help_text_harmonization_potential_page'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_statistics_coverage');
  $form['search_display_settings']['page_settings']['mica_statistics_coverage'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_statistics_coverage'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_taxonomy_figures');
  $form['search_display_settings']['page_settings']['mica_taxonomy_figures'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#required' => FALSE,
    '#default_value' => variable_get_value('mica_taxonomy_figures'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_taxonomy_order');
  $value = variable_get_value('mica_taxonomy_order');
  $form['search_display_settings']['page_settings']['mica_taxonomy_order'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#required' => FALSE,
    '#default_value' => empty($value) ? _obiba_mica_admin_get_taxonomy_summaries() : $value,
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

}


function _obiba_mica_settings_search_networks_form(&$form) {
  $form['search_display_settings']['networks_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Networks Search'),
    '#collapsible' => FALSE,
  );

  // tab
  $info = variable_get_info('search_networks');
  $form['search_display_settings']['networks_form']['search_networks'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_networks'),
    '#description' => $info['description'],
  );

  // columns
  $form['search_display_settings']['networks_form']['networks_column'] = array(
    '#type' => 'fieldset',
    '#title' => t('Network column display settings'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('networks_column_studies');
  $form['search_display_settings']['networks_form']['networks_column']['networks_column_studies'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_column_studies'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('networks_column_study_datasets');
  $form['search_display_settings']['networks_form']['networks_column']['networks_column_study_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_column_study_datasets'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('networks_column_harmonization_datasets');
  $form['search_display_settings']['networks_form']['networks_column']['networks_column_harmonization_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_column_harmonization_datasets'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('networks_column_variables');
  $form['search_display_settings']['networks_form']['networks_column']['networks_column_variables'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_column_variables'),
    '#description' => $info['description'],
  );

  // labels
  $info = variable_get_info('networks_search_label');
  $form['search_display_settings']['networks_form']['networks_search_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_search_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('networks_empty_label');
  $form['search_display_settings']['networks_form']['networks_empty_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('networks_empty_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
}

function _obiba_mica_settings_search_studies_form(&$form) {
  $form['search_display_settings']['studies_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Studies Search'),
    '#collapsible' => FALSE,
  );

  // tab
  $info = variable_get_info('search_studies');
  $form['search_display_settings']['studies_form']['search_studies'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_studies'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('studies_facet_search');
  $form['search_display_settings']['studies_form']['studies_facet_search'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_facet_search'),
    '#description' => $info['description'],
  );

  // columns
  $form['search_display_settings']['studies_form']['studies_column'] = array(
    '#type' => 'fieldset',
    '#title' => t('Study column display settings'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('studies_column_design');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_design'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_design'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_questionnaire');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_questionnaire'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_questionnaire'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_pm');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_pm'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_pm'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_bio');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_bio'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_bio'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_other');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_other'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_other'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_participants');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_participants'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_participants'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_networks');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_networks'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_networks'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_study_datasets');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_study_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_study_datasets'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_harmonization_datasets');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_harmonization_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_harmonization_datasets'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_column_variables');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_variables'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_variables'),
    '#description' => $info['description'],
  );

  // content details
  $info = variable_get_info('studies_column_variables');
  $form['search_display_settings']['studies_form']['studies_column']['studies_column_variables'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_column_variables'),
    '#description' => $info['description'],
  );

  // labels
  $info = variable_get_info('studies_search_label');
  $form['search_display_settings']['studies_form']['studies_search_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_search_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('studies_empty_label');
  $form['search_display_settings']['studies_form']['studies_empty_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_empty_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('search_studies_dce_autocomplete');
  $form['search_display_settings']['studies_form']['search_studies_dce_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_studies_dce_autocomplete'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('search_general_population_sources_autocomplete');
  $form['search_display_settings']['studies_form']['search_general_population_sources_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_general_population_sources_autocomplete'),
    '#description' => $info['description'],
  );
}

function _obiba_mica_settings_search_datasets_form(&$form) {
  $form['search_display_settings']['datasets_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datasets Search'),
    '#collapsible' => FALSE,
  );

  // tab
  $info = variable_get_info('search_datasets');
  $form['search_display_settings']['datasets_form']['search_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_datasets'),
    '#description' => $info['description'],
  );

  // columns
  $form['search_display_settings']['datasets_form']['datasets_column'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dataset column display settings'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('datasets_column_type');
  $form['search_display_settings']['datasets_form']['datasets_column']['datasets_column_type'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_column_type'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('datasets_column_networks');
  $form['search_display_settings']['datasets_form']['datasets_column']['datasets_column_networks'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_column_networks'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('datasets_column_studies');
  $form['search_display_settings']['datasets_form']['datasets_column']['datasets_column_studies'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_column_studies'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('datasets_column_variables');
  $form['search_display_settings']['datasets_form']['datasets_column']['datasets_column_variables'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_column_variables'),
    '#description' => $info['description'],
  );

  // labels
  $info = variable_get_info('datasets_search_label');
  $form['search_display_settings']['datasets_form']['datasets_search_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_search_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('datasets_empty_label');
  $form['search_display_settings']['datasets_form']['datasets_empty_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('datasets_empty_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('search_datasets_autocomplete');
  $form['search_display_settings']['datasets_form']['search_datasets_autocomplete'] = array(
    '#type' => 'select',
    '#options' => array(
      ObibaDatasetConstants::FACET_TYPE_HIDE => t('Hide'),
      ObibaDatasetConstants::FACET_TYPE_AUTOCOMPLETE => t('Autocomplete'),
      ObibaDatasetConstants::FACET_TYPE_CHECKBOX => t('Checkbox')
    ),
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_datasets_autocomplete'),
    '#description' => $info['description'],
  );
}

function _obiba_mica_settings_search_variables_form(&$form) {
  $form['search_display_settings']['variables_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Variables Search'),
    '#collapsible' => FALSE,
  );

  // tab
  $info = variable_get_info('search_variables');
  $form['search_display_settings']['variables_form']['search_variables'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('search_variables'),
    '#description' => $info['description'],
  );

  // columns
  $form['search_display_settings']['variables_form']['variables_column'] = array(
    '#type' => 'fieldset',
    '#title' => t('Variables column display settings'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('variables_column_study');
  $form['search_display_settings']['variables_form']['variables_column']['variables_column_study'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variables_column_study'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('variables_column_dataset');
  $form['search_display_settings']['variables_form']['variables_column']['variables_column_dataset'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variables_column_dataset'),
    '#description' => $info['description'],
  );

  // labels
  $info = variable_get_info('variable_coverage_label');
  $form['search_display_settings']['variables_form']['variable_coverage_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variable_coverage_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('variables_search_label');
  $form['search_display_settings']['variables_form']['variables_search_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variables_search_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('variables_empty_label');
  $form['search_display_settings']['variables_form']['variables_empty_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variables_empty_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
}


/**
 * Retrieves taxonomies
 * TODO this should become a dragtable action callback so clients can always refresh the list when desired
 * @return string
 */
function _obiba_mica_admin_get_taxonomy_summaries() {
  $service = new MicaTaxonomyResource();
  $taxonomies = $service->getTaxonomySummaries();
  $list = array();

  if (!empty($taxonomies) && !empty($taxonomies->summaries)) {
    foreach ($taxonomies->summaries as $summary) {
      $list[] = $summary->name;
    }
  }

  return implode(',', $list);
}