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
  Drupal.behaviors.obiba_mica_dataset_variable_crosstab = {
    attach: function (context, settings) {

      mica.DatasetVariableCrosstab = angular.module('mica.DatasetVariableCrosstab', [
        'ui.bootstrap',
        'obiba.notification',
        'schemaForm',
        'schemaForm-datepicker',
        'pascalprecht.translate',
        'angularMoment',
        'ui.bootstrap',
        'ui.select',
        'ui'
      ]);

      mica.DatasetVariableCrosstab.directive('toggle', function(){
        return {
          restrict: 'A',
          link: function(scope, element, attrs){
            if (attrs.toggle=="tooltip"){
              $(element).tooltip();
            }
            if (attrs.toggle=="popover"){
              $(element).popover();
            }
          }
        };
      })


    }
  }
}(jQuery));