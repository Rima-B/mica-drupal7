/**
 * @file
 * JsScript ajax helper for Search widget : in dataset, network, study list
 */

var datatables_add_head_TimeoutId = null;

(function ($) {
  Drupal.behaviors.datatables_add_head = {
    attach: function (context, settings) {

      /*******************/
      if (Drupal.settings.context) {
        $("#edit-search-query").on("keyup", function () {
          var that = this;
          function doSearch(){
            $.ajax({
              url: Drupal.settings.context.url + '/' + $(that).val() + '/' +
              $("#edit-search-sort").val() + '/' + $("#edit-search-sort-order").val() + '/0',
              success: function (data) {
                if (data) {
                  $('#refresh-list').empty().append(data.list);
                  $('#refresh-count').empty().append(data.total === null ? 0 : data.total);
                }
              }
            });
          }

          if (datatables_add_head_TimeoutId) {
            clearTimeout(datatables_add_head_TimeoutId);
          }

          datatables_add_head_TimeoutId = setTimeout(doSearch, 250);
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

      $("#refresh-button").on("click", function () {
        event.preventDefault();
        window.location = window.location.pathname;
      });
      /*******************/
    }
  }
})(jQuery);
