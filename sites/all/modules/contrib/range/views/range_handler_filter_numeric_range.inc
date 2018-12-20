<?php

/**
 * @file
 * Contains filter handlers for numeric range filters with views.
 */

/**
 * Filter handler for limiting a view to entities which lies within a range.
 */
class range_handler_filter_numeric_range extends views_handler_filter {

  public $always_multiple = TRUE;

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['operator'] = array('default' => 'within');
    $options['value'] = array('default' => '');
    $options['include_endpoints'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Define the operators supported for ranges.
   */
  protected function operators() {
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

  /**
   * {@inheritdoc}
   */
  public function operator_options() {
    $options = array();
    foreach ($this->operators() as $key => $value) {
      $options[$key] = $value['title'];
    }

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['include_endpoints'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include endpoints'),
      '#default_value' => $this->options['include_endpoints'],
      '#description' => t('Whether or not include endpoints into the query.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Value') : '',
      '#size' => 30,
      '#default_value' => $this->value,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    list($field_from, $field_to) = $this->get_range_subfields();

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field_from, $field_to);
    }
  }

  /**
   * Operation callback.
   */
  protected function op_within($field_from, $field_to) {
    $operators = array(
      '<', '>',
      '<=', '>=',
    );

    $inlude_endpoints = !($this->options['include_endpoints'] xor ($this->operator === 'within'));
    list($op_left, $op_right) = array_slice($operators, $inlude_endpoints ? 2 : 0, 2);

    $this->op_within_query($this->operator, $field_from, $field_to, $op_left, $op_right);
  }

  /**
   * Operation query.
   */
  protected function op_within_query($operator, $field_from, $field_to, $op_left, $op_right) {
    if ($operator === 'within') {
      $this->query->add_where($this->options['group'], db_and()->condition($field_from, $this->value, $op_left)->condition($field_to, $this->value, $op_right));
    }
    else {
      $this->query->add_where($this->options['group'], db_or()->condition($field_from, $this->value, $op_right)->condition($field_to, $this->value, $op_left));
    }
  }

  /**
   * Returns database field names for FROM and TO values.
   *
   * @return array
   *   FROM and TO values database field names.
   */
  protected function get_range_subfields() {
    $this->ensure_my_table();
    return array(
      $this->table_alias . '.' . $this->definition['additional fields']['from'],
      $this->table_alias . '.' . $this->definition['additional fields']['to']
    );
  }

  /**
   * {@inheritdoc}
   */
  public function admin_summary() {
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

/**
 * Filter handler for limiting a view to indexed content which lies within a range.
 */
class range_search_api_handler_filter_numeric_range extends range_handler_filter_numeric_range {

  /**
   * {@inheritdoc}
   */
  protected function op_within_query($operator, $field_from, $field_to, $op_left, $op_right) {
    if ($operator === 'within') {
      $filter = $this->query->createFilter('AND');
      $filter->condition($field_from, $this->value, $op_left);
      $filter->condition($field_to, $this->value, $op_right);
    }
    else {
      $filter = $this->query->createFilter('OR');
      $filter->condition($field_from, $this->value, $op_right);
      $filter->condition($field_to, $this->value, $op_left);
    }
    $this->query->filter($filter);
  }

  /**
   * {@inheritdoc}
   */
  protected function get_range_subfields() {
    return array(
      "$this->real_field:from",
      "$this->real_field:to",
    );
  }

}
