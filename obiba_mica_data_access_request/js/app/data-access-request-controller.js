/*
 * Copyright (c) 2014 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

(function ($) {
  Drupal.behaviors.obiba_mica_data_access_request_controller = {

    attach: function (context, settings) {

      'use strict';
      var user = Drupal.settings.angularjsApp.user;
      mica.DataAccessRequest
        .controller('DataAccessRequestViewController', ['$rootScope', '$scope', '$routeParams', '$filter', '$modal',
          'DataAccessRequestResource',
          'DataAccessRequestService',
          'DataAccessRequestStatusResource',
          'DataAccessFormResource',
          'DataAccessRequestCommentsResource',
          'DataAccessRequestCommentResource',
          'AlertService',
          'ServerErrorAlertService',
          'ForbiddenDrupalRedirect',
          'NOTIFICATION_EVENTS',

          function ($rootScope, $scope, $routeParams, $filter, $modal,
                    DataAccessRequestResource,
                    DataAccessRequestService,
                    DataAccessRequestStatusResource,
                    DataAccessFormResource,
                    DataAccessRequestCommentsResource,
                    DataAccessRequestCommentResource,
                    AlertService,
                    ServerErrorAlertService,
                    ForbiddenDrupalRedirect,
                    NOTIFICATION_EVENTS) {
            $.ObibaProgressBarController().start();
            $.ObibaProgressBarController().setPercentage(65, true);
            var onError = function (response) {
              $scope.serverError = true;
              ServerErrorAlertService.alert('DataAccessRequestViewController', response);
              ForbiddenDrupalRedirect.redirectDrupalMessage(response);
              $.ObibaProgressBarController().finish();
            };

            var selectTab = function (id) {
              $scope.selectedTab = id;
              switch (id) {
                case 'form':
                  break;

                case 'comments':
                  retrieveComments();
                  break;
              }
            };
            var retrieveComments = function () {
              $scope.form.comments = DataAccessRequestCommentsResource.get({id: $routeParams.id});
            };

            var submitComment = function (comment) {
              DataAccessRequestCommentsResource.save({id: $routeParams.id}, comment.message, retrieveComments, onError);
            };

            var updateComment = function (comment) {
              DataAccessRequestCommentResource.update({id: $routeParams.id, commentId: comment.id}, comment.message, retrieveComments, onError);
            };

            var deleteComment = function (comment) {
              $scope.commentToDelete = comment.id;
              $rootScope.$broadcast(NOTIFICATION_EVENTS.showConfirmDialog,
                {
                  titleKey: 'comment.delete-dialog.title',
                  messageKey: 'comment.delete-dialog.message',
                  messageArgs: [comment.createdBy]
                }, comment.id
              );
            };

            $scope.$on(NOTIFICATION_EVENTS.confirmDialogAccepted, function (event, id) {
              if ($scope.commentToDelete === id) {
                DataAccessRequestCommentResource.delete({id: $routeParams.id, commentId: id}, {}, retrieveComments, onError);
              }
            });

            $scope.form = {
              schema: {},
              definition: {},
              model: {},
              comments: null
            };

            $scope.getDownloadHref = function (attachments, id) {
              return 'request/' + $scope.dataAccessRequest.id + '/attachments/' + id + '/_download/ws';
            };

            $scope.serverError = false;
            $scope.actions = DataAccessRequestService.actions;
            $scope.nextStatus = DataAccessRequestService.nextStatus;
            $scope.selectTab = selectTab;
            $scope.submitComment = submitComment;
            $scope.updateComment = updateComment;
            $scope.deleteComment = deleteComment;
            $scope.getStatusHistoryInfoId = DataAccessRequestService.getStatusHistoryInfoId;
            $scope.getStatusHistoryInfo = DataAccessRequestService.getStatusHistoryInfo();
            var getRequest = function () {
              return DataAccessRequestResource.get({id: $routeParams.id}, function onSuccess(request) {
                try {
                  $scope.form.model = request.content ? JSON.parse(request.content) : {};
                } catch (e) {
                  $scope.serverError = true;
                  $scope.form.model = {};
                  AlertService.alert({
                    id: 'DataAccessRequestViewController',
                    type: 'danger',
                    msgKey: 'data-access-request.parse-error'
                  });
                }

                // Retrieve form data
                DataAccessFormResource.get(
                  function onSuccess(dataAccessForm) {

                    $scope.requestId = request.id;
                    $scope.form.definition = DataAccessRequestService.parseJsonSafely(dataAccessForm.definition, []);
                    $scope.form.schema = DataAccessRequestService.parseJsonSafely(dataAccessForm.schema, {});

                    if ($scope.form.definition.length === 0) {
                      $scope.serverError = true;
                      $scope.form.definition = [];
                      AlertService.alert({
                        id: 'DataAccessRequestViewController',
                        type: 'danger',
                        msgKey: 'data-access-config.parse-error.definition'
                      });
                    }
                    if (Object.getOwnPropertyNames($scope.form.schema).length === 0) {
                      $scope.serverError = true;
                      $scope.form.schema = {readonly: true};
                      AlertService.alert({
                        id: 'DataAccessRequestViewController',
                        type: 'danger',
                        msgKey: 'data-access-config.parse-error.schema'
                      });
                    }

                    $scope.form.schema.readonly = true;
                    $("a.download-btn").on('click', function (event) {
                      // create a form for the file upload
                      var form = $("<form action='request/" + request.id + '/_pdf/ws' + "' method='get'>");
                      $(this).after(form);
                      form.submit().remove();
                      return false;
                    });
                    angular.element("attachment-list").find('a').each(function () {
                      $(this).on('click', function (event) {
                        // create a form for the file upload
                        var form = $("<form action='" + $(this).attr('href') + "' method='get'>");
                        $(this).after(form);
                        form.submit().remove();
                        return false;
                      });
                    });

                    // $scope.$broadcast('schemaFormRedraw');

                    $scope.$on('sf-render-finished', function () {
                      $.ObibaProgressBarController().finish();
                      $scope.loading = false;
                    });
                  },
                  onError
                );

                request.attachments = request.attachments || [];

                return request;
              }, onError);
            };

            $scope.dataAccessRequest = $routeParams.id ? getRequest() : {};

            $scope.delete = function () {
              $scope.requestToDelete = $scope.dataAccessRequest.id;
              var title = ($scope.dataAccessRequest.title) ? $scope.dataAccessRequest.title : $scope.requestToDelete;
              $rootScope.$broadcast(NOTIFICATION_EVENTS.showConfirmDialog,
                {
                  titleKey: 'data-access-request.delete-dialog.title',
                  messageKey: 'data-access-request.delete-dialog.message',
                  messageArgs: [title, $scope.dataAccessRequest.applicant]
                }, $scope.requestToDelete
              );
            };
            $scope.$on(NOTIFICATION_EVENTS.confirmDialogAccepted, function (event, id) {
              if ($scope.requestToDelete === id) {
                DataAccessRequestResource.delete({id: $scope.requestToDelete},
                  function () {
                    window.location = Drupal.settings.basePath + Drupal.settings.angularjsApp.requests_url;
                  });

                delete $scope.requestToDelete;
              }
            });

            var onUpdatStatusSuccess = function () {
              $scope.dataAccessRequest = getRequest();
            };

            var confirmStatusChange = function(status, messageKey, statusName) {
              $rootScope.$broadcast(
                NOTIFICATION_EVENTS.showConfirmDialog,
                {
                  titleKey: 'data-access-request.status-change-confirmation.title',
                  messageKey: messageKey !== null ? messageKey : 'data-access-request.status-change-confirmation.message',
                  messageArgs: statusName !== null ? [$filter('translate')(statusName).toLowerCase()] : []
                }, status);
            };

            var statusChangedConfirmed = function(status, expectedStatus) {
              if (status === expectedStatus) {
                DataAccessRequestStatusResource.update({
                  id: $scope.dataAccessRequest.id,
                  status: status
                }, onUpdatStatusSuccess, onError);
              }
            };

            $scope.submit = function () {
              $scope.$broadcast('schemaFormValidate');
              if ($scope.forms.requestForm.$valid) {
                DataAccessRequestStatusResource.update({
                  id: $scope.dataAccessRequest.id,
                  status: "SUBMITTED"
                }, function onSubmitted() {
                  $modal.open({
                    scope: $scope,
                    templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_data_access_request/data-access-request-submitted-modal'
                  });
                  onUpdatStatusSuccess();
                }, onError);
              } else {
                AlertService.alert({
                  id: 'DataAccessRequestViewController',
                  type: 'danger',
                  msgKey: 'data-access-request.submit.invalid'
                });
              }
            };

            $scope.reopen = function () {
              confirmStatusChange(DataAccessRequestService.status.OPENED, null, 'reopen');
            };
            $scope.review = function () {
              confirmStatusChange(DataAccessRequestService.status.REVIEWED, 'data-access-request.status-change-confirmation.message-review', null);
            };
            $scope.approve = function () {
              confirmStatusChange(DataAccessRequestService.status.APPROVED, null, 'approve');
            };
            $scope.reject = function () {
              confirmStatusChange(DataAccessRequestService.status.REJECTED, null, 'reject');
            };

            $scope.$on(
              NOTIFICATION_EVENTS.confirmDialogAccepted,
              function(event, status) {
                statusChangedConfirmed(DataAccessRequestService.status.OPENED, status);
              }
            );
            $scope.$on(
              NOTIFICATION_EVENTS.confirmDialogAccepted,
              function(event, status) {
                statusChangedConfirmed(DataAccessRequestService.status.REVIEWED, status);
              }
            );
            $scope.$on(
              NOTIFICATION_EVENTS.confirmDialogAccepted,
              function(event, status) {
                statusChangedConfirmed(DataAccessRequestService.status.APPROVED, status);
              }
            );
            $scope.$on(
              NOTIFICATION_EVENTS.confirmDialogAccepted,
              function(event, status) {
                statusChangedConfirmed(DataAccessRequestService.status.REJECTED, status);
              }
            );

            var getAttributeValue = function(attributes, key) {
              var result = attributes.filter(function (attribute) {
                return attribute.key === key;
              });

              return result && result.length > 0 ? result[0].value : null;
            };

            $scope.getFullName = function (profile) {
              if (profile) {
                if (profile.attributes) {
                  return getAttributeValue(profile.attributes, 'firstName') + ' ' + getAttributeValue(profile.attributes, 'lastName');
                }
                return profile.username;
              }
              return null;
            };

            $scope.loading = true;
            $scope.forms = {};

          }])

        .controller('DataAccessRequestEditController', ['$log', '$scope', '$routeParams', '$location', '$modal',
          'DataAccessRequestsResource',
          'DataAccessRequestResource',
          'DataAccessFormResource',
          'AlertService',
          'ServerErrorAlertService',
          'ForbiddenDrupalRedirect',
          'DataAccessRequestService',

          function ($log, $scope, $routeParams, $location, $modal,
                    DataAccessRequestsResource,
                    DataAccessRequestResource,
                    DataAccessFormResource,
                    AlertService,
                    ServerErrorAlertService,
                    ForbiddenDrupalRedirect,
                    DataAccessRequestService) {
            $.ObibaProgressBarController().start();
            $.ObibaProgressBarController().setPercentage(65, true);
            var onError = function (response) {
              $scope.serverError = true;
              ServerErrorAlertService.alert('DataAccessRequestEditController', response);
              ForbiddenDrupalRedirect.redirectDrupalMessage(response);
              $.ObibaProgressBarController().finish();
            };

            var validate = function () {
              $scope.$broadcast('schemaFormValidate');

              $modal.open({
                scope: $scope,
                templateUrl: Drupal.settings.basePath + 'obiba_mica_app_angular/obiba_mica_data_access_request/data-access-request-validation-modal'
              });
            };

            var cancel = function () {
              $location.path('/view' + ($scope.dataAccessRequest.id ? '/' + $scope.dataAccessRequest.id : 's')).replace();
            };

            var save = function () {
              $scope.dataAccessRequest.content = JSON.stringify($scope.form.model);

              if ($scope.newRequest) {
                DataAccessRequestsResource.save($scope.dataAccessRequest, function (response) {
                  if (response && response.location) {
                    var parts = response.location.split('/');
                    $location.path('/view/' + parts[parts.length - 1]).replace();
                  } else {
                    window.location = Drupal.settings.basePath + Drupal.settings.angularjsApp.requests_url;
                  }

                }, onError);
              } else {
                DataAccessRequestResource.save($scope.dataAccessRequest, function () {
                  $location.path('/' + ($scope.dataAccessRequest.id ? 'view/' + $scope.dataAccessRequest.id : 'requests')).replace();
                }, onError);
              }
            };

            // Retrieve form data
            DataAccessFormResource.get(
              function onSuccess(dataAccessForm) {

                if (dataAccessForm.definition) {
                  $scope.form.definition = DataAccessRequestService.parseJsonSafely(dataAccessForm.definition, []);
                  $scope.form.schema = DataAccessRequestService.parseJsonSafely(dataAccessForm.schema, {});

                  if ($scope.form.definition.length === 0) {
                    $scope.serverError = true;
                    $scope.form.definition = [];
                    AlertService.alert({
                      id: 'DataAccessRequestEditController',
                      type: 'danger',
                      msgKey: 'data-access-config.parse-error.definition'
                    });
                  }
                  if (Object.getOwnPropertyNames($scope.form.schema).length === 0) {
                    $scope.serverError = true;
                    $scope.form.schema = {readonly: true};
                    AlertService.alert({
                      id: 'DataAccessRequestEditController',
                      type: 'danger',
                      msgKey: 'data-access-config.parse-error.schema'
                    });
                  }

                  if (!$scope.serverError) {
                    $scope.dataAccessRequest = $routeParams.id ?
                      DataAccessRequestResource.get({id: $routeParams.id}, function onSuccess(request) {
                        try {
                          $scope.form.model = request.content ? JSON.parse(request.content) : {};
                        } catch (e) {
                          $scope.form.model = {};
                          AlertService.alert({
                            id: 'DataAccessRequestEditController',
                            type: 'danger',
                            msgKey: 'data-access-request.parse-error'
                          });
                        }

                        $scope.canEdit = DataAccessRequestService.actions.canEdit(request);
                        $scope.form.schema.readonly = !$scope.canEdit;
                        $scope.$broadcast('schemaFormRedraw');
                        request.attachments = request.attachments || [];
                        return request;
                      }, onError) : {
                      applicant: user.name,
                      status: DataAccessRequestService.status.OPENED,
                      attachments: []
                    };
                  }
                }
                else {
                  var errorMessage = {
                    status: '',
                    statusText: dataAccessForm.message
                  };
                  onError(errorMessage);
                }
              },
              onError
            );

            $scope.form = {
              schema: {},
              definition: {},
              model: {}
            };
            $scope.$on('sf-render-finished', function () {
              $.ObibaProgressBarController().finish();
              $scope.loading = false;
            });
            $scope.loading = true;
            $scope.serverError = false;
            $scope.requestId = $routeParams.id;
            $scope.newRequest = $routeParams.id ? false : true;
            $scope.cancel = cancel;
            $scope.save = save;
            $scope.editable = true;
            $scope.validate = validate;

          }]);

    }
  }
}(jQuery));
