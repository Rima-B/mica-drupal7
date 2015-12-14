<?php
/**
 * @file
 * Obiba Mica variable file
 */

/**
 * Implements hook_variable_info().
 */
function obiba_mica_persons_variable_info($options) {
  $variable['associated_people'] = array(
    'title' => t('Affiliated Members'),
    'description' => t('The Affiliated Members captions'),
    'type' => 'string',
    'default' => 'Affiliated Members',
  );
  return $variable;
}
