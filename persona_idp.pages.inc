<?php

/**
 * Menu callback switch; Menu callback
 */
function persona_idp_switch() {
  $delegate = variable_get('persona_idp_delegate', FALSE);
  if ($delegate) {
    persona_idp_delegate_support();
  }
  else {
    persona_idp_basic_support();
  }
}

/**
 * Delegated Support Document; Menu callback
 */
function persona_idp_delegate_support() {
  $document = array(
    'authority' => variable_get('persona_idp_authority', 'persona.org')
  );
  drupal_add_http_header('Status', '200 OK');
  drupal_json_output($document);
  drupal_exit();
}

/**
 * Basic Support Document; Menu callback
 */
function persona_idp_basic_support() {
  // Make shure we have a trailing slash
  $keypath = rtrim(variable_get('persona_idp_keypath', '.' . base_path()), '/') . '/';
  $public_key = (array) json_decode(file_get_contents($keypath . 'key.publickey'));
  $document = array(
    'public-key' => $public_key,
    'authentication' => url('persona-idp/authenticate'),
    'provisioning' => url('persona-idp/provision'),
  );
  drupal_add_http_header('Status', '200 OK');
  drupal_json_output($document);
  drupal_exit();
}

/**
 * Persona Sign-In; Menu callback
 */
function persona_idp_authentication() {
  $jquery = drupal_get_library('system', 'jquery');
  $jquery_local = url(key($jquery['js']));
  $include_remote = 'https://login.dev.anosrep.org/include.js';
  $authentication_remote = 'https://login.dev.anosrep.org/authentication_api.js';
  $authentication_local = url(drupal_get_path('module', 'persona_idp') . '/js/persona_idp_authentication.js');
  // $form = drupal_get_form('user_login', array('persona_idp' => TRUE));
  $form = drupal_get_form('persona_idp_user_login_form');
  return $form;

  $document = "<!DOCTYPE html>\n";
  $document .= "<html>\n";
  $document .= "  <head>\n";
  $document .= "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
  $document .= "    <!--[if lt IE 9]>\n";
  $document .= "      <script src='https://static.dev.anosrep.org/v/4b0d630691/production/html5shim.js'></script>\n";
  $document .= "    <![endif]-->\n";
  $document .= "    <link href='https://static.dev.anosrep.org/v/cffd52d366/production/dialog.css' rel='stylesheet' type='text/css'>\n";
  $document .= "    <!--[if lt IE 9]>\n";
  $document .= "      <link href='https://static.dev.anosrep.org/v/89b9e4fef1/production/ie8_dialog.css' rel='stylesheet' type='text/css'>\n";
  $document .= "    <![endif]-->\n";
  $document .= "    <script type='text/javascript' src='" . $jquery_local . "'></script>\n";
  // $document .= "    <script type='text/javascript' src='" . $include_remote . "'></script>\n";
  $document .= "    <script type='text/javascript' src='" . $authentication_remote . "'></script>\n";
  $document .= "    <script type='text/javascript' src='" . $authentication_local . "'></script>\n";
  $document .= "  </head>\n";
  $document .= "  <body>\n";
  $document .= "    <header id='header'>\n";
  $document .= "      <h1><a class='' target='_blank' href='/'>" . variable_get('site_name', 'Drupal') . "</a></h1>\n";
  $document .= "    </header>\n";
  $document .= "    <div id='content'>\n";
  $document .= "      <section id='formWrap'>\n";
  $document .= "      <div class='form_section'>\n";
  $document .= drupal_render($form);
  $document .= "      </div>\n";
  $document .= "      </section>\n";
  $document .= "    </div>\n";
  $document .= "    <footer>\n";
  $document .= "    </footer>\n";
  $document .= "  </body>\n";
  $document .= "</html>\n";

  // if ($_SERVER['REQUEST_TYPE'] === 'POST') {
  //   persona_idp_api_login();
  // }

  print $document;
  drupal_exit();
}

/**
 * Persona Provision; Menu callback
 *
 * This is not a user facing page
 */
function persona_idp_provisioning() {
  $signature_service = variable_get('persona_idp_signature_service', 'http://127.0.0.1:9999/cert_key');
  $jquery = drupal_get_library('system', 'jquery');
  $jquery_local = url(key($jquery['js']));
  $include_remote = 'https://login.dev.anosrep.org/include.js';
  $provision_remote = 'https://login.dev.anosrep.org/provisioning_api.js';
  $provision_local = url(drupal_get_path('module', 'persona_idp') . '/js/persona_idp_provisioning.js');

  $document = "<!DOCTYPE html>\n";
  $document .= "<html>\n";
  $document .= "  <head>\n";
  $document .= "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
  $document .= "    <script type='text/javascript' src='" . $jquery_local . "'></script>\n";
  // $document .= "    <script type='text/javascript' src='" . $include_remote . "'></script>\n";
  $document .= "    <script type='text/javascript' src='" . $provision_remote . "'></script>\n";
  $document .= "    <script type='text/javascript' src='" . $provision_local . "'></script>\n";
  $document .= "  </head>\n";
  $document .= "</html>\n";

  print $document;
  drupal_exit();
}

/**
 * Persona api login; Menu callback
 */
function persona_idp_api_login() {
  global $user;

  $document = array(
    'success' => (boolean) $user->uid,
    'messages' => drupal_get_messages(NULL, TRUE),
    'data' => $_REQUEST,
  );
  drupal_add_http_header('Status', '200 OK');
  drupal_json_output($document);
  drupal_exit();
}

/**
 * Persona api whoami; Menu callback
 */
function persona_idp_api_whoami() {
  global $user;
  $document = array(
    'mail' => $user->mail
  );
  drupal_add_http_header('Status', '200 OK');
  drupal_json_output($document);
  drupal_exit();
}

/**
 * Persona api cert key; Menu callback
 */
function persona_idp_api_cert_key() {
  $url = variable_get('persona_idp_signature_service', 'http://127.0.0.1:8080/cert_key');
  $payload = array(
    'duration' => (integer) $_POST['duration'],
    'pubkey' => (string) $_POST['pubkey'],
    'email' => (string) $_POST['email'],
  );
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
    ),
    'method' => 'POST',
    'data' => drupal_json_encode($payload),
  );
  $response = drupal_http_request($url, $options);

  watchdog('persona-idp', '@request -> @response', array(
    '@request' => print_r($options, 1), '@response' => print_r($response, 1),
  ));

  drupal_add_http_header('Status', $response->code . ' ' . $response->status_message);
  foreach ($response->headers as $name => $value) {
    drupal_add_http_header($name, $value);
  }
  print $response->data;
  drupal_exit();
}
