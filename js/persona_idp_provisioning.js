(function ($) {

  navigator.id.beginProvisioning(function(email, cert_duration) {
    $.ajax({
      url: '/persona-idp/whoami',
      success: function(response) {
        console.log('success::beginProvisioning');
        console.log(response);
        if (email != response.mail) {
          return navigator.id.raiseProvisioningFailure('user is not authenticated as target user');
        }

        navigator.id.genKeyPair(function(pubkey) {
          var data = {
            'pubkey': pubkey,
            'duration': cert_duration,
            'email': email
          };
          $.ajax({
            type: 'POST',
            url: '/persona-idp/cert-key',
            data: data,
            success: function(response) {
              console.log('success::genKeyPair');
              console.log(response);
              console.log(response.certificate);
              navigator.id.registerCertificate(response.certificate);
            },
            error: function(error) {
              console.log('error::genKeyPair');
              console.log(error);
              navigator.id.raiseProvisioningFailure('could not certify key');
            }
          });
        });
      },
      error: function(error) {
        console.log('error::beginProvisioning');
        console.log(error);
        navigator.id.raiseProvisioningFailure("user isn't authenticated");
      }
    });
  });

}) (jQuery);