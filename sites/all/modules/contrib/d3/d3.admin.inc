<?php
/**
 * @file
 * D3 admin file
 */

/**
 * D3 configuration form
 */
function d3_config_form($form, &$form_state) {

  $libs_from_libraries = _d3_list_library_versions_from_libraries();
  $libs_from_cdn = _d3_list_library_versions_from_cdn();

  $lib_options = array();
  if (count($libs_from_libraries) > 0) {
    $lib_options['lib'] = 'Libraries';
  }
  $lib_options['cdn'] = 'CDN';

  $form['d3_library_source'] = array(
    '#title' => 'D3 library source',
    '#description' =>
      'Get the d3 source library from CDN, or from the libraries module',
    '#type' => 'select',
    '#options' => $lib_options,
    '#default_value' => variable_get('d3_library_source', reset($lib_options)),
  );

  $library_versions =
    array_unique(array_merge($libs_from_cdn, $libs_from_libraries));
  ksort($library_versions, SORT_NUMERIC);
  $library_versions = array_combine($library_versions, $library_versions);

  $form['d3_library_version'] = array(
    '#title' => 'D3 Library version',
    '#type' => 'select',
    '#options' => $library_versions,
    '#default_value' =>
      variable_get('d3_library_version', min($library_versions)),
  );

  $form['#submit'][] = 'd3_config_form_submit';

  // Use inline javascript to set d3_library_versions to available versions
  $form['#attached']['js'][] =
    drupal_get_path('module', 'd3') . '/js/d3.admin.js';

  $d3_library_versions = array(
    'lib' => $libs_from_libraries,
    'cdn' => $libs_from_cdn
  );

  $js = "var d3_library_versions = " . json_encode($d3_library_versions) . ";";

  $form['#attached']['js'][] = array(
    'type' => 'inline',
    'data' => $js
  );

  return system_settings_form($form);
}

/**
 * Form submit callback for d3_config_form
 */
function d3_config_form_submit(&$form, &$form_state) {
  foreach (array('libraries_get_path', 'libraries_info') as $name) {
    drupal_static_reset($name);
  }
  cache_clear_all('*', 'cache_libraries', TRUE);
}
