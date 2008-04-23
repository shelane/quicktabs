Drupal.behaviors.quicktabs = function (context) {
  $('.quicktabs_wrapper:not(.quicktabs-processed)', context).addClass('quicktabs-processed').each(function () {
    var i = 0;
    $(this).find('div.quicktabs').hide()
    .end()
    .find('div.quicktabs:first-child').show()
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
  $(this).parents('.quicktabs_wrapper').find('div.quicktabs').hide();
  $(this).parents('li').siblings().removeClass('active');
  $(this).parents('li').addClass('active');
  $(this).parents('.quicktabs_wrapper').find('div.quicktabs:eq('+tabIndex+')').show();
  return false;
}