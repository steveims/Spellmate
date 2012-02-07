<?php

session_start();

require_once ('private/utils.inc');
require_once ('private/User.inc');

$user = new User();
$link = getDbConnection();

if(!$user->isAuthenticated()) {
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
		if (!isset($_POST['username']) || !isset($_POST['password'])) {
			// If credentials not supplied, then return login form.
			require('private/viewLogin.inc');
			
		} else {
			// Check login credentials.
			$user->authenticate($_POST['username'], $_POST['password']);
			
			if (!$user->isAuthenticated()) {
				$message = 'Invalid username and/or password.';
				require('private/viewLogin.inc');

			} else {
				require('private/viewHome.inc');
			}
		}

		break;

	case 'logout' :
		session_destroy();
		
		require('private/viewLogin.inc');
		
		break;
		
	case 'spellFromBookList' :
		$_SESSION['lastLevel'] = $_POST['level'];
		
		require('private/viewSpellBookList.inc');
		
		break;
		
	case 'checkSpellingFromBookList' :
		require('private/viewSpellBookList.inc');
		
		break;
		
	case 'spellFromUserList' :
		require('private/viewSpellUserList.inc');
		
		break;
		
	case 'checkSpellingFromUserList' :
		require('private/viewSpellUserList.inc');
		
		break;
		
	case 'returnHome' :
		require('private/viewHome.inc');
		
		break;
		
	case 'viewReport' :
		require('private/viewReport.inc');
	
		break;
		
	case 'viewPracticeList' :
		require('private/viewPracticeList.inc');
		
		break;
		
	case 'modifyPracticeList' :
		require('private/viewModifyPracticeList.inc');
		
		break;
		
	case 'updatePracticeList' :
		foreach($_POST as $key => $val) {
			if(strpos($key, 'word_') === 0) {
				$newPracticeWords[] = substr($key, 5);			
			}
		}
		if(is_array($newPracticeWords)) {
			$user->replacePracticeList($newPracticeWords);
		} else {
			$user->replacePracticeList(array());
		}
	
		require('private/viewModifyPracticeList.inc');
		
		break;
		
	default :
		echo "i is not equal to 0, 1 or 2";
}
?>
