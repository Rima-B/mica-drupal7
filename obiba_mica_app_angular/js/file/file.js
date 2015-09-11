'use strict';
(function ($) {
  Drupal.behaviors.obiba_angular_app_file = {
    attach: function (context, settings) {

      mica.file = angular.module('mica.file', [
        'ngResource'
      ]);

    }
  }
}(jQuery));
