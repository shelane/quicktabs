Drupal.quicktabsShowHide = function() {
  if (this.value == 'block' || this.value == 'node') {
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
    $(this).parents('tr')
    .find('div.form-item :input[name*="nid"]').parent().show(); 
 } else {
    $(this).parents('tr')
    .find('div.form-item :input[name*="nid"]').parent().hide(); 
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

Drupal.behaviors.quicktabsform = function(context) {
  $('#quicktabs-form tr, #quicktabs-aj-form tr').not('.quicktabs-form-processed').addClass('quicktabs-form-processed').each(function(){
    var currentRow = $(this);
    $(':input[name*="tabtype"]:checked', this).each(function(){
      if(this.value == 'block' || this.value == 'node') {
        currentRow.find('div.form-item :input[name*="vid"]').parent().hide();
        currentRow.find('div.form-item :input[name*="args"]').parent().hide();
        currentRow.find('div.form-item :input[name*="display"]').parent().hide(); 
      } else {
        currentRow.find('div.form-item :input[name*="bid"]').parent().hide();
        currentRow.find('div.form-item :input[name*="hide_title"]').parent().hide();
        currentRow.find('div.form-item :input[name*="nid"]').parent().hide();
      }
    });
    currentRow.find('div.form-item :input[name*="tabtype"]').bind('click', Drupal.quicktabsShowHide);
  })
};