(function ($) {
  var hasAjax = false;
  var ready = false;
  $.ObibaProgressBarController = (function() {
    var bar = new $.ObibaProgressBar();

    return {
      start: bar.start,
      pause: bar.pause,
      inc: bar.inc,
      update: bar.update,
      finish: bar.finish,
      setPercentage: bar.set
    };
  });

  $(document).ready(function () {
    if (!hasAjax){
      $.ObibaProgressBarController().finish();
    }
    ready = true;
  });

  $(document).ajaxStart(function () {
    hasAjax = true;
    if (ready){
      $.ObibaProgressBarController().start();
    }
  });

  $(document).ajaxSend(function () {
  });

  $(document).ajaxComplete(function () {
    $.ObibaProgressBarController().inc(5);
  });

  $(document).ajaxStop(function () {
    hasAjax = false;
    $.ObibaProgressBarController().finish();
  });

})(jQuery);
