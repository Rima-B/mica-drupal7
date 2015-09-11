'use strict';
(function ($) {
  Drupal.behaviors.obiba_angular_app_file_filter = {
    attach: function (context, settings) {

      mica.file
        .filter('bytes', function () {
          return function (bytes) {
            return bytes === null || typeof bytes === 'undefined' ? '' : filesize(bytes);
          };
        });

    }
  }
}(jQuery));
