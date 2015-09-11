/**
 * @file
 * JavaScript ajax helper for Statistics variables retrieving
 */
var mica;
(function ($) {
  Drupal.behaviors.obiba_angular_app = {
    attach: function (context, settings) {

      var modules = [
        'ngObiba',
        'ngRoute',
        'ngSanitize',
        'ngResource',
        'ui.bootstrap',
        'obiba.form',
        'obiba.comments',
        'angularUtils.directives.dirPagination',
        'pascalprecht.translate'
      ];

      'use strict';
      /* App Module */
      if (Drupal.settings.angularjsApp.module) {
        modules = modules.concat(Drupal.settings.angularjsApp.module);
      }
      mica = angular.module('mica', modules);
      mica.config(['$routeProvider', '$translateProvider',
        function ($routeProvider, $translateProvider) {
          $routeProvider
            .when('/', {
              controller: 'MainController'
            });

          $translateProvider.useLoader('DrupalTranslationLoader', {});
          $translateProvider.preferredLanguage('en');

        }]);

      mica.controller('MainController', [
        function () {
        }]);

      mica.factory('TranslationService', ['$resource',
        function ($resource) {
          return $resource(Drupal.settings.basePath + 'obiba_mica_app_angular/translation', {}, {
            'get': {method: 'GET'}
          });
        }]);

      mica.factory('DrupalTranslationLoader',
        function ($http, $q) {
          return function (options) {
            var deferred = $q.defer();

            $http({
              method: 'GET',
              url: Drupal.settings.basePath + 'obiba_mica_app_angular/translation'
            }).success(function (data) {
              deferred.resolve(data);
            }).error(function () {
              deferred.reject(options.key);
            });

            return deferred.promise;
          }
        });

      mica.factory('ErrorTemplate', function () {
        return {
          getServerError: function (response) {
            if (angular.isObject(response.data)) {
              if (!response.data.messageTemplate) {
                response.data.messageTemplate = 'server.error.' + response.status;
              }
            } else {
              response.data = {messageTemplate: 'server.error.' + response.status};
            }
            return response;
          }
        }

      });

      mica.factory('ForbiddenDrupalRedirect', function () {

        var createDestinationPath = function(path) {
          if (angular.isDefined(path)) {
            var regExp = new RegExp('(view|edit)\/(.*)$');
            var results = regExp.exec(path);
            if (results && results.length > 1) {
              return '?destination=mica/data-access/request/redirect/' + results[1] + '/' + results[2];
            }

            return '';
          }
        };

        return {
          redirectDrupalMessage: function (response) {
            if (response.status && response.status == 403 && !Drupal.settings.angularjsApp.authenticated) {
              $.post('un-authorized-error');
              $(window).delay(200).queue(function () {
                window.location = Drupal.settings.basePath + 'user/login' + createDestinationPath(window.location.hash);
              });
            }
          }
        }

      })

      /**
       * A N G U L A R     G L O B A L     S E R V I C E S
       */

      .service('ServerErrorAlertService', ['AlertService', 'ServerErrorUtils', 'ErrorTemplate',
        function(AlertService, ServerErrorUtils, ErrorTemplate) {
          this.alert = function(id, response) {
            if (angular.isDefined(response.data)) {
              var errorDto = JSON.parse(response.data);
              if (angular.isDefined(errorDto) && angular.isDefined(errorDto.messageTemplate)) {
                AlertService.alert({
                  id: id,
                  type: 'danger',
                  msgKey: errorDto.messageTemplate,
                  msgArgs: errorDto.arguments
                });
                return;
              }
            }

            AlertService.alert({
              id: id,
              type: 'danger',
              msg: ServerErrorUtils.buildMessage(ErrorTemplate.getServerError(response))
            });
          };

          return this;
        }])

      .service('AttributeService',
        function () {
          return {
            getAttributes: function(container, names) {
              if (!container && !container.attributes && !names){
                return null;
              }
              return container.attributes.filter(
                function(attribute) {
                  return names.indexOf(attribute.name) !== -1;
                });
            },

            getValue: function (attribute) {
              if (!attribute) {
                return null;
              }
              var value = attribute.values.filter(
                function(value) {
                  return value.lang === Drupal.settings.angularjsApp.locale || value.lang === 'und';
                });

              return value.length > 0 ? value[0].value : null;
            }
          }
        })

      .service('LocalizedStringService',
        function () {
          return {
            getValue: function (localized) {
              if (!localized) {
                return null;
              }
              var value = localized.filter(
                function(locale) {
                  return locale.lang === Drupal.settings.angularjsApp.locale || locale.lang === 'und';
                });

              return value.length > 0 ? value[0].value : null;
            }
          }
        })

    }
  }
}(jQuery));
