<?php
require_once ('Word.inc');
?>

<html>
<head>
<title>Spell : User List</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>
<body>

<?php
	if(!isset($_POST['id'])) {
		if(isset($_REQUEST['resumeId'])) {
			$nextId = $_REQUEST['resumeId'];
		} else {
			$nextId = $user->getUserList()->getFirst();
		}
		
	} else {
		$word = new Word($_POST['id']);
		
		if($word->getSpelling() == $_POST['spelling']) {
			echo "<div class=\"boxShowingCorrect\">";

			echo "<h3>Previous word:  " . $word->getSpelling() . "</h3>";
			echo "You spelled it correctly.  Well done!";
			
			$user->addCorrectResult($_POST['id']);
			
		} else {
			echo "<div class=\"boxShowingIncorrect\">";

			echo "<h3>Previous word:  " . $word->getSpelling() . "</h3>";
			echo "You entered ".$_POST['spelling'].", which is incorrect.";

			$user->addIncorrectResult($_POST['id']);
		}
		
		echo "</div>";

		$nextId = $user->getUserList()->getNext($_POST['id']);
		
		if($nextId == -1) {
			$user->unsetBookmark();
			
			echo "<p>You have completed this list.  <a href=\"action.php?action=returnHome\">Return to the Spelling home page.</a>";
			return;

		} else {
			$user->setBookmark($nextId, 'userlist');
		}
	}
	
	$nextWord = new Word($nextId);
	
	// Ensure a unique name for the spelling text input box so the browser doesn't present previous inputs.
	$fieldSuffix = time();
?>

<h3>Current word:  (<?php echo $user->getUserList()->getIndex($nextId) ?> of <?php echo $user->getUserList()->length() ?>)</h3>

<a href="action.php?action=spellFromUserList&resumeId=<?php echo $nextId ?>">Click to hear the word</a>
<EMBED SRC="<?php echo $nextWord->getAudioFile() ?>" HIDDEN="true" AUTOSTART="true">
<p>

<SCRIPT TYPE="text/javascript">
function updateSpellingValue()
{
	document.spellingForm.spelling.value = document.spellingForm.spelling<?php echo $fieldSuffix ?>.value;
	return(true);
}
</SCRIPT>

<form name="spellingForm" action="action.php" method="POST" onSubmit="return updateSpellingValue()">
<input type="hidden" name="action" value="checkSpellingFromUserList" />
<input type="hidden" name="id" value="<?php echo $nextId ?>" />
<input type="hidden" name="spelling" value="placeholder" />
Enter spelling here <input type="text" size="32" name="spelling<?php echo $fieldSuffix ?>" />
then
<input type="submit" value="Click here to submit" />
</form>

<script language="JavaScript"><!--
document.spellingForm.spelling<?php echo $fieldSuffix ?>.focus();
//--></script>

<br><br><br>

<hr>

<a href="action.php?action=returnHome">Return to the main page; I will finish this list later.</a>


</body>
</html>
