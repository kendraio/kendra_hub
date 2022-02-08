/**
 * @file
 * Integrates d3 library and functionality with D7 core javascript
 */

// polyfill for Array.includes
Array.prototype.includes || Object.defineProperty(Array.prototype, "includes", {
  value: function(r, e) {
    if (null == this) throw new TypeError('"this" is null or not defined');
    var t = Object(this),
      n = t.length >>> 0;
    if (0 === n) return !1;
    var i, o, a = 0 | e,
      u = Math.max(a >= 0 ? a : n - Math.abs(a), 0);
    for (; u < n;) {
      if ((i = t[u]) === (o = r) || "number" == typeof i && "number" == typeof o && isNaN(i) && isNaN(o)) return !0;
      u++
    }
    return !1
  }
});

(function($, Drupal) {
  Drupal.behaviors.d3_config_form = {
    attach: function(context, settings) {

      var source_select = $('.form-item-d3-library-source select');

      //this is for setting up options on page load
      test();

      source_select.change(test);

      function test() {
        var selected = source_select.val();
        var ok_versions = d3_library_versions[selected];
        $('.form-item-d3-library-version option').each(function() {
          $(this).attr('disabled',!ok_versions.includes($(this).val()));
        });
      }
    }
  };
})(jQuery, Drupal);
