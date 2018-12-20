<?php

/**
 * @file
 * Contains range field migration handler.
 */

/**
 * Range field migration handler class.
 *
 * Range field requires both FROM and TO values, so it doesn't make sense to use
 * the primary value, i.e. map to field itself.
 * In order to use it, please map values to the subfields (mapping to the field
 * won't work).
 *
 * Subfields:
 *   - from: Range field FROM value
 *   - to: Range field TO value
 *   - language: Range field value language
 *
 * See more at https://www.drupal.org/node/2417365
 */
class RangeMigrateFieldHandler extends MigrateFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    $this->registerTypes(array('range_integer', 'range_decimal', 'range_float'));
  }

  /**
   * {@inheritdoc}
   */
  static public function arguments($from = NULL, $to = NULL, $language = NULL) {
    $arguments = array();
    if (!is_null($from)) {
      $arguments['from'] = $from;
    }
    if (!is_null($to)) {
      $arguments['to'] = $to;
    }
    if (!is_null($language)) {
      $arguments['language'] = $language;
    }
    return $arguments;
  }

  /**
   * {@inheritdoc}
   */
  public function fields($type, $instance, $migration = NULL) {
    return array(
      'from' => t('Subfield: <a href="!href">Range field FROM value</a>', array('!href' => 'https://www.drupal.org/node/2417365#from')),
      'to' => t('Subfield: <a href="!href">Range field TO value</a>', array('!href' => 'https://www.drupal.org/node/2417365#to')),
      'language' => t('Subfield:  <a href="!href">Range field value language</a>', array('!href' => 'https://www.drupal.org/node/2417365#language')),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {

    $arguments = isset($values['arguments']) ? $values['arguments'] : array();
    $arguments += array(
      'from' => array(),
      'to' => array(),
    );

    // When a single value is mapped to an argument, Migrate module doesn't wrap
    // it in array.
    foreach (array('from', 'to') as $subelement) {
      if (!is_array($arguments[$subelement])) {
        $arguments[$subelement] = array($arguments[$subelement]);
      }
    }

    $return = array();
    foreach ($arguments['from'] as $delta => $value) {
      $item = array(
        'from' => $arguments['from'][$delta],
        'to' => isset($arguments['to'][$delta]) ? $arguments['to'][$delta] : NULL,
      );

      if ($this->validate($entity, $field_info, $instance, $item)) {
        $language = $this->getRangeFieldLanguage($entity, $field_info, $arguments, $delta);
        $return[$language][] = $item;
      }
    }

    return !empty($return) ? $return : NULL;
  }

  /**
   * Determines field language.
   *
   * @param object $entity
   *   Entity being processed.
   * @param array $field_info
   *   Field info.
   * @param array $arguments
   *   Arguments passed to the field migration handler.
   * @param int $delta
   *   Field values delta.
   *
   * @return string
   *   Language code.
   *
   * @see MigrateFieldHandler::getFieldLanguage()
   */
  public function getRangeFieldLanguage($entity, array $field_info, array $arguments, $delta = 0) {
    static $language = NULL;

    if (is_null($language)) {
      $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    }

    if (!is_array($language)) {
      return $language;
    }
    else {
      if (!empty($language[$delta])) {
        return $language[$delta];
      }
      else {
        return LANGUAGE_NONE;
      }
    }
  }

  /**
   * Validates given range field value item.
   *
   * @param object $entity
   *   Entity being processed.
   * @param array $field_info
   *   Field info.
   * @param array $instance
   *   Field instance.
   * @param array $item
   *   Field values delta.
   *
   * @return bool
   *   Returns TRUE if given item contains valid range field value, FALSE -
   *   otherwise.
   */
  protected function validate($entity, array $field_info, array $instance, array $item) {
    $item_is_valid = TRUE;

    $migration = Migration::currentMigration();

    if (((string) $item['from'] === '') || ((string) $item['to'] === '')) {
      $migration->saveMessage(t('%name: both FROM and TO values are required.', array('%name' => $instance['label'])), Migration::MESSAGE_ERROR);
      $item_is_valid = FALSE;
    }
    if (!is_numeric($item['from']) || !is_numeric($item['to'])) {
      $migration->saveMessage(t('%name: only numeric values are acceptable.', array('%name' => $instance['label'])), Migration::MESSAGE_ERROR);
      $item_is_valid = FALSE;
    }
    if ($field_info['type'] === 'range_integer' && (!ctype_digit($item['from']) || !ctype_digit($item['to']))) {
      $migration->saveMessage(t('%name: is an integer range, however float point values were found.', array('%name' => $instance['label'])), Migration::MESSAGE_WARNING);
    }

    return $item_is_valid;
  }

}
