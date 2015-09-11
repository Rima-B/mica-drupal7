<?php

/**
 * @file
 * Obiba Mica client module settings UI.
 */

/**
 * Provides settings pages form.
 *
 * @return array
 *   The setting form on a array drupal markup.
 */
function obiba_mica_content_pages_admin_settings() {
  $form['content_display_settings'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Content page display settings'),
    '#collapsible' => FALSE,
  );

  obiba_mica_settings_study_content($form);
  obiba_mica_settings_dataset_content($form);
  obiba_mica_settings_variable_content($form);
  obiba_mica_settings_data_access_content($form);

  $form['translation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Translations'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('mica_disabled_path_translation');
  $form['translation']['mica_disabled_path_translation'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#required' => FALSE,
    '#default_value' => variable_get_value('mica_disabled_path_translation'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  return system_settings_form($form);
}

/**
 * Data access content form setting.
 *
 * @param array $form
 *   The admin setting form.
 */
function obiba_mica_settings_data_access_content(&$form) {
  $form['content_display_settings']['access_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data Access Content'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('access_home_page_title');
  $form['content_display_settings']['access_form']['access_home_page_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_home_page_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_requests_page_title');
  $form['content_display_settings']['access_form']['my_requests']['access_requests_page_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_requests_page_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_request_page_title');
  $form['content_display_settings']['access_form']['my_requests']['access_request_page_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_request_page_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_documents_section_title');
  $form['content_display_settings']['access_form']['my_requests']['access_documents_section_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_documents_section_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('access_documents_section_help_text');
  $form['content_display_settings']['access_form']['my_requests']['access_documents_section_help_text'] = array(
    '#type' => 'textarea',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_documents_section_help_text'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_my_requests_button');
  $form['content_display_settings']['access_form']['access_my_requests_button'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_my_requests_button'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_download_button');
  $form['content_display_settings']['access_form']['access_download_button'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_download_button'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_new_request_button');
  $form['content_display_settings']['access_form']['access_new_request_button'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_new_request_button'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_signup_button');
  $form['content_display_settings']['access_form']['access_signup_button'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_signup_button'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_signin_button');
  $form['content_display_settings']['access_form']['access_signin_button'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_signin_button'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('access_comments_enabled');
  $form['content_display_settings']['access_form']['access_comments_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('access_comments_enabled'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
}

/**
 * Study content form setting.
 *
 * @param array $form
 *   The admin setting form.
 */
function obiba_mica_settings_study_content(&$form) {
  $form['content_display_settings']['study_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Study Content'),
    '#collapsible' => FALSE,
  );

  $form['content_display_settings']['study_form']['study_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Study details page'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('study_title_acronym_display');
  $form['content_display_settings']['study_form']['study_details']['study_title_acronym_display'] = array(
    '#type' => 'radios',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('study_title_acronym_display'),
    '#options' => array(
      0 => t('Study Title - Acronym'),
      1 => t('Acronym - Study Title'),
      2 => t('Study Title only'),
    ),
    '#description' => $info['description'],
  );

  $info = variable_get_info('study_show_networks');
  $form['content_display_settings']['study_form']['study_details']['study_show_networks'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('study_show_networks'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('study_show_datasets');
  $form['content_display_settings']['study_form']['study_details']['study_show_datasets'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('study_show_datasets'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('study_show_coverage_graphs');
  $form['content_display_settings']['study_form']['study_details']['study_show_coverage_graphs'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('study_show_coverage_graphs'),
    '#description' => $info['description'],
  );

  $form['content_display_settings']['study_form']['studies_list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Studies list page'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('studies_list_show_search_form');
  $form['content_display_settings']['study_form']['studies_list']['studies_list_show_search_form'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_list_show_search_form'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('studies_list_show_search_button');
  $form['content_display_settings']['study_form']['studies_list']['studies_list_show_search_button'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_list_show_search_button'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('studies_list_show_study_sup_info');
  $form['content_display_settings']['study_form']['studies_list']['studies_list_show_study_sup_info'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_list_show_study_sup_info'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('studies_list_show_trimmed_description_study');
  $form['content_display_settings']['study_form']['studies_list']['studies_list_show_trimmed_description_study'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('studies_list_show_trimmed_description_study'),
    '#description' => $info['description'],
  );

}

/**
 * Dataset content form setting.
 *
 * @param array $form
 *   The admin setting form.
 */
function obiba_mica_settings_dataset_content(&$form) {
  $bootstrap_theme_path = base_path() . drupal_get_path('theme', 'obiba_bootstrap');
  $form['content_display_settings']['dataset_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dataset Content'),
    '#collapsible' => FALSE,
  );

  $form['content_display_settings']['dataset_form']['dataset_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Study details page'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('dataset_title_acronym_setup');
  $form['content_display_settings']['dataset_form']['dataset_details']['dataset_title_acronym_setup'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('dataset_title_acronym_setup'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('dataset_show_studies');
  $form['content_display_settings']['dataset_form']['dataset_details']['dataset_show_studies'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('dataset_show_studies'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('dataset_description_overview_field');
  $form['content_display_settings']['dataset_form']['dataset_details']['dataset_description_overview_field'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('dataset_description_overview_field'),
    '#description' => $info['description'],
  );

  $form['content_display_settings']['dataset_form']['harmonization_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Harmonization status display'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('dataset_harmonization_impossible_icon');
  $form['content_display_settings']['dataset_form']['harmonization_status']['dataset_harmonization_impossible_icon'] = array(
    '#type' => 'radios',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('dataset_harmonization_impossible_icon'),
    '#options' => array(
      ObibaDatasetConstants::IMPOSSIBLE_HARMO_ICON => '<img src="' . $bootstrap_theme_path . '/glyphicon-remove.png" > Remove',
      ObibaDatasetConstants::INCOMPLETE_HARMO_ICON => '<img src="' . $bootstrap_theme_path . '/glyphicon-minus.png" > Minus',
    ),
    '#description' => $info['description'],
  );

  $info = variable_get_info('dataset_harmonization_impossible_label');
  $form['content_display_settings']['dataset_form']['harmonization_status']['dataset_harmonization_impossible_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#maxlength' => 255,
    '#default_value' => variable_get_value('dataset_harmonization_impossible_label'),
    '#description' => $info['description'],
  );

}

/**
 * Variable content form setting.
 *
 * @param array $form
 *   The admin setting form.
 */
function obiba_mica_settings_variable_content(&$form) {
  $form['content_display_settings']['variable_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Variable Content'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('mica_statistics');
  $form['content_display_settings']['variable_form']['mica_statistics'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_statistics'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('variable_show_studies');
  $form['content_display_settings']['variable_form']['variable_show_studies'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('variable_show_studies'),
    '#description' => $info['description'],
  );

  $info = variable_get_info('dataset_detailed_var_stats');
  $form['content_display_settings']['variable_form']['dataset_detailed_var_stats'] = array(
    '#type' => 'checkbox',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('dataset_detailed_var_stats'),
    '#description' => $info['description'],
  );
}
