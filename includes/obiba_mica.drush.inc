<?php
/**
 * @file
 * drush commands  implementations to help users download required libraries.
 */

/**
 * Implements hook_drush_command().
 */
function obiba_mica_drush_command() {
  $items['highcharts-download'] = array(
    'callback' => 'obiba_mica_drush_highcharts_download',
    'aliases' => 'dhili',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional highcharts library from highcharts repositories.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/highcharts</code>).'),
    ),
  );

  $items['datatables-plugins-download'] = array(
    'callback' => 'obiba_mica_drush_datatable_bootstrap_plugins_download',
    'aliases' => 'dataplg',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional Datatables Bootstrap Plugin library from datatables.net .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/highcharts</code>).'),
    ),
  );

  $items['datatables-download'] = array(
    'callback' => 'obiba_mica_drush_datatable_download',
    'aliases' => 'ddlib',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional Datatables library from datatables.net .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/datatables</code>).'),
    ),
  );

  $items['obiba-progressbar-download'] = array(
    'callback' => 'obiba_mica_drush_obiba_progressbar',
    'aliases' => 'opblib',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the  obiba-progressbar library from Obiba repository .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/obiba-progressbar-1.0.0</code>).'),
    ),
  );

  return $items;
}

/**
 * Implementation of command to download HighCharts library.
 */
function obiba_mica_drush_highcharts_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }
  // Create the path if it does not exist.
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . '/sites/all/libraries/highcharts')) {
    drush_op('mkdir', $path . '/sites/all/libraries/highcharts');
  }
  if (!is_dir($path . '/sites/all/libraries/highcharts/js')) {

    drush_op('mkdir', $path . '/sites/all/libraries/highcharts/js');
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  else {
    drush_log('No dir @path', array('@path' => $path), 'notice');
  }
  $path = $path . '/sites/all/libraries/highcharts/js';
  chdir($path);

  $svn_cmd = 'wget   http://code.highcharts.com/highcharts.js';
  if (drush_shell_exec($svn_cmd)) {
    drush_log(dt('Highcharts.js  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Highcharts.js to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !wget.', array('!wget' => '<code>' . $svn_cmd . '</code>')), 'error');
  }

  $svn_cmd = 'wget   http://code.highcharts.com/highcharts-more.js';
  if (drush_shell_exec($svn_cmd)) {
    drush_log(dt('Highcharts-more.js  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Highcharts-more.js to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !wget.', array('!wget' => '<code>' . $svn_cmd . '</code>')), 'error');
  }

}

/**
 * Implementation of command to download dataTable library.
 */
function obiba_mica_drush_datatable_download() {
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
  if (!is_dir($path . '/sites/all/libraries/datatables')) {
    drush_op('mkdir', $path . '/sites/all/libraries/datatables');
  }
  $path = $path . '/sites/all/libraries/datatables';
  drush_op('mkdir', $path . '/DataTables-master');
  chdir($path . '/DataTables-master');

  $get_cmd = 'wget http://datatables.net/releases/DataTables-1.10.4.zip';
  if (drush_shell_exec($get_cmd)) {
    drush_log(dt('DataTables  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');

    $unzip_cmd = 'unzip DataTables-1.10.4.zip -d ' . $path . '/DataTables-master';
    if (drush_shell_exec($unzip_cmd)) {
      drush_log(dt('DataTables Unzipped  to !path.', array('!path' => ' ' . $path)), 'success');
      $path_to_del = $path . '/DataTables-master/DataTables-1.10.4';
      chdir($path_to_del);
      $mv_cmd = "mv * ../../";
      if (drush_shell_exec($mv_cmd)) {
        drush_log(dt('DataTables archive moved to   !path.', array('!path' => ' ' . $path)), 'success');
        chdir($path);
        $rm_cmd = "rm " . '-rf ' . $path . '/DataTables-master';
        if (drush_shell_exec($rm_cmd)) {
          drush_log(dt('DataTables archive deleted !path.', array('!path' => ' ' . $path)), 'success');
        }
      }
    }
  }
}

/**
 * Implementation of command to download DataTable Bootstrap Plugin library.
 */
function obiba_mica_drush_datatable_bootstrap_plugins_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  if (!is_dir($path . '/sites/all/libraries/datatables')) {
    drush_log(dt('Drush was unable to find DataTables plugin ', array()), 'error');
  }
  else {
    $path = $path . '/sites/all/libraries/datatables/extensions';
    chdir($path);
    // Create the plugins path if it does not exist.
    if (!is_dir($path . '/Plugins')) {
      drush_op('mkdir', $path . '/Plugins');
    }

    $path = $path . '/Plugins';
    drush_op('mkdir', $path . '/Plugins-master');
    chdir($path . '/Plugins-master');
    $get_cmd = 'wget https://github.com/DataTables/Plugins/archive/master.zip';

    if (drush_shell_exec($get_cmd)) {

      $unzip_cmd = 'unzip master.zip -d ' . $path . '/Plugins-master';
      if (drush_shell_exec($unzip_cmd)) {
        $path_to_del = $path . '/Plugins-master/Plugins-master';
        chdir($path_to_del);
        $mv_cmd = "mv * ../../";
        if (drush_shell_exec($mv_cmd)) {
          chdir($path);
          $rm_cmd = "rm " . '-rf ' . $path . '/Plugins-master';
          if (drush_shell_exec($rm_cmd)) {
            drush_log(dt('DataTables archive deleted !path.', array('!path' => ' ' . $path)), 'success');
          }
        }
      }

      drush_log(dt('DataTables Plugins was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
    }
    else {
      drush_log(dt('Drush was unable to download DataTables Plugins to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !git.', array('!git' => '<code>' . $get_cmd . '</code>')), 'error');
    }
  }
}

/**
 * Implementation of command to download Obiba Progress bar library.
 */
function obiba_mica_drush_obiba_progressbar() {
  $args = func_get_args();
  if ($args[1]) {
    $path = $args[1];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  if ($args[0]) {
    $version_progress_bar = $args[0];
  }
  else {
    $version_progress_bar = variable_get('obiba-progressbar-lib-version', '1.0.0');
  }

  // Create the libraries path if it does not exist.
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar")) {
    drush_op('mkdir', $path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar");
  }
  $path = $path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar";
  drush_op('mkdir', $path . '/obiba-progressbar-master');
  chdir($path . '/obiba-progressbar-master');

  $get_cmd = "wget https://github.com/obiba/obiba-progressbar/archive/$version_progress_bar.zip";
  if (drush_shell_exec($get_cmd)) {
    drush_log(dt('Obiba Progressbar  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');

    $unzip_cmd = "unzip $version_progress_bar.zip -d " . $path . '/obiba-progressbar-master';
    if (drush_shell_exec($unzip_cmd)) {
      drush_log(dt('Obiba Progressbar Unzipped  to !path.', array('!path' => ' ' . $path)), 'success');
      $path_to_del = $path . "/obiba-progressbar-master/";
      chdir($path_to_del);
      $mv_cmd = "mv obiba-progressbar-$version_progress_bar/* ../";
      if (drush_shell_exec($mv_cmd)) {
        drush_log(dt('Obiba Progressbar archive moved to   !path.', array('!path' => ' ' . $path)), 'success');
        chdir($path);
        $rm_cmd = "rm " . '-rf ' . $path . '/obiba-progressbar-master';
        if (drush_shell_exec($rm_cmd)) {
          drush_log(dt('Obiba Progressbar archive deleted !path.', array('!path' => ' ' . $path)), 'success');
        }
      }
    }
  }
}
