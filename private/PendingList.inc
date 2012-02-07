<?php

require_once('Word.inc');

class PendingList {
	private $user;
	private $record = null;
	
	function PendingList($user) {
		$this->user = $user;
		
		$link = getDbConnection();
		
		// Performing SQL query
		$query = sprintf("SELECT * FROM bookmarks WHERE User_Id = %d", $this->user->getId());
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());
		
		if(mysql_num_rows($result) != 0) {
			$this->record = mysql_fetch_object($result);
		}

		// Free resultset
		mysql_free_result($result);
	}
	
	function isEmpty() {
		return($this->record == null);
	}
	
	function isBookList() {
		return(!$this->isEmpty() && ($this->record->Type == 'booklist'));
	}
	
	function isUserList() {
		return(!$this->isEmpty() && ($this->record->Type == 'userlist'));
	}
	
	function getNextWord() {
		return(new Word($this->record->Word_Id));
	}
}
?>
