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
    <?php print variable_get_value('access_request_page_title'); ?>:
    {{dataAccessRequest.id}}
  </h2>

  <obiba-alert id="DataAccessRequestViewController"></obiba-alert>

  <div ng-hide="serverError">
    <p class="help-block pull-left">
      <span><?php print t('Created by'); ?></span>
      {{getFullName(dataAccessRequest.profile) || dataAccessRequest.applicant}},
      <span>{{dataAccessRequest.timestamps.created | amCalendar}}</span>
      <span class="label label-success">{{dataAccessRequest.status | translate | uppercase}}</span>
    </p>

    <div class="pull-right">
      <a ng-click="submit()"
        ng-if="actions.canEditStatus(dataAccessRequest) && nextStatus.canSubmit(dataAccessRequest)"
        class="btn btn-info"><?php print t('Submit'); ?>

      </a>
      <a ng-click="reopen()"
        ng-if="actions.canEditStatus(dataAccessRequest) && nextStatus.canReopen(dataAccessRequest)"
        class="btn btn-info"><?php print t('Reopen'); ?>
      </a>
      <a ng-click="review()"
        ng-if="actions.canEditStatus(dataAccessRequest) && nextStatus.canReview(dataAccessRequest)"
        class="btn btn-info"><?php print t('Review'); ?>
      </a>
      <a ng-click="approve()"
        ng-if="actions.canEditStatus(dataAccessRequest) && nextStatus.canApprove(dataAccessRequest)"
        class="btn btn-info"><?php print t('Approve'); ?>
      </a>
      <a ng-click="reject()"
        ng-if="actions.canEditStatus(dataAccessRequest) && nextStatus.canReject(dataAccessRequest)"
        class="btn btn-info"><?php print t('Reject'); ?>
      </a>
      <a ng-href="#/edit/{{dataAccessRequest.id}}"
         ng-if="actions.canEdit(dataAccessRequest)"
         class="btn btn-primary">
        <i class="glyphicon glyphicon-edit"></i>
        <?php print t('Edit'); ?>
      </a>
      <a target="_self" ng-href="" class="btn btn-default download-btn">
        <i class="glyphicon glyphicon-download-alt"></i> <span><?php print variable_get_value('access_download_button'); ?></span>
      </a>
      <a ng-click="delete()"
        ng-if="actions.canDelete(dataAccessRequest)"
        class="btn btn-danger" title="<?php print t('Delete'); ?>
        ">
        <i class="glyphicon glyphicon-trash"></i>
      </a>
    </div>

    <div class="clearfix"></div>

    <tabset class="lg-top-margin">
      <div class="pull-right">
      </div>
      <tab heading="<?php print t('Application form'); ?>
      ">
        <img
          src="<?php print base_path() . drupal_get_path('theme', obiba_mica_commons_get_current_theme()) ?>/img/spin.gif"
          ng-if="loading" class="lg-top-margin">
        <form id="request-form" name="forms.requestForm">
          <div sf-model="form.model" sf-form="form.definition"
            sf-schema="form.schema"></div>
        </form>
        <h2><?php print variable_get_value('access_documents_section_title'); ?></h2>

        <p ng-if="dataAccessRequest.attachments.length == 0">
          <?php print t('No documents provided.'); ?>
        </p>

        <div class="row">
          <div class="col-md-6">
            <attachment-list files="dataAccessRequest.attachments"
              href-builder="getDownloadHref(attachments, id)"></attachment-list>
          </div>
        </div>
      </tab>
      <tab ng-if="<?php print variable_get_value('access_comments_enabled') ? 'true' : 'false'?>" ng-click="selectTab('comments')"
        heading="<?php print t('Comments'); ?>">
        <obiba-comments comments="form.comments" on-update="updateComment"
          on-delete="deleteComment" edit-action="EDIT"
          delete-action="DELETE"></obiba-comments>
        <obiba-comment-editor on-submit="submitComment"
          class="md-top-margin"></obiba-comment-editor>
      </tab>
      <tab heading="<?php print t('History'); ?>">
        <div
          ng-include="'<?php ;
          print base_path(); ?>obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_data_access_request-histroy-view'"></div>
      </tab>

    </tabset>
  </div>

</div>
