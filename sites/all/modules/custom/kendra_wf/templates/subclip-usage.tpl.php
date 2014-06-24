<div class="subclip-wrapper">
  <?php if (!empty($edit_url)): ?>
    <a href="<?php print $edit_url; ?>" class="subclip-config-link"><i class="icon icon-config"></i></a>
  <?php endif; ?>
  <div class="subclip-usage" style="<?php print $subclip_style; ?>">
    <?php print l($subclip->title, 'node/' . $subclip->nid); ?>
  </div>
</div>
