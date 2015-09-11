'use strict';
(function ($) {
  Drupal.behaviors.obiba_angular_app_attachement = {
    attach: function (context, settings) {
      mica.attachment = angular.module('mica.attachment', [
        'mica.file',
        'ui',
        'ui.bootstrap',
        'ngFileUpload'
      ]);

    }
  }
}(jQuery));
