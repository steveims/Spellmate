<?php
class UserList {
	private $user;

	public function UserList($user) {
		$this->user = $user;
	}

	public function length() {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM userList WHERE User_Id = %d", $this->user->getId());
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$length = mysql_num_rows($result);

		// Free resultset
		mysql_free_result($result);

		return ($length);
	}

	/**
	 * Returns Id of the first word on the userlist, or -1 if the userlist is empty.
	 */
	public function getFirst() {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM userList WHERE User_Id = %d ORDER BY Word_Id", $this->user->getId());
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		if(mysql_num_rows($result) == 0) {
			$id = -1;
		} else {
			$row = mysql_fetch_assoc($result);
			$id = $row["Word_Id"];
		}

		// Free resultset
		mysql_free_result($result);

		return ($id);
	}

	public function getNext($wordId) {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM userList WHERE User_Id = %d AND Word_Id > %d ORDER BY Word_Id", $this->user->getId(), mysql_real_escape_string($wordId));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		if(mysql_num_rows($result) == 0) {
			$id = -1;
		} else {
			$row = mysql_fetch_assoc($result);
			$id = $row["Word_Id"];
		}

		// Free resultset
		mysql_free_result($result);

		return ($id);
	}
	
	public function getIndex($wordId) {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM userList WHERE User_Id = %d AND Word_Id <= %d ORDER BY Word_Id", $this->user->getId(), mysql_real_escape_string($wordId));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$index = mysql_num_rows($result);

		// Free resultset
		mysql_free_result($result);

		return ($index);
	}
	
	public function toArray() {
		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM userList, words WHERE User_Id = %d AND userList.Word_Id = words.Id ORDER BY Word_Id", $this->user->getId());
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());
		
		$words = array();

		while ($row = mysql_fetch_assoc($result)) {
			$words[] = $row["Word"];
		}

		// Free resultset
		mysql_free_result($result);

		return ($words);
	}
}
?>
