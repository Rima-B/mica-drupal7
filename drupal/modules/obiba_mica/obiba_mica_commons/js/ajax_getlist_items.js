/**
 * @file
 * JsScript ajax helper for Search widget : in dataset, network, study list
 */

(function ($) {
  Drupal.behaviors.datatables_add_head = {
    attach: function (context, settings) {
      /*******************/
      if (Drupal.settings.context) {
        //console.log(Drupal.settings.context.url);
        $("#edit-search-query").on("keyup", function () {
          $.ajax({
            url: Drupal.settings.context.url + '/' + $(this).val() + '/' + $("#edit-search-sort").val() + '/' + $("#edit-search-sort-order").val(),
            success: function (data) {
              if (data) {
                $('#refresh-list').empty().append(data.list);
                $('#refrech-count').empty().append(data.total === null ? 0 : data.total);
              }
            }
          });
        });
      }
      $("#edit-search-query").on("blur", function () {
        var data_url = $('#obiba-mica-search-form').serialize();
        window.location = '?' + data_url;
      });

      $("#edit-search-sort-order").on("change", function () {
        var data_url = $('#obiba-mica-search-form').serialize();
        window.location = '?' + data_url;
      });

      $("#edit-search-sort").on("change", function () {
        var data_url = $('#obiba-mica-search-form').serialize();
        window.location = '?' + data_url;
      });
      /*******************/
    }
  }
})(jQuery);
