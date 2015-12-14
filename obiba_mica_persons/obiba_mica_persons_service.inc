<?php
/**
 * @file
 * Mica Persons module file
 */

/**
 * Get the person list by studies id (id1+id2+id3...).
 *
 * @param string $entity_id
 * @param string $entity_query
 */
function obiba_mica_persons_list($entities_id) {
  $entities_id = json_decode($entities_id);
  $query_person = empty($entities_id->studiesId) ? '' : "studyMemberships.parentId:(" . implode('+', $entities_id->studiesId) . ")";
  if (!empty($entities_id->networksId)) {
    if (!empty($query_person)) {
      $query_person .= '+OR+';
    }
    $query_person .= "networkMemberships.parentId:(" . implode('+', $entities_id->networksId) . ")";
  }
  $persons_resources = new MicaPersonsResource();
  $persons = $persons_resources->search($query_person);
  drupal_json_output(array(
    "recordsTotal" => $persons->total,
    "data" => $persons->persons
  ));
}

/**
 * Download a csv file of the person list by studies id (id1+id2+id3...).
 *
 * @param string $studies_id
 */
function obiba_mica_persons_list_csv_download($entities_id) {
  $entities_id = json_decode($entities_id);
  $query_person = empty($entities_id->studiesId) ? '' : "studyMemberships.parentId:(" . implode('+', $entities_id->studiesId) . ")";
  if (!empty($entities_id->networksId)) {
    if (!empty($query_person)) {
      $query_person .= '+OR+';
    }
    $query_person .= "networkMemberships.parentId:(" . implode('+', $entities_id->networksId) . ")";
  }
  $persons_resources = new MicaPersonsResource();
  $response = $persons_resources->download($query_person);
  if (!empty($response)) {
    $headers = $persons_resources->getLastResponseHeaders();
    drupal_add_http_header('Content-Disposition', $headers['Content-Disposition'][0]);
    ob_clean();
    $csv_response = $response->body;
    print $csv_response;
    exit;
  }
}
