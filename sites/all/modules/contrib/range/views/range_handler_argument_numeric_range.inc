<?php

/**
 * @file
 * Contains argument handler for numeric range arguments with views.
 */

/**
 * Argument handler for limiting a view to entities which lies within a range.
 */
class range_handler_argument_numeric_range extends views_handler_argument {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['operator'] = array('default' => 'within');
    $options['include_endpoints'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['operator'] = array(
      '#type' => 'radios',
      '#title' => t('Operator'),
      '#options' => array(
        'within' => t('Range contains'),
        'not within' => t('Range does not contain'),
      ),
      '#default_value' => $this->options['operator'],
      '#fieldset' => 'more',
    );
    $form['include_endpoints'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include endpoints'),
      '#default_value' => !empty($this->options['include_endpoints']),
      '#description' => t('Whether or not include endpoints into the query.'),
      '#fieldset' => 'more',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function query($group_by = FALSE) {
    list($field_from, $field_to) = $this->get_range_subfields();

    $operators = array(
      '<', '>',
      '<=', '>=',
    );
    $operator = $this->options['operator'];

    $inlude_endpoints = !($this->options['include_endpoints'] xor ($operator === 'within'));
    list($op_left, $op_right) = array_slice($operators, $inlude_endpoints ? 2 : 0, 2);

    $this->op_within_query($operator, $field_from, $field_to, $op_left, $op_right);
  }

  /**
   * Operation query.
   */
  protected function op_within_query($operator, $field_from, $field_to, $op_left, $op_right) {
    if ($operator === 'within') {
      $this->query->add_where(0, db_and()->condition($field_from, $this->argument, $op_left)->condition($field_to, $this->argument, $op_right));
    }
    else {
      $this->query->add_where(0, db_or()->condition($field_from, $this->argument, $op_right)->condition($field_to, $this->argument, $op_left));
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
  public function get_sort_name() {
    return t('Numerical', array(), array('context' => 'Sort order'));
  }

}

/**
 * Argument handler for limiting a view to indexed content which lies within a range.
 */
class range_search_api_handler_argument_numeric_range extends range_handler_argument_numeric_range {

  /**
   * {@inheritdoc}
   */
  protected function op_within_query($operator, $field_from, $field_to, $op_left, $op_right) {
    if ($operator === 'within') {
      $filter = $this->query->createFilter('AND');
      $filter->condition($field_from, $this->argument, $op_left);
      $filter->condition($field_to, $this->argument, $op_right);
    }
    else {
      $filter = $this->query->createFilter('OR');
      $filter->condition($field_from, $this->argument, $op_right);
      $filter->condition($field_to, $this->argument, $op_left);
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
