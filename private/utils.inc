<?php
function getDbConnection() {
	// Production database.
	$link = mysql_connect('localhost', 'sabslorg_dbadmin', 'forHim') or die('Could not connect: ' . mysql_error());

	mysql_select_db('sabslorg_db1', $link) or die('Could not select database: ' . mysql_error());
	
	return ($link);
}

function getLevels() {
	$link = getDbConnection();

	// Performing SQL query
	$query = sprintf("SELECT * FROM levels");
	$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

	while ($row = mysql_fetch_assoc($result)) {
		$levels[] = $row["Level"];
	}

	// Free resultset
	mysql_free_result($result);

	return ($levels);
}

function getCategories() {
	$link = getDbConnection();

	// Performing SQL query
	$query = sprintf("SELECT * FROM categories");
	$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

	while ($row = mysql_fetch_assoc($result)) {
		$categories[] = $row["Category"];
	}

	// Free resultset
	mysql_free_result($result);

	return ($categories);
}

?> 
