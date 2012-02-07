<html>
<head>
<title>Spell : Home</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>
<body>

<h2>Welcome to Spellmate</h2>

<hr>

<br>

<?php
if(!$user->getPendingList()->isEmpty()) {
?>

<div style="background-color: #88ff88; padding: 6px;">

<?php
	if($user->getPendingList()->isBookList()) {
		$word = $user->getPendingList()->getNextWord();
?>

<h3>I want to resume spelling words from where I left off ...</h3>

<blockquote>
I left off in the midst of a list from the book, category <?php echo $word->getCategory() ?> at level <?php echo $word->getLevel() ?>.

<p>


<form action="action.php" method="POST">
<input type="hidden" name="action" value="spellFromBookList" />
<input type="hidden" name="resumeId" value="<?php echo $word->getId() ?>" />
<input type="hidden" name="category" value="<?php echo $word->getCategory() ?>" />
<input type="hidden" name="level" value="<?php echo $word->getLevel() ?>" />
<input type="submit" value="Click here to start" />
</form>

<?php
	} else if($user->getPendingList()->isUserList()) {
		$word = $user->getPendingList()->getNextWord();
?>

<h3>I want to resume spelling words from where I left off ...</h3>

<blockquote>
I left off in the midst of my personal list.

<p>

<form action="action.php" method="POST">
<input type="hidden" name="action" value="spellFromUserList" />
<input type="hidden" name="resumeId" value="<?php echo $word->getId() ?>" />
<input type="submit" value="Click here to start" />
</form>

<?php
	}
?>

</div>

<p>

<?php
}
?>

<div style="background-color: #ffff88; padding: 6px;">
<h3>I want to practice spelling all the words from a category in the book ...</h3>

<blockquote>
<form action="action.php" method="POST">
<input type="hidden" name="action" value="spellFromBookList" />

First select the category 
  <select name="category" size="1">

<?php
$categories = getCategories();
foreach($categories as $category) {
	echo "<option value=\"" . $category . "\">" . $category . "</option>";
}
?>

  </select>
  
and the level
  <select name="level" size="1">
  
<?php
$levels = getLevels();
if(isset($_SESSION['lastLevel'])) {
	$lastLevel = $_SESSION['lastLevel'];
}
foreach($levels as $level) {
	if(isset($lastLevel) && ($lastLevel == $level)) {
		echo "<option value=" . $level . " selected>" . $level . "</option>";
	} else {
		echo "<option value=" . $level . ">" . $level . "</option>";
	}
}
?>

  </select>

then <input type="submit" value="Click here to start" />

</form>
</blockquote>
</div>


<?php if($user->getUserList()->length() > 0) { ?>

<p>

<div style="background-color: #8888ff; padding: 6px;">
<h3>I want to practice spelling all the words from "My Practice List" ...</h3>

<blockquote>
<form action="action.php" method="POST">
<input type="hidden" name="action" value="spellFromUserList" />
<input type="submit" value="Click here to start" />
</form>
</blockquote>
</div>

<?php } ?>

<p>

<div style="background-color: #FFA566; padding: 6px;">
<h3>I want to review my progress ...</h3>

<blockquote>
<form action="action.php" method="GET">
<input type="hidden" name="action" value="viewReport" />
<input type="submit" value="View my statistics" />
</form>

<form action="action.php" method="GET">
<input type="hidden" name="action" value="viewPracticeList" />
<input type="submit" value="View My Practice List" />
</form>

<form action="action.php" method="GET">
<input type="hidden" name="action" value="modifyPracticeList" />
<input type="submit" value="Modify My Practice List" />
</form>
</blockquote>
</div>

<br><br>

<hr>

<a href="action.php?action=logout">Quit.</a>

</body>
</html>
