<?php
/**
 * @file
 * Obiba Mica variable file
 */

/**
 * Implements hook_variable_info().
 */
function obiba_mica_variable_info($options) {
  _obiba_mica_variable_info_mica_server($variable);
  _obiba_mica_variable_info_access($variable);

  $variable['image_cache_time_out'] = array(
    'title' => t('The image cache timeout'),
    'description' => t('The timeout in minute to expire Studies and networks image cache'),
    'type' => 'boolean',
    'default' => 1440,
  );

  _obiba_mica_variable_search_page($variable);
  _obiba_mica_variable_search_networks($variable);
  _obiba_mica_variable_search_studies($variable);
  _obiba_mica_variable_search_datasets($variable);
  _obiba_mica_variable_search_variables($variable);

  _obiba_mica_variable_study_content($variable);
  _obiba_mica_variable_dataset_content($variable);
  _obiba_mica_variable_variable_content($variable);
  _obiba_mica_variable_statistics_content($variable);

  $variable['mica_taxonomy_figures'] = array(
    'title' => t('Taxonomy in figures filter'),
    'description' => t('List of taxonomy names (ordered, case sensitive, comma separated) that will be displayed in Variable Classification figures. If none is specified, all taxonomies (alphabetically ordered) will be displayed.'),
    'type' => 'string',
    'default' => '',
  );

  $variable['mica_taxonomy_order'] = array(
    'title' => t('Taxonomy order'),
    'description' => t('List of taxonomy names (case sensitive, comma separated, without blank spaces) determining their order of appearance in the search and classification UI.'),
    'type' => 'string',
    'default' => '',
  );

  $variable['mica_disable_translation'] = array(
    'title' => t('Disable translation'),
    'description' => t('Disable translation in selected pages.'),
    'type' => 'boolean',
    'default' => '',
  );

  $variable['mica_disabled_path_translation'] = array(
    'title' => t('Disable translation for paths'),
    'description' => t('List of page paths for which translation is disabled (separated by comma).'),
    'type' => 'boolean',
    'default' => '',
  );

  return $variable;
}

/**
 * Setting Variable search page.
 */
function _obiba_mica_variable_search_page(&$variable) {
  $variable['help_text_search_page'] = array(
    'title' => t('Search help text'),
    'description' => t('Help text to display on the search page.'),
    'type' => 'string',
    'default' => '<i class="glyphicon glyphicon-info-sign"></i> Search page allows users to browse all information catalogued, including partner networks, studies, datasets and variables.',
  );

  $variable['help_text_harmonization_potential_page'] = array(
    'title' => t('Classification help text'),
    'description' => t('Help text to display on the variable classification page'),
    'type' => 'string',
    'default' => '<i class="glyphicon glyphicon-info-sign"></i> The Repository search page allows users to browse all information catalogued, including partner networks, studies, datasets and variables.',
  );
}

/**
 * Setting Variable search page.
 */
function _obiba_mica_variable_info_mica_server(&$variable) {
  $variable['mica_url'] = array(
    'title' => t('Mica address'),
    'description' => t('URL of the Mica server. Example: https://mica.example.org:8445'),
    'type' => 'string',
    'default' => 'https://localhost:8445',
  );
  $variable['mica_anonymous_name'] = array(
    'title' => t('Anonymous user name'),
    'description' => t('The name under which an anonymous user is known by Mica.'),
    'type' => 'string',
    'default' => 'anonymous',
  );
  $variable['mica_anonymous_password'] = array(
    'title' => t('Anonymous user password'),
    'description' => t('The password used by the Drupal server when issuing requests to Mica.'),
    'type' => 'string',
    'default' => 'password',
  );
  $variable['mica_copyright_notice'] = array(
    'title' => t('Copyright notice text'),
    'description' => t('Copyright notice displayed in the site and downloaded documents.'),
    'type' => 'string',
    'default' => '',
  );
  $variable['mica_response_page_size'] = array(
    'title' => t('Number of items per server response page'),
    'description' => t('Determines the how many items that must be displayed in a server response page.'),
    'type' => 'number',
    'default' => 20,
  );
  $variable['mica_response_page_size_small'] = array(
    'title' => t('Minimum number of items per server response page'),
    'description' => t('Determines the minimum number of items that must to be displayed in a server response page.'),
    'type' => 'number',
    'default' => 10,
  );
}

