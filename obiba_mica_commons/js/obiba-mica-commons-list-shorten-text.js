(function ($) {
  Drupal.behaviors.obiba_mica_study_shorten_text = {
    attach: function (context, settings) {

      /********* More/Less in search block ********/
      $(".trim-studies").each(function () {
        var controlSelector = $(this).attr('data-target');
        $(controlSelector).on('shown.bs.collapse', function () {
          $("button[data-target='" + controlSelector + "']").html(Drupal.settings.linkRead['less']);
        });
        $(controlSelector).on('hidden.bs.collapse', function () {
          $("button[data-target='" + controlSelector + "']").html(Drupal.settings.linkRead['more']);
        });
      });

    }
  }
}(jQuery));
