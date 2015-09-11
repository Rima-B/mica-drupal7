<?php

/**
 * @file
 * Obiba Mica client module settings UI.
 */

/**
 * Provides settings pages.
 */
function obiba_mica_admin_settings() {
  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('OBiBa study server (Mica)'),
    '#collapsible' => FALSE,
  );

  _obiba_mica_settings_server($form);

  $form['image_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache images settings'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('image_cache_time_out');
  $form['image_cache']['image_cache_time_out'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('image_cache_time_out'),
    '#description' => $info['description'],
  );

  $form['image_cache']['clear_cache'] = array(
    '#type' => 'submit',
    '#value' => 'Clear images Cache',
    '#submit' => array('obiba_mica_clear_images_cache'),
    '#limit_validation_errors' => array(),
  );

  $form['#submit'] = array('obiba_mica_default_password');
  return system_settings_form($form);
}

/**
 * Server setting forms.
 */
function _obiba_mica_settings_server(&$form) {
  $info = variable_get_info('mica_url');
  $form['server']['mica_url'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#required' => TRUE,
    '#default_value' => variable_get_value('mica_url'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_anonymous_name');
  $form['server']['mica_anonymous_name'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#required' => TRUE,
    '#default_value' => variable_get_value('mica_anonymous_name'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_anonymous_password');
  $form['server']['mica_anonymous_password'] = array(
    '#type' => 'password',
    '#title' => $info['title'],
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_copyright_notice');
  $form['server']['mica_copyright_notice'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_copyright_notice'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('mica_response_page_size');
  $form['server']['mica_response_page_size'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_response_page_size'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('mica_response_page_size');
  $form['server']['mica_response_page_size'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_response_page_size'),
    '#description' => $info['description'],
  );
  $info = variable_get_info('mica_response_page_size_small');
  $form['server']['mica_response_page_size_small'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('mica_response_page_size_small'),
    '#description' => $info['description'],
  );
}

/**
 * Call Back on submit setting form option to set default password.
 */
function obiba_mica_default_password($form, &$form_state) {
  if (empty($form_state['values']['mica_anonymous_password'])) {
    $form_state['values']['mica_anonymous_password']
      = variable_get_value('mica_anonymous_password',
        array('default' => 'password'));
  }
}

/**
 * Call Back on clear images cache..
 */
function obiba_mica_clear_images_cache() {
  cache_clear_all('images-cache-', 'cache', TRUE);
  drupal_set_message(t('The images cache cleared'));
}
