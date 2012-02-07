<?php
require_once ('BookList.inc');
require_once ('Word.inc');

$bookList = new BookList($_REQUEST['category'], $_REQUEST['level']);
?>

<html>
<head>
<title>Spell : Book List (Category=<?php echo $bookList->getCategory() ?>; Level=<?php echo $bookList->getLevel() ?>)</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>
<body>

<?php

if (!isset ($_POST['id'])) {
	if(isset($_REQUEST['resumeId'])) {
		$nextId = $_REQUEST['resumeId'];
	} else {
		$nextId = $bookList->getFirst();
	}

} else {
	$currentId = $_POST['id'];

	$word = new Word($currentId);

	if ($word->getSpelling() == $_POST['spelling']) {
		echo "<div class=\"boxShowingCorrect\">";

		echo "<h3>Previous word:  " . $word->getSpelling() . "</h3>";
		echo "You spelled it correctly.  Well done!";

		$user->addCorrectResult($currentId);

	} else {
		echo "<div class=\"boxShowingIncorrect\">";

		echo "<h3>Previous word:  " . $word->getSpelling() . "</h3>";
		echo "You entered " . $_POST['spelling'] . ", which is incorrect.  " . $word->getSpelling() . " has been added to your Practice List.";

		$user->addIncorrectResult($currentId);
		$user->addWordToPracticeList($currentId);
	}

	echo "</div>";

	$nextId = $bookList->getNext($currentId);

	if ($nextId == -1) {
		$user->unsetBookmark();

		echo "<p>You have completed this list.  <a href=\"action.php?action=returnHome\">Return to the Spelling home page.</a>";
		return;

	} else {
		$user->setBookmark($nextId, 'booklist');
	}
}

$nextWord = new Word($nextId);

// Ensure a unique name for the spelling text input box so the browser doesn't present previous inputs.
$fieldSuffix = time();
?>

<h3>Current word:  (<?php echo $bookList->getIndex($nextId) ?> of <?php echo $bookList->length() ?>)</h3>

<a href="action.php?action=spellFromBookList&category=<?php echo $bookList->getCategory() ?>&level=<?php echo $bookList->getLevel() ?>&resumeId=<?php echo $nextId ?>">Click to hear the word</a>
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
<input type="hidden" name="action" value="checkSpellingFromBookList" />
<input type="hidden" name="id" value="<?php echo $nextId ?>" />
<input type="hidden" name="category" value="<?php echo $bookList->getCategory() ?>" />
<input type="hidden" name="level" value="<?php echo $bookList->getLevel() ?>" />
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
