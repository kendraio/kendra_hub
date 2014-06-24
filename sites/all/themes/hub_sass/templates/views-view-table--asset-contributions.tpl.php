<?php if (!empty($title) || !empty($caption)) : ?>
  <div><caption><?php print $caption . $title; ?></caption></div>
<?php endif; ?>

<?php foreach ($result as $result_id => $record): ?>
  <?php // dpm($record); ?>
  <?php foreach ($record->field_field_cont_target_usage as $target): ?>
    <?php print theme('subclip_usage', array(
      'start' => $target['raw']['from'],
      'end' => $target['raw']['to'],
      'nid' => $record->nid,
    )); ?>
  <?php endforeach; ?>
<?php endforeach; ?>

<table <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>

  <?php if (!empty($header)) : ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th <?php if ($header_classes[$field]) { print 'class="'. $header_classes[$field] . '" '; } ?>>
            <?php print $label; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
    <?php foreach ($rows as $row_count => $row): ?>
      <tr <?php if ($row_classes[$row_count]) { print 'class="' . implode(' ', $row_classes[$row_count]) .'"';  } ?>>
        <?php foreach ($row as $field => $content): ?>
          <td <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
