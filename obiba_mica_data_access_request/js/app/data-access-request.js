/*
 * Copyright (c) 2015 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
  Drupal.behaviors.obiba_mica_data_access_request = {
    attach: function (context, settings) {

      mica.DataAccessRequest = angular.module('mica.DataAccessRequest', [
        'ui.bootstrap',
        'obiba.notification',
        'schemaForm',
        'schemaForm-datepicker',
        'mica.attachment',
        'obiba.comments',
        'hc.marked',
        'pascalprecht.translate',
        'angularMoment'
      ]);

    }
  }
}(jQuery));
