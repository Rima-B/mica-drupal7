<?php
/**
 * @file
 * Hooks provided by Mica client
 */

/**
 * A hook implementation.
 *
 * Given a harmonization csv file, this hook can alter the content as required.
 *
 * @param array $csv
 *   The csv array.
 *
 * @return array
 *   Can be the csv array modified on the implemented hook
 */
function hook_harmonization_csv_alter($csv) {
  return $csv;
}
