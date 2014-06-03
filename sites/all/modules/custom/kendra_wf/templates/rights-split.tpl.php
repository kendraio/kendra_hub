
<h3><?php print $title; ?></h3>
<table class="views-table cols-2">
  <thead>
    <tr>
      <th>Right Type</th>
      <th>Split</th>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rights as $right => $value): ?>
      <tr>
        <td><?php print $right; ?></td>
        <td><?php print $value; ?></td>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
