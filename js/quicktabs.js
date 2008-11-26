Drupal.settings.views = Drupal.settings.views || {'ajax_path': 'views/ajax'};

Drupal.behaviors.quicktabs = function (context) {
  $('.quicktabs_wrapper:not(.quicktabs-processed)', context).addClass('quicktabs-processed').each(function () {
    var i = 0;
    $(this).find('div.quicktabs:first-child').show()
    .end()
    .find('ul.quicktabs_tabs li:first-child').addClass('active')
    .end()
    .find('ul.quicktabs_tabs li a').each(function(){
      this.myTabIndex = i++;
      $(this).bind('click', quicktabsClick);
    });
  });
};

var quicktabsClick = function() {
  var tabIndex = this.myTabIndex;
  $(this).parents('li').siblings().removeClass('active');
  $(this).parents('li').addClass('active');
  if ( $(this).hasClass('qt_ajax_tabs') ) {
    var viewDetails = this.id.split('-');
    var $container = $('div#quicktabs_ajax_container_' + viewDetails[1]);
    if (viewDetails[0] == 'node') {
      $.get(Drupal.settings.basePath + 'quicktabs/ajax/node/' + viewDetails[3], null, function(data){
        var result = Drupal.parseJson(data);
        $container.html(result['data']);
      });
    } else {
      var target;
      target = $('div#quicktabs_ajax_container_' + viewDetails[1] + ' > div');
      var ajax_path = Drupal.settings.views.ajax_path;
       //If there are multiple views this might've ended up showing up multiple times.
      if (ajax_path.constructor.toString().indexOf("Array") != -1) {
        ajax_path = ajax_path[0];
      }
      var args;
      if (viewDetails.length == 6) {
        args = viewDetails[5].replace(/\|/g, '/');
      } else {
        args = '';
      }
      var viewData = {
      'view_name': viewDetails[3],
      'view_display_id': viewDetails[4],
      'view_args': args
      }
      $.ajax({
        url: ajax_path,
        type: 'GET',
        data: viewData,
        success: function(response) {
          // Call all callbacks.
          if (response.__callbacks) {
            $.each(response.__callbacks, function(i, callback) {
              eval(callback)(target, response);
            });
          }
        },
        error: function() { alert(Drupal.t("An error occurred at ") + ajax_path); },
        dataType: 'json'
      });
      //$.get(Drupal.settings.basePath + 'quicktabs/ajax/views/' + viewDetails[3] + '/' + viewDetails[4] + args, null, function(data){
      //  var result = Drupal.parseJson(data);
      //  $container.html(result['data']);
      //});
    }
  } else {
    $(this).parents('.quicktabs_wrapper').find('div.quicktabs').hide();
    $(this).parents('.quicktabs_wrapper').find('div.quicktabs:eq('+tabIndex+')').show();
  }
  return false;
}