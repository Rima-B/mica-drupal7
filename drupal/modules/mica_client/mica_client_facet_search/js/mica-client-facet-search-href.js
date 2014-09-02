(function ($) {
  Drupal.behaviors.queries = {
    attach: function (context, settings) {

      /*******************/
      $.extend({
        checkthebox: function (obj_span) {
          obj_span.removeClass("unchecked");
          obj_span.addClass("checked");
          obj_span.children(":first").removeClass();
          obj_span.children(":first").addClass("glyphicon glyphicon-ok");
        },

        uncheckthebox: function (obj_span) {
          obj_span.removeClass("checked");
          obj_span.addClass("unchecked");
          obj_span.children(":first").removeClass();
          obj_span.children(":first").addClass("glyphicon glyphicon-unchecked");
        },
        rollover: function (obj_span) {
          obj_span.children(":first").removeClass();
          obj_span.children(":first").toggleClass("glyphicon glyphicon-remove");
        },
        rollout: function (obj_span) {
          obj_span.children(":first").removeClass();
          obj_span.children(":first").toggleClass("glyphicon glyphicon-ok");
        },

        getUrlVars: function () {
          var vars = [], hash;
          var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
          for (var i = 0; i < hashes.length; i++) {
            hash = hashes[i].split('=');
            //console.log(decodeURIComponent(hash[1]));
            vars.push(decodeURIComponent(hash[1]));
          }
          return vars;
        },
        sendCheckboxCheckedValues: function (idcheckbox) {
          var serializedData = "";
          $('form').each(function () {
            $('input', 'form').each(function () {
              $(this).val() == "" && $(this).remove();
            })
            var SerilizedForm = ($(this).serialize());
            if (SerilizedForm && $(this).attr('id').match(/facet-search/g)) {
              serializedData = serializedData.concat(SerilizedForm).concat('&');
            }
          });
          return serializedData;
        }
      });

      /**********************/
      var allVars = $.getUrlVars();
      $('input[type="checkbox"]').each(function () {
        var currInputValue = $(this).attr('value');
        if ($.inArray(currInputValue, allVars) !== -1) {
          $(this).attr('checked', true);
          $(this).parent().parent().addClass("selected_item");
        }
      });

      $('input[type="hidden"]').each(function () {
        var currInputid = $(this).attr('id');
        if (allVars.toString().search(currInputid) != -1) {
          $.grep(allVars, function (element, i) {
            if (!element.indexOf(currInputid)) {
              $('#' + currInputid).val(element.replace(/\+/g, ' '));
            }
          });
        }
      });

      $('span#checkthebox').each(function () {
        var currInputid = $(this).attr('value');
        if (allVars.toString().search(currInputid) != -1) {
          //console.log(currInputid);
          if ($(this).hasClass("unchecked")) {
            $.checkthebox($(this));
            $(this).children('input[type="hidden"]').val($(this).attr("value"));
          }
        }
        else {
          if ($(this).hasClass("checked")) {
            $.uncheckthebox($(this));
            $(this).children('input[type="hidden"]').val('');
          }
        }
      });

      $("span#checkthebox").on("click", function (e) {
        if ($(this).hasClass("unchecked")) {
          $.checkthebox($(this));
          $(this).children('input[type="hidden"]').val($(this).attr("value"));
          window.location = '?' + $.sendCheckboxCheckedValues();
          return false;

        }
        if ($(this).hasClass("checked")) {
          $.uncheckthebox($(this));
          $(this).children('input[type="hidden"]').val('');
          window.location = '?' + $.sendCheckboxCheckedValues();
          //  console.log($.sendCheckboxCheckedValues());
          return false;
        }
      });

      $("span#checkthebox").mouseover(function () {
        if ($(this).hasClass("checked")) {
          $.rollover($(this));
          return false;
        }
      });

      $("span#checkthebox").mouseout(function () {
        if ($(this).hasClass("checked")) {
          $.rollout($(this));
          return false;
        }
      });

      /*send request search on checking the box or on click go button */
      $("div#checkthebox").on("click", function () {
        //  $.sendCheckboxCheckedValues();
        window.location = '?' + $.sendCheckboxCheckedValues();
      });

      $("input[id='range-auto-fill']").on("blur", function () {

        var term = $(this).attr('termselect');
        var minid = term + '-min';
        var maxid = term + '-max';
        var minvalue = $("input[term='" + minid + "']").val();
        var maxvalue = $("input[term='" + maxid + "']").val();

        if (minvalue || maxvalue) {
          $('#' + $(this).attr('termselect')).val(term + '.[ ' + minvalue + ' to ' + maxvalue + ' ]');
        }
        if (!maxvalue && !minvalue) {
          $('#' + $(this).attr('termselect')).val('');
        }

      });


      ///her we goo
//      if (Drupal.settings.mica_client_study.queries) {
//        // console.log(Drupal.settings.mica_client_study.queries);
//
//      }


    }
  }
})(jQuery);