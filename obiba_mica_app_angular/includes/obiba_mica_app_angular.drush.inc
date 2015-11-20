<?php
/**
 * @file
 * Drush commands  implementations to help users download required libraries.
 */

/**
 * Implements hook_drush_command().
 */
function obiba_mica_app_angular_drush_command() {
  $items['angular-app'] = array(
    'callback' => 'obiba_mica_drush_angular_app',
    'aliases' => array('asflib'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Install the Angula js + angular-schema-form library, Please make sure to perform this command  "bower install angular-schema-form"  in current folder before installing '),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/angular-schema-form</code>).'),
    ),
  );

  return $items;
}

/**
 * Implementation of command to download angular dependencies library.
 *
 * Need the bower.json dependency configuration file.
 */
function obiba_mica_drush_angular_app() {
  // Download lib by bower installer.
  $bower_install_cmd = "cd sites/all/modules/obiba_mica && bower --allow-root install";
  if ($out = drush_shell_exec($bower_install_cmd)) {
    drush_log(dt('Angular-js dependencies downloaded in /bower_components'), 'success');

    $args = func_get_args();
    if ($args[0]) {
      $path = $args[0];
    }
    else {
      $path = drush_get_context('DRUSH_DRUPAL_ROOT');
    }

    // Create the libraries path if it does not exist.
    if (!is_dir($path . '/sites/all/libraries')) {
      drush_op('mkdir', $path . '/sites/all/libraries');
    }
    if (!is_dir($path . '/sites/all/libraries/angular-app')) {
      drush_op('mkdir', $path . '/sites/all/libraries/angular-app');
    }
    $path = $path . '/sites/all/libraries/angular-app';
    $bower_components_dir = 'sites/all/modules/obiba_mica/bower_components/';
    $mv_cmd = "mv $bower_components_dir* $path";
    if (drush_shell_exec($mv_cmd)) {
      drush_log(dt('angular-schema-form dependencies moved to !path.', array('!path' => ' ' . $path)), 'success');
      $rm_cmd = "rm " . '-rf ' . $bower_components_dir;
      if (drush_shell_exec($rm_cmd)) {
        drush_log(dt('angular-schema-form downloaded folder deleted !path.', array('!path' => ' ' . $bower_components_dir)), 'success');
      }
    }
  }
  else {
    drush_log(dt('can not download angular-app '), 'error');
  }
}
