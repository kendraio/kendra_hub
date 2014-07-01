(function ($, settings) {

Drupal.behaviors.adminToggle = {
  attach: function(context) {
    // If the admin toggle is set to 0 (hidden) by default, then toggle all
    // admin items off.
    if (settings.adminToggle.default == 0) {
      adminToggle(true);
    }

    // Bind toggling functionality to the keypress set the in admin area.
    $(document).keypress(function(e) {
      var unicode = e.keyCode ? e.keyCode : e.charCode;
      if (String.fromCharCode(unicode) == settings.adminToggle.key) {
        adminToggle();
      }
    });
  }
};

function adminToggle(skipAnimation) {
  // Set a flag, so that we know which way to toggle.
  if (typeof adminToggle.flag == 'undefined') {
    adminToggle.flag = true;
  }

  if (adminToggle.flag) {
    if (skipAnimation) {
      $(settings.adminToggle.selector).hide();
    }
    else {
      $(settings.adminToggle.selector).fadeOut('fast');
    }

    // Disable contextual links.
    $('.contextual-links-region').removeClass('contextual-links-region').addClass('admin-toggle-contextual-links-region').find('.contextual-links-wrapper').hide();

    // Toggle unpublished node class.
    $('.node-unpublished').removeClass('node-unpublished').addClass('admin-toggle-node-unpublished');
  }
  else {
    if (skipAnimation) {
      $(settings.adminToggle.selector).show();
    }
    else {
      $(settings.adminToggle.selector).fadeIn('fast');
    }

    // Enable contextual links.
    $('.admin-toggle-contextual-links-region').removeClass('admin-toggle-contextual-links-region').addClass('contextual-links-region').find('.contextual-links-wrapper').show();

    // Toggle unpublished node class.
    $('.admin-toggle-node-unpublished').removeClass('admin-toggle-node-unpublished').addClass('node-unpublished');

  }

  // Toggle the flag.
  adminToggle.flag = !adminToggle.flag;

  // Set a session toggle variable. This is done via POST for security reasons.
  var toggle = adminToggle.flag ? 1 : 0;
  $.post(
    settings.basePath + 'admin_toggle/toggle',
    {admin_toggle: toggle}
  );
}

})(jQuery, Drupal.settings);
