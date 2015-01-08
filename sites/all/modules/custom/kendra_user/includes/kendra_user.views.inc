<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function kendra_user_views_plugins() {

  return array(

    'access' => array(
      'asset' => array(
        'title' => t('Asset access'),
        'help' => t('Access will be granted to asset owners and kendra admin.'),
        'handler' => 'kendra_user_views_plugin_access',
        'path' => drupal_get_path('module', 'kendra_user') . '/includes/plugins',
        'uses options' => TRUE,
      ),
   ),
 );
}