
<h3><?php print $title; ?></h3>
<table class="aggregated-rights-table cols-3">
  <thead>
    <tr>
      <th>Legal entity</th>
      <th>Contributions</th>
      <th>Percent</th>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($splits as $split): ?>
      <tr>
        <td><?php print $split['key']; ?></td>
        <td><?php print $split['roles']; ?></td>
        <td><?php print $split['share']; ?></td>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
