<?php

/**
 * @file
 * Integration with Internationalization module.
 */

/**
 * Implements hook_i18n_string_list_TEXTGROUP_alter().
 */
function range_i18n_string_list_field_alter(&$strings, $type = NULL, $object = NULL) {
  if ($type === 'field_instance' && $object['widget']['type'] === 'range') {
    $element = &$strings['field'][$object['field_name']][$object['bundle']];


    foreach (_range_translatable_field_instance_settings() as $key => $title) {
      list($l1, $l2) = explode('_', $key);
      $element[$key] = array(
        'title' => $title,
        'string' => $object['settings'][$l1][$l2],
      );
    }
  }
}

/**
 * Translates given range field instance.
 *
 * This function translates the following field instance properties:
 *   - field label (by i18n_field module)
 *   - field description (by i18n_field module)
 *   - FROM value form element label
 *   - FROM value prefix and suffix
 *   - TO value form element label
 *   - TO value prefix and suffix
 *   - FIELD value prefix and suffix
 *   - COMBINED value prefix and suffix
 *
 * @param array $instance
 *   Field instance.
 *
 * @return array
 *   Translated field instance.
 */
function range_translate_field_instance(array $instance) {
  if (module_exists('i18n_field')) {
    foreach (_range_translatable_field_instance_settings() as $key => $title) {
      list($l1, $l2) = explode('_', $key);
      $instance['settings'][$l1][$l2] = i18n_string_translate("field:{$instance['field_name']}:{$instance['bundle']}:$key", $instance['settings'][$l1][$l2]);
    }
    $instance['label'] = i18n_string_translate("field:{$instance['field_name']}:{$instance['bundle']}:label", $instance['label']);
    $instance['description'] = i18n_string_translate("field:{$instance['field_name']}:{$instance['bundle']}:description", $instance['description']);
    return $instance;
  }
  else {
    return $instance;
  }
}

/**
 * Helper function. Returns translatable range field instance settings.
 *
 * @return array
 *   Array of translatable field instance settings keyed by property ID.
 */
function _range_translatable_field_instance_settings() {
  return array(
    'from_label' => t('FROM value: Form element label'),
    'from_prefix' => t('FROM value: Prefix'),
    'from_suffix' => t('FROM value: Suffix'),
    'to_label' => t('TO value: Form element label'),
    'to_prefix' => t('TO value: Prefix'),
    'to_suffix' => t('TO value: Suffix'),
    'field_prefix' => t('FIELD value: Prefix'),
    'field_suffix' => t('FIELD value: Suffix'),
    'combined_prefix' => t('COMBINED value: Prefix'),
    'combined_suffix' => t('COMBINED value: Suffix'),
  );
}