/**
 * Setting Variable access page.
 */
function _obiba_mica_variable_info_access(&$variable) {
  $variable['access_home_page_title'] = array(
    'title' => t('Home Page title'),
    'description' => t('Data Access home page title.'),
    'type' => 'string',
    'default' => 'Data Access',
  );
  $variable['access_requests_page_title'] = array(
    'title' => t('Requests Page title'),
    'description' => t('Data Access requests page title.'),
    'type' => 'string',
    'default' => 'My Data Access Requests',
  );
  $variable['access_request_page_title'] = array(
    'title' => t('Request Page title'),
    'description' => t('Data Access request page title.'),
    'type' => 'string',
    'default' => 'Data Access Request',
  );
  $variable['access_my_requests_button'] = array(
    'title' => t('My requests button'),
    'description' => t('Data access my requests button caption.'),
    'type' => 'string',
    'default' => 'My Data Access Requests',
  );
  $variable['access_new_request_button'] = array(
    'title' => t('New request button'),
    'description' => t('Data access new request button caption.'),
    'type' => 'string',
    'default' => 'New Data Access Request',
  );
  $variable['access_documents_section_title'] = array(
    'title' => t('Documents section title'),
    'description' => t('Data access request "Documents" section title.'),
    'type' => 'string',
    'default' => 'Documents',
  );

  $variable['access_documents_section_help_text'] = array(
    'title' => t('Documents section help text'),
    'description' => t('Data access request "Documents" section help text.'),
    'type' => 'string',
    'default' => 'Select one or several documents to be attached to the application form.',
  );
  $variable['access_download_button'] = array(
    'title' => t('Download button caption'),
    'description' => t('Data access download button caption.'),
    'type' => 'string',
    'default' => 'Download PDF',
  );
  $variable['access_signup_button'] = array(
    'title' => t('Sign up button'),
    'description' => t('Data access sign up button caption.'),
    'type' => 'string',
    'default' => 'Sign Up',
  );
  $variable['access_signin_button'] = array(
    'title' => t('Sign in button'),
    'description' => t('Data access sign in button caption.'),
    'type' => 'string',
    'default' => 'Sign In',
  );
  $variable['access_comments_enabled'] = array(
    'title' => t('Enable commenting'),
    'description' => t('If checked, data access request commenting is enabled.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
}

/**
 * Setting Variable search page networks.
 */
function _obiba_mica_variable_search_networks(&$variable) {
  $variable['search_networks'] = array(
    'title' => t('Show networks search'),
    'description' => t('If selected, the networks tab will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['networks_column_studies'] = array(
    'title' => t('Show studies'),
    'description' => t('Show studies column in the networks table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['networks_column_study_datasets'] = array(
    'title' => t('Show studies dataset'),
    'description' => t('Show studies dataset column in the networks table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['networks_column_harmonization_datasets'] = array(
    'title' => t('Show harmonization dataset'),
    'description' => t('Show harmonization dataset column in the networks table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['networks_column_variables'] = array(
    'title' => t('Show variables'),
    'description' => t('Show variables column in the networks table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['networks_search_label'] = array(
    'title' => t('Networks search label'),
    'description' => t('Text to be displayed in links to network search.'),
    'type' => 'string',
    'default' => 'Search Networks',
  );
  $variable['networks_empty_label'] = array(
    'title' => t('No network label'),
    'description' => t('Text to be displayed when no network is found.'),
    'type' => 'string',
    'default' => 'No network found',
  );
}

/**
 * Setting Variable search page studies.
 */
function _obiba_mica_variable_search_studies(&$variable) {
  $variable['search_studies'] = array(
    'title' => t('Show studies search'),
    'description' => t('If selected, the studies tab will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_facet_search'] = array(
    'title' => t('Show studies search filter'),
    'description' => t('If selected, the studies search filters tab will be displayed.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_design'] = array(
    'title' => t('Show design'),
    'description' => t('Show design column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_questionnaire'] = array(
    'title' => t('Show questionnaires'),
    'description' => t('Show questionnaires column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_pm'] = array(
    'title' => t('Show physical measures'),
    'description' => t('Show physical measures column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_bio'] = array(
    'title' => t('Show biological samples'),
    'description' => t('Show biological samples column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_other'] = array(
    'title' => t('Show other data sources'),
    'description' => t('Show other data sources column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_participants'] = array(
    'title' => t('Show participants'),
    'description' => t('Show participants column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_networks'] = array(
    'title' => t('Show networks'),
    'description' => t('Show networks column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_study_datasets'] = array(
    'title' => t('Show study datasets'),
    'description' => t('Show study datasets column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_harmonization_datasets'] = array(
    'title' => t('Show harmonization datasets'),
    'description' => t('Show harmonization dataset column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_column_variables'] = array(
    'title' => t('Show variables'),
    'description' => t('Show variables column in the studies table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_search_label'] = array(
    'title' => t('Studies search label'),
    'description' => t('Text to be displayed in links to study search.'),
    'type' => 'string',
    'default' => 'Search Studies',
  );
  $variable['studies_empty_label'] = array(
    'title' => t('No study label'),
    'description' => t('Text to be displayed when no study is found.'),
    'type' => 'string',
    'default' => 'No study found',
  );
  $variable['search_studies_dce_autocomplete'] = array(
    'title' => t('Show Data Collection Event auto-complete search filter'),
    'description' => t('If selected, the auto-complete search filter will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['search_general_population_sources_autocomplete'] = array(
    'title' => t('Show General Population Sources search filter'),
    'description' => t('If selected, the search filter will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
}

/**
 * Setting Variable content page study.
 */
function _obiba_mica_variable_study_content(&$variable) {
  $variable['study_title_acronym_display'] = array(
    'title' => t('Show acronym/title study order display'),
    'description' => t('Select the order of acronym,  title that will be displayed in the title study page'),
    'type' => 'string',
    'default' => TRUE,
  );
  $variable['study_show_networks'] = array(
    'title' => t('Show networks'),
    'description' => t('If selected the Networks section will be displayed'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['study_show_datasets'] = array(
    'title' => t('Show datasets'),
    'description' => t('If selected the Datasets section will be displayed'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['study_show_coverage_graphs'] = array(
    'title' => t('Show classification graphics'),
    'description' => t('If selected the variable classification graphics will be displayed'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_list_show_search_form'] = array(
    'title' => t('Show the rapid search/sort form in list studies page'),
    'description' => t('If selected the the rapid search/sort form will be displayed in list studies page'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_list_show_search_button'] = array(
    'title' => t('Show the search studies button  in list studies page'),
    'description' => t('If selected the search studies button will be displayed in list studies page'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_list_show_study_sup_info'] = array(
    'title' => t('Show supplementary details study in list studies page'),
    'description' => t('If selected the supplementary details study badges will be displayed in list studies page'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['studies_list_show_trimmed_description_study'] = array(
    'title' => t("Show trimmed description details study, with 'Read more' link in list studies page"),
    'description' => t("If selected the description details study will be displayed in list studies page with a 'Read more' link"),
    'type' => 'boolean',
    'default' => TRUE,
  );

}

/**
 * Setting Variable content page dataset.
 */
function _obiba_mica_variable_dataset_content(&$variable) {

  $variable['dataset_title_acronym_setup'] = array(
    'title' => t('Show acronym on the title'),
    'description' => t('If selected, the acronym is shown in the title page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['dataset_show_studies'] = array(
    'title' => t('Show studies'),
    'description' => t('If selected the Studies section will be displayed'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['dataset_description_overview_field'] = array(
    'title' => t('Show description inside overview'),
    'description' => t('If selected, dataset description is shown as a plain text inside the overview section.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['dataset_harmonization_impossible_icon'] = array(
    'title' => t("Icon to show for the 'Impossible' status"),
    'description' => t("Select,the Icon to show for 'Impossible' harmonization status"),
    'type' => 'string',
    'default' => ObibaDatasetConstants::IMPOSSIBLE_HARMO_ICON,
  );
  $variable['dataset_harmonization_impossible_label'] = array(
    'title' => t("Label to show for the 'Impossible' status"),
    'description' => t("Text to display, for the label to show for 'Impossible' harmonization status"),
    'type' => 'string',
    'default' => 'Impossible',
  );
  $variable['dataset_harmonization_undetermined_icon'] = array(
    'title' => t("Icon to show for the 'Undetermined' status"),
    'description' => t("Select,the Icon to show for 'Undetermined' harmonization status"),
    'type' => 'string',
    'default' => ObibaDatasetConstants::UNDETERMINED_HARMO_ICON,
  );
  $variable['dataset_harmonization_undetermined_label'] = array(
    'title' => t("Label to show for the 'Undetermined' status"),
    'description' => t("Text to display, for the label to show for 'Undetermined' harmonization status"),
    'type' => 'string',
    'default' => 'Undetermined',
  );
}

/**
 * Setting Variable content page.
 */
function _obiba_mica_variable_variable_content(&$variable) {
  $variable['variable_show_studies'] = array(
    'title' => t('Show studies'),
    'description' => t('If selected the Studies section will be displayed'),
    'type' => 'string',
    'default' => TRUE,
  );
  $variable['dataset_detailed_var_stats'] = array(
    'title' => t('Show detailed harmonization variable data statistics'),
    'description' => t('If selected, detailed data summary and crosstab statistics about each harmonized variables will be displayed when querying the harmonization variable data statistics.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['mica_statistics'] = array(
    'title' => t('Show data statistics'),
    'description' => t('If selected, variable pages will include a statistics summary section and a link to the crosstab page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
}

/**
 * Setting Variable statistic content.
 */
function _obiba_mica_variable_statistics_content(&$variable) {

  $variable['mica_statistics_coverage'] = array(
    'title' => t('Show variable classification'),
    'description' => t('If selected, a link to the classification page will be available from different pages (network, study, search etc.).'),
    'type' => 'boolean',
    'default' => TRUE,
  );
}

/**
 * Setting Variable search page datasets.
 */
function _obiba_mica_variable_search_datasets(&$variable) {
  $variable['search_datasets'] = array(
    'title' => t('Show datasets search'),
    'description' => t('If selected, the datasets tab will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['datasets_column_type'] = array(
    'title' => t('Show type dataset'),
    'description' => t('Show type dataset column in the datasets table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['datasets_column_networks'] = array(
    'title' => t('Show networks'),
    'description' => t('Show networks column in the datasets table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['datasets_column_studies'] = array(
    'title' => t('Show studies'),
    'description' => t('Show studies column in the datasets table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['datasets_column_variables'] = array(
    'title' => t('Show variables'),
    'description' => t('Show variables column in the datasets table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['datasets_search_label'] = array(
    'title' => t('Datasets search label'),
    'description' => t('Text to be displayed in links to dataset search.'),
    'type' => 'string',
    'default' => 'Search Datasets',
  );
  $variable['datasets_empty_label'] = array(
    'title' => t('No dataset label'),
    'description' => t('Text to be displayed when no dataset is found.'),
    'type' => 'string',
    'default' => 'No dataset found',
  );
  $variable['search_datasets_autocomplete'] = array(
    'title' => t('Show dataset auto-complete search filter'),
    'description' => t('If selected, the auto-complete search filter will be displayed in the search page.'),
    'type' => 'number',
    'default' => ObibaDatasetConstants::FACET_TYPE_AUTOCOMPLETE,
  );
}

/**
 * Setting Variable search page variables.
 */
function _obiba_mica_variable_search_variables(&$variable) {
  $variable['search_variables'] = array(
    'title' => t('Show variables search'),
    'description' => t('If selected, the variables tab will be displayed in the search page.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['variables_column_study'] = array(
    'title' => t('Show study'),
    'description' => t('Show study column in the variables table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['variables_column_dataset'] = array(
    'title' => t('Show dataset'),
    'description' => t('Show dataset column in the variables table.'),
    'type' => 'boolean',
    'default' => TRUE,
  );
  $variable['variable_coverage_label'] = array(
    'title' => t('Variable classification label'),
    'description' => t('Text to be displayed in links to variable classification.'),
    'type' => 'string',
    'default' => 'Variable Classification',
  );
  $variable['variables_search_label'] = array(
    'title' => t('Variables search label'),
    'description' => t('Text to be displayed in links to variable search.'),
    'type' => 'string',
    'default' => 'Search Variables',
  );
  $variable['variables_empty_label'] = array(
    'title' => t('No variable label'),
    'description' => t('Text to be displayed when no variable is found.'),
    'type' => 'string',
    'default' => 'No variable found',
  );
}
