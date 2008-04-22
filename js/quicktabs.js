Drupal.quicktabs = function() {
  $('div.quicktabs').hide();
  $('div.quicktabs:first-child').show();
  $('ul.quicktabs_tabs li:first-child').addClass('active');
  var effect = Drupal.settings.quicktabs.effect;
  var clickFunction = function() {      
    var tabIndex = this.myTabIndex;
    $(this).parents('div.quicktabs_wrapper').find('div.quicktabs').hide();
    $(this).parents('li').siblings().removeClass('active');
    $(this).parents('li').addClass('active');
    switch (effect) {
      case 'none':
        $(this).parents('div.quicktabs_wrapper').find('div.quicktabs:eq('+tabIndex+')').show();
        break;
      case 'fade':
        $(this).parents('div.quicktabs_wrapper').find('div.quicktabs:eq('+tabIndex+')').fadeIn();        
        break;
      case 'slide':
        $(this).parents('div.quicktabs_wrapper').find('div.quicktabs:eq('+tabIndex+')').slideDown('slow');
        break;
    }

    return false;
  };
  $('ul.quicktabs_tabs').each(function(){
    var i = 0;
    $(this).find('li a').each(function() {
      this.myTabIndex = i++;
    });
  });
  $('ul.quicktabs_tabs li a').bind('click', clickFunction);
}


if (Drupal.jsEnabled) {
  $(document).ready(Drupal.quicktabs);
}