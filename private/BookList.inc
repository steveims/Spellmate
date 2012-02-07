<?php

require_once ('Word.inc');

class BookList {
	private $category;
	private $level;

	public function BookList($category, $level) {
		$this->category = $category;
		$this->level = $level;
	}
	
	public function getCategory() {
		return($this->category);
	}
	
	public function getLevel() {
		return($this->level);
	}

	public function length() {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM words, categories, levels " .
		"WHERE words.Category_Id = categories.Id " .
		"AND categories.Category = '%s' " .
		"AND words.Level_Id = levels.Id " .
		"AND levels.Level = '%s'", mysql_real_escape_string($this->category), mysql_real_escape_string($this->level));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$length = mysql_num_rows($result);

		// Free resultset
		mysql_free_result($result);

		return ($length);
	}

	public function getFirst() {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT words.Id FROM words, categories, levels 
				                      WHERE words.Category_Id = categories.Id 
				                      AND categories.Category = '%s'
				                      AND words.Level_Id = levels.Id
				                      AND levels.Level = '%s'
				                      ORDER BY words.Id", mysql_real_escape_string($this->category), mysql_real_escape_string($this->level));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$row = mysql_fetch_assoc($result);
		$id = $row["Id"];

		// Free resultset
		mysql_free_result($result);

		return ($id);
	}

	public function getNext($wordId) {
		$word = new Word($wordId);

		$link = getDbConnection();
		
		// Performing SQL query
		$query = sprintf("SELECT words.Id FROM words, categories, levels " .
							"WHERE words.Category_Id = categories.Id " .
		                    "AND categories.Category = '%s' " .
		                    "AND words.Level_Id = levels.Id " .
		                    "AND levels.Level = '%s' " .
		                    "AND words.Id > %d " .
		                    "ORDER BY words.Id", 
		                      mysql_real_escape_string($word->getCategory()),
		                      mysql_real_escape_string($word->getLevel()),
		                      mysql_real_escape_string($wordId));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		if (mysql_num_rows($result) == 0) {
			$id = -1;
		} else {
			$row = mysql_fetch_assoc($result);
			$id = $row["Id"];
		}

		// Free resultset
		mysql_free_result($result);

		return ($id);
	}

	public function getIndex($wordId) {
		$word = new Word($wordId);

		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT words.Id FROM words, categories, levels " .
							"WHERE words.Category_Id = categories.Id " .
							"AND categories.Category = '%s' " .
							"AND words.Level_Id = levels.Id " .
							"AND levels.Level = '%s' " .
							"AND words.Id <= %d " .
							"ORDER BY words.Id", 
							mysql_real_escape_string($word->getCategory()), mysql_real_escape_string($word->getLevel()), mysql_real_escape_string($wordId));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$index = mysql_num_rows($result);

		// Free resultset
		mysql_free_result($result);

		return ($index);
	}
}
?>
