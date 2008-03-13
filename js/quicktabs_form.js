$(document).ready(function(){
  
  $('#quicktabs-form div.form-item :input[@name^="type"]:checked').each(function(){
    if(this.value == 'block') {
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="vid"]').parent().hide();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="args"]').parent().hide();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="limit"]').parent().hide();      
    } else {
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="bid"]').parent().hide();       
    }
  });

  var showhide = function() {
    if (this.value == 'block') {
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="vid"]').parent().hide();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="args"]').parent().hide();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="limit"]').parent().hide();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="bid"]').parent().show();    
   } else {
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="bid"]').parent().hide();      
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="vid"]').parent().show();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="args"]').parent().show();
      $(this).parents('fieldset').not('legend:contains("QT")')
      .find('div.form-item :input[@name^="limit"]').parent().show();  
   }
  };
  
  $('#quicktabs-form div.form-item :input[@name^="type"]').bind('click', showhide);
  
});