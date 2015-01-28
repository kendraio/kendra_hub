<?php

/**
 * @file
 * Rules integration for Kendra user permissions.
 */

/**
 * Implements hook_rules_condition_info().
 */
function kendra_user_rules_condition_info() {
  $conditions = array();

  $conditions['kendra_user_can_edit_asset'] = array(
    'label' => t('User can edit the asset'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('user'),
      ),
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
    ),
    'group' => t('Kendra'),
    'callbacks' => array(
      'execute' => 'kendra_user_can_edit_asset',
    ),
  );

  return $conditions;
}


function kendra_user_can_edit_asset($user, $node) {
  return kendra_user_can_specified_user_edit_specified_asset($user, $node);
  return TRUE;
  return kendra_user_can_specified_user_edit_asset($user);
}
