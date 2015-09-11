<?php
/**
 * @file
 * Code for the obiba_mica_data_access_request modules.
 */

?>

<!--
  ~ Copyright (c) 2015 OBiBa. All rights reserved.
  ~
  ~ This program and the accompanying materials
  ~ are made available under the terms of the GNU Public License v3.0.
  ~
  ~ You should have received a copy of the GNU General Public License
  ~ along with this program.  If not, see <http://www.gnu.org/licenses/>.
  -->

<div>
  <h2 class="page-header">
    <a href="<?php print url(MicaClientPathProvider::DATA_ACCESS_LIST); ?>"
      title="<?php print variable_get_value('access_my_requests_button'); ?>">
      <i class="glyphicon glyphicon-chevron-left"></i>
    </a>
    <span ng-if="newRequest">
      <?php print t('New') . ' '  . variable_get_value('access_request_page_title'); ?>
    </span>
    <span ng-if="!newRequest">
      <?php print t('Edit') . ' ' . variable_get_value('access_request_page_title'); ?>: {{requestId}}
    </span>
  </h2>

  <obiba-alert id="DataAccessRequestEditController"></obiba-alert>

  <div ng-hide="serverError">
    <div class="pull-right">
      <?php print l(t('Cancel'), MicaClientPathProvider::DATA_ACCESS_LIST, array(
          'attributes' => array(
            'class' => 'btn btn-default',
            'ng-if' => 'newRequest',
          ),
        )); ?>

      <a ng-if="!newRequest" ng-click="cancel()" type="button" class="btn btn-default">
        <?php print t('Cancel'); ?>
      </a>

      <a ng-click="save()" type="button" class="btn btn-primary">
        <?php print t('Save'); ?>
      </a>

      <a ng-click="validate()" type="button" class="btn btn-info">
        <?php print t('Validate'); ?>
      </a>
    </div>

    <div class="clearfix"></div>

    <img
      src="<?php print base_path() . drupal_get_path('theme', obiba_mica_commons_get_current_theme()) ?>/img/spin.gif"
      ng-if="loading">
    <form name="requestForm" ng-submit="submit(requestForm)">
      <div sf-model="form.model" sf-form="form.definition" sf-schema="form.schema" required="true"></div>
      <h2><?php print variable_get_value('access_documents_section_title'); ?></h2>
      <?php print variable_get_value('access_documents_section_help_text'); ?>
      <div class="row">
        <div class="col-md-6">
          <div class="form-group md-top-margin">
            <attachment-input files="dataAccessRequest.attachments" multiple="true"></attachment-input>
          </div>
        </div>
      </div>
    </form>

    <div class="lg-top-margin">
      <?php print l(t('Cancel'), MicaClientPathProvider::DATA_ACCESS_LIST, array(
        'attributes' => array(
          'class' => 'btn btn-default',
          'ng-if' => 'newRequest',
        ),
      )); ?>

      <a ng-if="!newRequest" ng-click="cancel()" type="button" class="btn btn-default">
        <?php print t('Cancel'); ?>
      </a>

      <a ng-click="save()" type="button" class="btn btn-primary">
        <?php print t('Save'); ?>
      </a>

      <a ng-click="validate()" type="button" class="btn btn-info">
        <?php print t('Validate'); ?>
      </a>
    </div>
  </div>

</div>
