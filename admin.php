<?php

session_start();

require_once ('private/utils.inc');
require_once ('private/User.inc');

$user = new User();
$link = getDbConnection();

if(!$_SESSION['adminuser']) {
  $action = "login";
} else if(isset($_POST['action'])) {
  $action = $_POST['action'];
} else if(isset($_GET['action'])) {
  $action = $_GET['action'];
} else {
  $action = "login";
}

switch ($action) {
 case 'login' :
   if (!isset($_POST['password'])) {
     // If credentials not supplied, then return login form.
     require('private/adminLogin.inc');
     
   } else if ($_POST['password'] != 'un2Him') {
     $message = 'Invalid password.';
     require('private/adminLogin.inc');
     
   } else {
     $_SESSION['adminuser'] = true;
     require('private/adminHome.inc');
   }

   break;

 case 'logout' :
   session_destroy();

   require('private/adminLogin.inc');
		
   break;
		
 case 'adduser' :
   if (($_POST['username'] == '') || ($_POST['password'] == '')) {
     $message = "Username and Password are required.";

   } else {
     $iQuery = sprintf("INSERT INTO users (User, Password) VALUES ( '%s', '%s' )", mysql_real_escape_string($_POST['username']), mysql_real_escape_string($_POST['password']));
     $result = mysql_query($iQuery, $link);
     if (!$result) {
       $message = "Username must be unique (" . mysql_error() . ")";
     } else {
       $message = "Added user " . $_POST['username'];
     }
   }

   require('private/adminHome.inc');

   break;
		
 case 'setPassword' :
   if ($_POST['password'] == '') {
     $message = "Password cannot be blank.";

   } else {
     $iQuery = sprintf("UPDATE users SET Password='%s' WHERE User='%s'", mysql_real_escape_string($_POST['password']), mysql_real_escape_string($_POST['username']));
     $result = mysql_query($iQuery, $link);
     if (!$result) {
       $message = "Failed to set password (" . mysql_error() . ")";
     } else {
       $message = "Password updated for " . $_POST['username'];
     }
   }

   require('private/adminHome.inc');

   break;
		
 default :
   echo "Unknown action: " . $action;
 }
?>
