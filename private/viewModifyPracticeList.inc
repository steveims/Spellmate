<?php 
	$categories = getCategories();
	$levels = getLevels();
	
	if(isset($_REQUEST['level'])) {
		$lastLevel = $_REQUEST['level'];

	} else if(isset($_SESSION['lastLevel'])) {
		$lastLevel = $_SESSION['lastLevel'];

	} else {
		$lastLevel = $levels[0];
	}

	$sortByStrings = array (
		"word" => "Word ASC",
		"tries" => "Tries DESC, Correct DESC, Streak DESC, Word ASC",
		"userWord" => "UserWord DESC, Streak DESC, Word ASC",
		"category" => "Category ASC, Word ASC",
		"streak" => "Streak DESC, Tries DESC, Correct DESC, Word ASC",
		"correct" => "Correct DESC, Tries DESC, Streak DESC, Word ASC",
	);
	
	if(isset($_REQUEST['sortBy']) && array_key_exists($_REQUEST["sortBy"], $sortByStrings)) {
		$sortBy = $sortByStrings[$_REQUEST["sortBy"]];
		
	} else {
		$sortBy = $sortByStrings['userWord'];
	}
	
	$result = $user->getWords($sortBy);
?>

<html>
<head>
<title>Spell : Modify Practice List</title>
<link rel=StyleSheet href="style.css" type="text/css">
<link rel=StyleSheet href="table.css" type="text/css">

<script type="text/javascript">
<!--
var isDirty = false;
var oldSelectIndex;

function sort(sortBy) {
	var isOk = true;
	
	if(isDirty) {
		isOk=confirm("Resorting now will lose any unsaved changes in your Practice List.  Click 'Ok' to continue or 'Cancel' to have a chance to save your changes.");
	}
	
	if(isOk) {
	    parent.location.href='action.php?action=modifyPracticeList&sortBy=' + sortBy;
    	parent.location.load();
	}
}

function cacheCurrentSelect() {
	oldSelectIndex = document.words.level.selectedIndex;
}

function changeLevel() {
	var isOk = true;
	
	if(isDirty) {
		isOk=confirm("Changing level now will lose any unsaved changes in your Practice List.  Click 'Ok' to continue or 'Cancel' to have a chance to save your changes.");
	}
	
	box = document.words.level;
	level = box.options[box.selectedIndex].value;
	
	if(isOk) {
	    parent.location.href='action.php?action=modifyPracticeList<?php if(isset($_REQUEST['sortBy'])) { echo "&sortBy=" . $_REQUEST['sortBy']; } ?>&level=' + level;
    	parent.location.load();
    	
	} else {
		box.selectedIndex = oldSelectIndex;
		alert('set select to ' + oldSelectIndex);
	}
}

function setDirty() {
	isDirty = true;
}
// -->
</script>

</head>

<body>

<form name="words" action="action.php" method="POST">
<input type="hidden" name="action" value="updatePracticeList">

<?php
if(isset($_REQUEST['sortBy'])) {
?>
<input type="hidden" name="sortBy" value="<?php echo $_REQUEST['sortBy'] ?>" />
<?php
}
?>

Showing level <select name="level" size="1" onfocus="cacheCurrentSelect()" onchange="changeLevel()">
  
<?php
foreach($levels as $level) {
	if($lastLevel == $level) {
		echo "<option value=" . $level . " selected>" . $level . "</option>";
	} else {
		echo "<option value=" . $level . ">" . $level . "</option>";
	}
}
?>

  </select>
  
<p>
  
<input type="submit" value="Click to update contents of your Practice List" />

<p>

<a href="action.php?action=returnHome">Return to the main page.</a>

<p>

<table cellspacing="1" cellpadding="6">

<?php
$columnHeadings = "<tr><th><a href=\"javascript:sort('word');\">Word</a></th>" .
		"<th><a href=\"javascript:sort('category');\">Category</a></th>" .
		"<th><a href=\"javascript:sort('tries');\">Tries</a></th>" .
		"<th><a href=\"javascript:sort('correct');\">Correct</a></th>" .
		"<th><a href=\"javascript:sort('streak');\">Streak</a></th>" .
		"<th><a href=\"javascript:sort('userWord');\">In Practice List</a></th></tr>";

$counter = 0;
$repeatInterval = 30;

while ($row = mysql_fetch_assoc($result)) {
	if($row["Level"] == $lastLevel) {
		if($counter % $repeatInterval == 0) {
			echo $columnHeadings;
		}
		
		$counter += 1;

		echo "<tr>";
		echo "<td>" . $row["Word"] . "</td>";
		echo "<td>" . $row["Category"] . "</td>";
		echo "<td>" . $row["Tries"] . "</td>";
		echo "<td>" . $row["Correct"] . "</td>";
		echo "<td>" . $row["Streak"] . "</td>";
		if($row["UserWord"]) {
			echo "<td><input type=\"checkbox\" name=\"word_" . $row["Id"] . "\" onchange=\"setDirty();\" checked></td>";
		} else {
			echo "<td><input type=\"checkbox\" name=\"word_" . $row["Id"] . "\" onchange=\"setDirty();\"></td>";
		}
		echo "</tr>";
	}
}

?>

</table>

<?php
mysql_data_seek($result, 0);
while($row = mysql_fetch_assoc($result)) {
	if(($row["Level"] != $lastLevel) && ($row["UserWord"])) {
		echo "<input type=\"hidden\" name=\"word_" . $row["Id"] . "\" value=\"on\">";
	}
}
?>

<p>

<input type="submit" value="Click to update contents of your Practice List" />

</form>

<br><br><br>

<hr>

<a href="action.php?action=returnHome">Return to the main page.</a>

</body>
</html>
