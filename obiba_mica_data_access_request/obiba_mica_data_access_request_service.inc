<?php
/**
 * @file
 * Copyright (c) 2015 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Data access list page.
 */
function obiba_mica_data_access_request_list_page() {
  obiba_mica_data_access_requests_datatable_init();
  drupal_set_title(variable_get_value('access_requests_page_title'));
  return theme('obiba_mica_data_access_request-list');
}

/**
 * Get the data access request form config.
 */
function obiba_mica_data_access_request_form_get_config() {
  $mica_client = new MicaDataAccessRequest();
  $config = $mica_client->getFormDefinition();
  drupal_json_output($config);
}

/**
 * Actions on data access requests.
 */
function obiba_mica_data_access_requests_action() {
  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $mica_client = new MicaDataAccessRequests(NULL, 'METHOD_POST');
    $data_access_request = $mica_client->dataAccessSave(file_get_contents('php://input'), '/data-access-requests');
    drupal_json_output($data_access_request);
  }

}

/**
 * Actions on comments of data access request.
 */
function obiba_mica_data_access_request_comments_action($id_data_request) {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaDataAccessRequestComments(NULL, 'METHOD_GET');
      $data_access_request = $mica_client->getComments($id_data_request, '/data-access-request');
      drupal_json_output($data_access_request);
      break;

    case 'POST':
      $mica_client = new MicaDataAccessRequestComments(NULL, 'METHOD_POST');
      $data_access_request = $mica_client->createComment(file_get_contents('php://input'), $id_data_request, '/data-access-request');
      drupal_json_output($data_access_request);
      break;
  }

}

/**
 * Actions on comment of data access request.
 */
function obiba_mica_data_access_request_comment_action($id_data_request, $comment_id) {

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'PUT':
      $mica_client = new MicaDataAccessRequestComment(NULL, 'METHOD_PUT');
      $data_access_request = $mica_client->saveComment($id_data_request, $comment_id, file_get_contents('php://input'), '/data-access-request');
      drupal_json_output($data_access_request);
      break;

    case 'DELETE':
      $mica_client = new MicaDataAccessRequestComment(NULL, 'METHOD_DELETE');
      $data_access_request = $mica_client->deleteComment($id_data_request, $comment_id, '/data-access-request');
      drupal_json_output($data_access_request);
      break;
  }

}

/**
 * Get data access request attachments.
 */
function obiba_mica_data_access_request_attachment($id_data_request, $id_file) {
  $dataset_resource = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $responses = $dataset_resource->downloadAttachment($id_data_request, $id_file);
  if (!empty($responses)) {
    drupal_add_http_header('Content-Disposition', $responses['headers']['Content-Disposition'][0]);
    echo $responses['response']->body;
    flush();
    // Stop drupal execution error : 'Headers already sent'.
    die();
  }
}

/**
 * Download a pdf of the data access request.
 */
function obiba_mica_data_access_request_download_pdf($id_data_request) {
  if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    $dataset_resource = new MicaDataAccessRequest(NULL, 'METHOD_GET');
    $responses = $dataset_resource->downloadPdf($id_data_request);
    if (!empty($responses)) {
      drupal_add_http_header('Content-Disposition', $responses['headers']['Content-Disposition'][0]);
      echo $responses['response']->body;
      flush();
      // Stop drupal execution error : 'Headers already sent'.
      die();
    }
  }
}

/**
 * Actions on data access request.
 */
function obiba_mica_data_access_request_action($id_data_request) {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_GET');
      $data_access_request = $mica_client->getRequest($id_data_request);
      if (empty($data_access_request)) {
        return FALSE;
      }
      drupal_json_output($data_access_request);
      break;

    case 'PUT':
      $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_PUT');
      $data_access_request = $mica_client->dataAccessSave(file_get_contents('php://input'), '/data-access-request', $id_data_request);
      drupal_json_output($data_access_request);
      break;

    case 'DELETE':
      obiba_mica_data_access_request_delete_request_access($id_data_request);
      break;
  }
}

/**
 * Delete the data access request.
 */
function obiba_mica_data_access_request_delete_request_access($id_request) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_DELETE');
  $data_access_request = $mica_client->deleteRequest($id_request);
  drupal_json_output($data_access_request);
}

/**
 * Redirection callaBack on not permitted actions, detected in AngularJs app.
 */
function obiba_mica_data_access_request_redirect($operation, $id_request) {
  global $base_url;
  $url = $base_url . '/' . 'mica/data-access/request#/' . $operation . '/' . $id_request;
  header('Location: ' . $url, TRUE);
  drupal_exit($url);
}

/**
 * Update the data access request.
 */
function obiba_mica_data_access_request_update_request($id_request, $status) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_PUT');
  $data_access_request = $mica_client->statusUpdateRequest($id_request, $status);
  drupal_json_output($data_access_request);
}

/**
 * Update the data access request attachments.
 */
function obiba_mica_data_access_request_upload_attachment() {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_POST');
  $response = $mica_client->uploadFile($_FILES);
  drupal_json_output($response);
}

/**
 * GEt the temp data access request attachments.
 */
function obiba_mica_data_access_request_get_attachment($id_file) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $response = $mica_client->getFile($id_file, 'files/temp');
  drupal_json_output($response);
}
