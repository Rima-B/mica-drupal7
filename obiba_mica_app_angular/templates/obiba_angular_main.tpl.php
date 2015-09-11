<?php
/**
 * @file
 * Code for the obiba_mica_app_angular modules.
 */

?>

<div ng-app="mica" ng-controller="MainController">
  <obiba-alert id="MainController"></obiba-alert>
  <div ng-controller="NotificationController"></div>
  <div ng-view=""></div>
</div>
