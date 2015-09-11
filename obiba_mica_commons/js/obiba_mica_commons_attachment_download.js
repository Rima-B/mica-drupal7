/**
 * @file
 * JsScript to deal with  attachment files
 */

(function ($) {
  Drupal.behaviors.attachement_download = {
    attach: function (context, settings) {
      $('a#download-attachment').each(function () {
        //console.log($(this));

        $(this).on('click', function (event) {
          // create a form for the file upload
          var entityType = $(this).attr('entity');
          var idEntity = $(this).attr('id_entity');
          var idAttachment = $(this).attr('id_attachment');

          var form = $("<form action='" + Drupal.settings.basePath + 'download/' + entityType + '/' + idEntity + '/' + idAttachment + "' method='post'>");
          $(this).after(form);
          form.submit().remove();
          return false;
        });

      });

    }
  }
})(jQuery);
