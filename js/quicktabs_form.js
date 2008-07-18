Drupal.quicktabsShowHide = function() {
  if (this.value == 'block') {
    $(this).parents('tr')
    .find('div.form-item :input[name*="vid"]').parent().hide();
    $(this).parents('tr')
    .find('div.form-item :input[name*="args"]').parent().hide();
    $(this).parents('tr')
    .find('div.form-item :input[name*="display"]').parent().hide();  
    $(this).parents('tr')
    .find('div.form-item :input[name*="bid"]').parent().show();    
    $(this).parents('tr')
    .find('div.form-item :input[name*="hide_title"]').parent().show(); 
 } else {
    $(this).parents('tr')
    .find('div.form-item :input[name*="bid"]').parent().hide();
    $(this).parents('tr')
    .find('div.form-item :input[name*="hide_title"]').parent().hide(); 
    $(this).parents('tr')
    .find('div.form-item :input[name*="vid"]').parent().show();
    $(this).parents('tr')
    .find('div.form-item :input[name*="args"]').parent().show(); 
    $(this).parents('tr')
    .find('div.form-item :input[name*="display"]').parent().show();  

 }
};
    
Drupal.quicktabsGetDisplays = function() {
  var ajax_path = Drupal.settings.quicktabsForm.ajax_path;
  var viewName = this.value;
  var viewDisplaySelect = $(this).parent().parent().find(':input[name*="display"]');
  var queryString = $('#quicktabs-form').formSerialize();
  var ddIndex = $('#quicktabs-form tbody tr').index($(this).parents('tr')[0]);
  $.post(ajax_path + '/' + viewName+ '/' + ddIndex, queryString, function(response){
    var result = Drupal.parseJson(response);
    viewDisplaySelect.parent().empty().append(result.data);
  });
};

Drupal.behaviors.quicktabsform = function(context) {
  $('#quicktabs-form tr').not('.quicktabs-form-processed').addClass('quicktabs-form-processed').each(function(){
    var currentRow = $(this);
    $(':input[name*="tabtype"]:checked', this).each(function(){
      if(this.value == 'block') {
        currentRow.find('div.form-item :input[name*="vid"]').parent().hide();
        currentRow.find('div.form-item :input[name*="args"]').parent().hide();
        currentRow.find('div.form-item :input[name*="display"]').parent().hide(); 
      } else {
        currentRow.find('div.form-item :input[name*="bid"]').parent().hide();
        currentRow.find('div.form-item :input[name*="hide_title"]').parent().hide();
      }
    });
    
    currentRow.find('div.form-item :input[name*="tabtype"]').bind('click', Drupal.quicktabsShowHide);
    currentRow.find('div.form-item :input[name*="vid"]').bind('change', Drupal.quicktabsGetDisplays);
  })
};