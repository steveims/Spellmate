<?php
require_once('Word.inc');
?>

<html>
<head>
<title>Spell : Practice List</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>

<body>

Practice list for <?php echo $user->getUsername(); ?><p>

<?php
$userListWords = $user->getUserList()->toArray();

foreach($userListWords as $word) {
	echo $word . "<br>";
}

?>

<br><br><br>

<hr>

<a href="action.php?action=returnHome">Return to the main page.</a>

</body>
</html>