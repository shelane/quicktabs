$(document).ready(function(){
  
  $('#quicktabs-form div.form-item :input[@name*="tabtype"]:checked').each(function(){
    if(this.value == 'block') {
      $(this).parents('tr')
      .find('div.form-item :input[@name*="vid"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="args"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="limit"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="build"]').parent().hide(); 
    } else {
      $(this).parents('tr')
      .find('div.form-item :input[@name*="bid"]').parent().hide();       
    }
  });

  var showhide = function() {
    if (this.value == 'block') {
      $(this).parents('tr')
      .find('div.form-item :input[@name*="vid"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="args"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="limit"]').parent().hide();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="build"]').parent().hide();  
      $(this).parents('tr')
      .find('div.form-item :input[@name*="bid"]').parent().show();    
   } else {
      $(this).parents('tr')
      .find('div.form-item :input[@name*="bid"]').parent().hide();      
      $(this).parents('tr')
      .find('div.form-item :input[@name*="vid"]').parent().show();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="args"]').parent().show();
      $(this).parents('tr')
      .find('div.form-item :input[@name*="limit"]').parent().show();  
      $(this).parents('tr')
      .find('div.form-item :input[@name*="build"]').parent().show();  

   }
  };
  
  $('#quicktabs-form div.form-item :input[@name*="tabtype"]').bind('click', showhide);
  $('input.delete-tab-disabled').attr('disabled', 'disabled');
});