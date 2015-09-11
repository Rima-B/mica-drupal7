/**
 * @file
 * JsScript for Go to top button
 */

(function ($) {
  Drupal.behaviors.obiba_mica_commons = {
    attach: function (context, settings) {
      /*deal with back to top button*/
      var offset = 220;
      var duration = 500;
      jQuery(window).scroll(function () {
        if (jQuery(this).scrollTop() > offset) {
          jQuery('.back-to-top').fadeIn(duration);

        }
        else {
          jQuery('.back-to-top').fadeOut(duration);
        }
      });

      jQuery('.back-to-top').click(function (event) {
        event.preventDefault();
        jQuery('html, body').animate({scrollTop: 0}, duration);
        return false;
      });
      /****************************/

    }

  }

}(jQuery));
