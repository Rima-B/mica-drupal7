'use strict';
(function ($) {
  Drupal.behaviors.obiba_angular_app_file_service = {
    attach: function (context, settings) {

      mica.file
        .factory('TempFileResource', ['$resource',
          function ($resource) {
            return $resource('request/file/:idRequest/:id', {}, {
              'get': {method: 'GET'},
              'delete': {method: 'DELETE'}
            });
          }]);

    }
  }
}(jQuery));
