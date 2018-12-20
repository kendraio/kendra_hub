<?php

/**
 * @file
 * Implementation of Feeds mapping API.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsProcessor::getMappingTargets()
 */
function range_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  $numeric_types = array(
    'range_integer',
    'range_decimal',
    'range_float',
  );
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], $numeric_types)) {
      $targets[$name . ':from'] = array(
        'name' => t('@name: From', array('@name' => $instance['label'])),
        'callback' => 'range_feeds_set_target',
        'description' => t('FROM value of the @label field of the entity.', array('@label' => $instance['label'])),
        'real_target' => $name,
      );
      $targets[$name . ':to'] = array(
        'name' => t('@name: To', array('@name' => $instance['label'])),
        'callback' => 'range_feeds_set_target',
        'description' => t('TO value of the @label field of the entity.', array('@label' => $instance['label'])),
        'real_target' => $name,
      );
    }
  }
}

/**
 * Callback for mapping numeric ranges.
 *
 * Ensure that $value is a numeric to avoid database errors.
 */
function range_feeds_set_target($source, $entity, $target, $value) {

  // Do not perform the regular empty() check here. 0 is a valid value. That's
  // really just a performance thing anyway.
  if (!is_array($value)) {
    $value = array($value);
  }

  // Iterate over all values.
  list($field_name, $column) = explode(':', $target);
  $info = field_info_field($field_name);

  $field = isset($entity->$field_name) ? $entity->$field_name : array(LANGUAGE_NONE => array());
  $delta = 0;

  foreach ($value as $v) {
    if ($info['cardinality'] == $delta) {
      break;
    }

    if (is_object($v) && ($v instanceof FeedsElement)) {
      $v = $v->getValue();
    }

    if (is_numeric($v)) {
      $field[LANGUAGE_NONE][$delta][$column] = $v;
      $delta++;
    }
  }

  $entity->$field_name = $field;
}
