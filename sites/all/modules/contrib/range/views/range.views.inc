<?php

/**
 * @file
 * Implementation of Views API.
 */

/**
 * Implements hook_views_data().
 */
function range_views_data() {
  $data = array();

  foreach (field_info_fields() as $field) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      continue;
    }
    if ($field['module'] != 'range') {
      continue;
    }

    list($label) = field_views_field_label($field['field_name']);

    $current_table = _field_sql_storage_tablename($field);
    $revision_table = _field_sql_storage_revision_tablename($field);

    $add_fields = array();
    foreach ($field['columns'] as $column_name => $attributes) {
      $add_fields[$column_name] = _field_sql_storage_columnname($field['field_name'], $column_name);
    }
    $base = array(
      'table' => $current_table,
      'handler' => 'range_views_handler_filter_range',
      'field_name' => $field['field_name'],
      'allow empty' => FALSE,
      'additional fields' => $add_fields,
    );

    $data[$current_table][$field['field_name']]['filter'] = array(
      'title' => t('@label range filter', array('@label' => $label)),
    ) + $base;
    $data[$revision_table][$field['field_name'] . '-' . 'revision_id']['filter'] = array(
      'title' => t('@label range filter (historical data)', array('@label' => $label)),
    ) + $base;
  }

  return $data;
}
