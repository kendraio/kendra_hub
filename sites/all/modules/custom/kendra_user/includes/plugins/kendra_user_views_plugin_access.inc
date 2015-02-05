<?php

/**
 * @file
 * Contains views access plugin.
 */

/**
 * Allow views to allow access to only users with access to the asset.
 */
class kendra_user_views_plugin_access extends views_plugin_access {

  function summary_title() {
    return $this->options['options'];
  }

  /**
   * Determine if the current user has access or not.
   */
  function access($account) {
    return kendra_user_views_access($this->options['options']);

    //drupal_set_message('access');
    switch ($this->options['options']) {
      case 'Asset owner':
        //return FALSE;
        return kendra_user_can_user_edit_asset();
        break;

      default:
        return FALSE;
        break;
    }
  }

  function get_access_callback() {
    return array('kendra_user_views_access', array($this->options['options']));
  }

    function options_form(&$form, &$form_state) {
      parent::options_form($form, $form_state);
      $options = array(
        'Asset owner' => 'Only show on asset pages owned by curent user ro for Kendra admin',
      );
      $form['options'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Display view for'),
        '#default_value' => $this->options['options'],
        '#description' => t(''),
      );
  }

}
