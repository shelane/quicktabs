(function ($) {

Drupal.behaviors.qt_accordion = {
  attach: function (context, settings) {
    $('.quick-accordion').once(function(){
      var qtKey = 'qt_' + Drupal.quicktabs.getQTName(this);
      var options = Drupal.settings.quicktabs[qtKey].options;
      var active_tab = parseInt(Drupal.settings.quicktabs[qtKey].active_tab);
      options.active = active_tab;
      $(this).accordion(options);
    });
  }
}

})(jQuery);
