// $Id$

Drupal.settings.views = Drupal.settings.views || {'ajax_path': 'views/ajax'};

Drupal.behaviors.quicktabs = function (context) {
  $('.quicktabs_wrapper:not(.quicktabs-processed)', context).addClass('quicktabs-processed').each(function(){
    $(this).prepareQuicktabs();
  });
};

$.fn.prepareQuicktabs = function() {
  var i = 0;
  $(this).find('ul.quicktabs_tabs li a').each(function(){
    this.myTabIndex = i++;
    $(this).bind('click', quicktabsClick);
  });

  // Search for the active tab.
  var $active_tab = $(this).children('.quicktabs_tabs').find('li.active a');

  if ($active_tab.hasClass('qt_tab') || $active_tab.hasClass('qt_ajax_tab')) {
    $active_tab.trigger('click');
  }
  else {
    // Click on the first tab.
    $(this).children('.quicktabs_tabs').find('li.first a').trigger('click');
  }

  return false;
};

var quicktabsClick = function() {
  var tabIndex = this.myTabIndex;
  var tabDetails = this.id.split('--');
  var tabType = tabDetails[0];
  var qtid = tabDetails[1];
  var tabKey = tabDetails[2];
  var tabpage_id = 'quicktabs_tabpage_' + qtid + '_' + tabKey;

  var $container = $('#quicktabs_container_' + qtid);
  var $tabpage = $container.find('#' + tabpage_id);

  // Set clicked tab to active.
  $(this).parents('li').siblings().removeClass('active');
  $(this).parents('li').addClass('active');

  // Hide all tabpages.
  $container.children().hide();

  // Show the active tabpage.
  if ($tabpage.hasClass('quicktabs_tabpage')) {
    $tabpage.show();
  }
  else {
    if ( $(this).hasClass('qt_ajax_tab') ) {
      // Construct the ajax tabpage.
      switch (tabType) {
        case 'qtabs':
          $.get(Drupal.settings.basePath + 'quicktabs/ajax/qtabs/' + tabDetails[3], null, function(data){
            var result = Drupal.parseJson(data);
            $container.append('<div id="' + tabpage_id + '" class="quicktabs_tabpage">' + result['data'] + '</div>');
            // The new quicktabs must be prepared.
            $container.find('#quicktabs-' + tabDetails[3]).prepareQuicktabs();
          });
          break;
        case 'block':
          $.get(Drupal.settings.basePath + 'quicktabs/ajax/block/' + tabDetails[3] + '/' + tabDetails[4], null, function(data){
            var result = Drupal.parseJson(data);
            $container.append('<div id="' + tabpage_id + '" class="quicktabs_tabpage">' + result['data'] + '</div>');
          });
          break;
        case 'node':
          $.get(Drupal.settings.basePath + 'quicktabs/ajax/node/' + tabDetails[3] + '/' + tabDetails[4] + '/' + tabDetails[5], null, function(data){
            var result = Drupal.parseJson(data);
            $container.append('<div id="' + tabpage_id + '" class="quicktabs_tabpage">' + result['data'] + '</div>');
          });
          break;
  
        case 'view':
          // Create an empty div for the tabpage. The generated view will be inserted into this.
          $container.append('<div id="' + tabpage_id + '" class="quicktabs_tabpage"><div></div></div>');
  
          var target;
          target = $('#' + tabpage_id + ' > div');
          var ajax_path = Drupal.settings.views.ajax_path;
           //If there are multiple views this might've ended up showing up multiple times.
          if (ajax_path.constructor.toString().indexOf("Array") != -1) {
            ajax_path = ajax_path[0];
          }
          var args;
          if (tabDetails.length == 6) {
            args = tabDetails[5].replace(/-/g, '/');
          } else {
            args = '';
          }
          var viewData = {
            'view_name': tabDetails[3],
            'view_display_id': tabDetails[4],
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
          break;
      }
    }
  }
  return false;
}
