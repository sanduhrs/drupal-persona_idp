(function ($) {

  $(document).ready(function() {

    var form = $('#persona-idp-user-login-form');

    try {
      navigator.id.beginAuthentication(function(email) {});

      $(form).submit(function(event) {
        // stop form from submitting normally
        // event.preventDefault();

        // Get the relevant form values
        var $form = $(this);
        var url = $(form).attr('action');
        var payload = {
          'form_build_id': $form.find('input[name="form_build_id"]').val(),
          'form_id': $form.find('input[name="form_id"]').val(),
          'op': $form.find('input[name="op"]:first').val(),
          'name': $form.find('input[name="name"]').val(),
          'pass': $form.find('input[name="pass"]').val()
        };

        // Send the data using post
        $.post(url, payload, function(response) {
          if (response.success === true) {
            // navigator.id.completeAuthentication();
          }
          else {
          }
          console.log(response);
        });
      });

      $('#edit-cancel').click(function(event) {
        event.preventDefault();
        navigator.id.raiseAuthenticationFailure('cancel');
      });
    } catch(e) {
    }
  });

}) (jQuery);