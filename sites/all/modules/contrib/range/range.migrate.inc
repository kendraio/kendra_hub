<?php

/**
 * @file
 * Integration with Migrate module.
 */

/**
 * Implements hook_migrate_api().
 */
function range_migrate_api() {
  return array(
    'api' => 2,
    'field handlers' => array('RangeMigrateFieldHandler'),
  );
}
