/*
 * Copyright (c) 2015 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

'use strict';

(function ($) {
  Drupal.behaviors.obiba_mica_dataset_variable_crosstab_router = {
    attach: function (context, settings) {
      mica.DatasetVariableCrosstab.config(['$routeProvider', '$locationProvider',
        function ($routeProvider, $locationProvider) {
          $routeProvider
            .when('/crosstab/:type/:ds', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_dataset/obiba_mica_dataset_variable_crosstab',
              controller: 'DatasetVariableCrosstabController'
            })

          .when('/crosstab/:type/:ds/variable/:varId', {
            templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_dataset/obiba_mica_dataset_variable_crosstab',
            controller: 'DatasetVariableCrosstabController'
          })

          .when('/crosstab/:type/:ds/variable/:varId/by/:byId', {
            templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_dataset/obiba_mica_dataset_variable_crosstab',
            controller: 'DatasetVariableCrosstabController'
          })
        }]);


    }
  }
}(jQuery));