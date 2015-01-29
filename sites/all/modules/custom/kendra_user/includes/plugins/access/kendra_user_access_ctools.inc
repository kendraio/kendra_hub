<?php
/**
 * @file
 * Plugin to provide access control
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Kendra Access Asset'),
  'description' => t('Control access according Asset or its related types.'),
  'callback' => 'kendra_user_ctools_access_check',
  'default' => array('name' => ''),
  'settings form' => 'kendra_user_access_ctools_access_settings',
  'settings form submit' => 'kendra_user_access_ctools_access_settings_submit',
  'summary' => 'kendra_user_ctools_access_summary',
  'all contexts' => TRUE,
);

/**
 * Settings form for the path_access access plugin
 */
function kendra_user_access_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['name'] = array(
    '#title' => t('name'),
    '#type' => 'textfield',
    '#description' => t('name'),
    '#default_value' => $conf['name'],
  );
  return $form;
}

/**
 * Validation for the settings form for the access plugin
 */
function kendra_user_access_ctools_access_settings_validate($form, &$form_state, $conf) {
  if (url_is_external($form_state['values']['name'])) {
    form_set_error('name', t('Please supply aname'));
  }
}

/**
 * Check for access.
 */
function kendra_user_ctools_access_check($conf, $context) {
  global $user;
  return kendra_user_can_specified_user_edit_asset($user);
}


/**
 * Provide a summary description based upon the checked path_accesss.
 */
function kendra_user_ctools_access_summary($conf, $context) {
  if (!empty($conf['name'])) {
    $text = $conf['name'];
  }
  else {
    $text = '';
  }

  return t('Logged in user has access to !text', array('!text' => $text));
}
