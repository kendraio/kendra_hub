<?php

/**
 * @file
 * Provide an admin UI for the Admin Toggle module.
 */

/**
 * Provide a settings form.
 */
function admin_toggle_admin_form($form, &$form_state) {
  $form['admin_toggle_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Shortcut key'),
    '#description' => t('Type the key that will be used to toggle the visibility of admin items.'),
    '#maxlength' => 1,
    '#size' => 1,
    '#default_value' => variable_get('admin_toggle_key', '`'),
    '#required' => TRUE,
  );

  $form['admin_toggle_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default toggle'),
    '#description' => t('Choose whether to toggle admin items on or off by default.'),
    '#options' => array(
      1 => t('On'),
      0 => t('Off'),
    ),
    '#default_value' => variable_get('admin_toggle_default', 1),
  );

  $form['admin_toggle_custom_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom selector(s)'),
    '#description' => t('Enter comma-separated selectors for additional admin items to toggle when the shortcut key is pressed.'),
    '#default_value' => variable_get('admin_toggle_custom_selector'),
  );

  return system_settings_form($form);
}
