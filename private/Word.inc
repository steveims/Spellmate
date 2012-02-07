<?php

class Word {
	private $metadata;

	public function Word($id) {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT words.*, categories.Category, levels.Level FROM words, categories, levels " .
						"WHERE words.Id = '%s' " .
						"AND words.Category_Id = categories.Id " .
						"AND words.Level_Id = levels.Id", mysql_real_escape_string($id));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$this->metadata = mysql_fetch_object($result);

		// Free resultset
		mysql_free_result($result);
	}
	
	public function getSpelling() {
		return($this->metadata->Word);
	}
	
	public function getAudioFile() {
		return($this->metadata->Audio_File);
	}
	
	public function getCategory() {
		return ($this->metadata->Category);
	}
	
	public function getLevel() {
		return ($this->metadata->Level);
	}
	
	public function getId() {
		return($this->metadata->Id);
	}
}
?>
