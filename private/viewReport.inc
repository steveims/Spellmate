<?php 
	$categories = getCategories();
	$levels = getLevels();
?>

<html>
<head>
<title>Spell : Report</title>
<link rel=StyleSheet href="style.css" type="text/css">
<link rel=StyleSheet href="table.css" type="text/css">
</head>

<body>
<table cellspacing="1" cellpadding="6">

<?php
// First row:  Headings
echo "<tr><td></td>";
foreach($levels as $level) {
	echo "<th>" . $level . "</th>";
}
echo "</tr>";

// Middle rows:  Individual categories.
foreach($categories as $category) {
	echo "<tr><th align=left>" . $category . "</th>";
	
	foreach($levels as $level) {
		echo "<td>" . $user->getCorrect($category, $level) . " / " . $user->getTries($category, $level) . "</td>";
	}

	echo "</tr>";
}

// Last row:  Summary of all categories
echo "<tr><th align=left style=\"background-color:#8888ff;\">Total for all categories</th>";
foreach($levels as $level) {
	echo "<td style=\"background-color:#8888ff;\">" . $user->getCorrect(null, $level) . " / " . $user->getTries(null, $level) . "</td>";
}
echo "</tr>";

?>

</table>

<br><br><br>

<hr>

<a href="action.php?action=returnHome">Return to the main page.</a>

</body>
</html>
  