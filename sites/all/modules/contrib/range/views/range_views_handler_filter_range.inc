<?php

/**
 * @file
 * Contains filter handlers for range filters with views.
 */

/**
 * Filter handler for limiting a view to entities which lies within a range.
 */
class range_views_handler_filter_range extends views_handler_filter {

  var $always_multiple = TRUE;

  function option_definition() {
    $options = parent::option_definition();

    $options['operator'] = array('default' => 'within');
    $options['value'] = array('default' => '');
    $options['include_endpoints'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Define the operators supported for ranges.
   */
  function operators() {
    $operators = array(
      'within' => array(
        'title' => t('Range contains'),
        'short' => t('range contains'),
        'method' => 'op_within',
        'values' => 1,
      ),
      'not within' => array(
        'title' => t('Range does not contain'),
        'short' => t('range does not contain'),
        'method' => 'op_within',
        'values' => 1,
      ),
    );

    return $operators;
  }

  public function operator_options() {
    $options = array();
    foreach ($this->operators() as $key => $value) {
      $options[$key] = $value['title'];
    }

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['include_endpoints'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include endpoints'),
      '#default_value' => $this->options['include_endpoints'],
      '#description' => t('Whether or not include endpoints into the query.')
    );
  }

  public function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Value') : '',
      '#size' => 30,
      '#default_value' => $this->value,
    );
  }

  function query() {
    $this->ensure_my_table();
    $field_from = $this->table_alias . '.' . $this->definition['additional fields']['from'];
    $field_to = $this->table_alias . '.' . $this->definition['additional fields']['to'];

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field_from, $field_to);
    }
  }

  /**
   * Operation callback.
   */
  function op_within($field_from, $field_to) {
    $operators = array(
      '<', '>',
      '<=', '>=',
    );

    $inlude_endpoints = !($this->options['include_endpoints'] xor ($this->operator == 'within'));
    list($op_left, $op_right) = array_slice($operators, $inlude_endpoints ? 2 : 0, 2);

    if ($this->operator == 'within') {
      $this->query->add_where($this->options['group'], db_and()->condition($field_from, $this->value, $op_left)->condition($field_to, $this->value, $op_right));
    }
    else {
      $this->query->add_where($this->options['group'], db_or()->condition($field_from, $this->value, $op_right)->condition($field_to, $this->value, $op_left));
    }
  }

  function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $options = $this->operator_options('short');
    $output = check_plain($options[$this->operator]) . ' ' . check_plain($this->value);

    return $output;
  }

}
