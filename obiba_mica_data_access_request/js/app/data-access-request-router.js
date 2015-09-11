/*
 * Copyright (c) 2014 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

'use strict';

(function ($) {
  Drupal.behaviors.obiba_mica_data_access_request_router = {
    attach: function (context, settings) {
      mica.DataAccessRequest.config(['$routeProvider', '$locationProvider',
        function ($routeProvider, $locationProvider) {
          $routeProvider
//            .when('/data-access-request-list', {
//              controller: 'DataAccessRequestListController'
//            })
            .when('/new', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_data_access_request-form-page',
              controller: 'DataAccessRequestEditController'
            })
            .when('/edit/:id', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_data_access_request-form-page',
              controller: 'DataAccessRequestEditController'
            })
            .when('/view/:id', {
              templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_data_access_request-view-page',
              controller: 'DataAccessRequestViewController'
            });

        }]);

    }
  }
}(jQuery));
