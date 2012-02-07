<?php
require_once ('utils.inc');
require_once ('UserList.inc');
require_once ('PendingList.inc');

class User {
	private $username = null;
	private $id = null;

	// Cons a User from session data; if none, then make this an anonymous user.
	public function User() {
		// Unknown user; treat as anonymous and return.
		if (!isset ($_SESSION['username'])) {
			return;
		}

		// Known user; cache metadata.
		$username = $_SESSION['username'];

		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("SELECT * FROM users WHERE User = '%s'", mysql_real_escape_string($username));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		if (mysql_num_rows($result) != 1) {
			// Return null rather than throwing an exception to ensure that this code runs on PHP4 and PHP5.
			throw new Exception($username . ' is an unknown username.');

		} else {
			$row = mysql_fetch_assoc($result);
			$this->username = $row["User"];
			$this->id = $row["Id"];
		}

		// Free resultset
		mysql_free_result($result);
	}

	public function isAnonymous() {
		return ($this->username == null);
	}

	public function authenticate($username, $password) {
		$link = getDbConnection();

		// Performing SQL query
		// http://us3.php.net/function.mysql-query
		$query = sprintf("SELECT * FROM users WHERE User='%s' AND Password='%s'", mysql_real_escape_string($username), mysql_real_escape_string($password));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		$isValid = (mysql_num_rows($result) == 1);

		if ($isValid) {
			$row = mysql_fetch_assoc($result);
			$this->username = $row["User"];
			$this->id = $row["Id"];

			// Stow client info in the session.
			$_SESSION['username'] = $_POST['username'];
			$_SESSION['clientaddr'] = $_SERVER['REMOTE_ADDR'];
		}

		// Free resultset
		mysql_free_result($result);
	}

	public function isAuthenticated() {
		return (!$this->isAnonymous() && isset ($_SESSION['clientaddr']) && ($_SESSION['clientaddr'] == $_SERVER['REMOTE_ADDR']));
	}

	public function getUsername() {
		return ($this->username);
	}

	public function getId() {
		return ($this->id);
	}

	public function addCorrectResult($wordId) {
		$this->addResult($wordId, true);
	}

	public function addIncorrectResult($wordId) {
		$this->addResult($wordId, false);
	}

	private function addResult($wordId, $isCorrect) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to add result for an anonymous user.');
		}

		$link = getDbConnection();

		mysql_query("START TRANSACTION", $link);

		// Create an entry with zero counters, if none exists.
		$iQuery = sprintf("INSERT IGNORE INTO results VALUES ( %d, %d, 0, 0, 0 )", $this->id, mysql_real_escape_string($wordId));
		mysql_query($iQuery, $link) or die('Query failed: ' . mysql_error());

		// Increment the counters.
		if ($isCorrect) {
			$uQuery = sprintf("UPDATE results SET tries=tries+1, correct=correct+1, streak=streak+1
						   								WHERE User_Id = %d
						   								AND Word_Id = %d", $this->id, mysql_real_escape_string($wordId));

		} else {
			$uQuery = sprintf("UPDATE results SET tries=tries+1, streak=0
						   								WHERE User_Id = %d
						   								AND Word_Id = %d", $this->id, mysql_real_escape_string($wordId));
		}
		mysql_query($uQuery, $link) or die('Query failed: ' . mysql_error());

		mysql_query("COMMIT", $link);
	}

	public function addWordToPracticeList($wordId) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to modify practice list for an anonymous user.');
		}

		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("INSERT IGNORE INTO userList VALUES ( %d, %d )", $this->id, mysql_real_escape_string($wordId));
		mysql_query($query, $link) or die('Query failed: ' . mysql_error());
	}
	
	public function replacePracticeList($newPracticeWords) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to clearPracticeList for an anonymous user.');
		}

		$link = getDbConnection();
		
		mysql_query("START TRANSACTION", $link);

		// Performing SQL query
		$query = sprintf("DELETE FROM userList WHERE User_Id = %d", $this->id);
		mysql_query($query, $link) or die('Query failed: ' . mysql_error());
		
		foreach($newPracticeWords as $newPracticeWord) {
			$this->addWordToPracticeList($newPracticeWord);
		}

		mysql_query("COMMIT", $link);
	}

	public function setBookmark($wordId, $type) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to setBookmark for an anonymous user.');
		}

		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("REPLACE bookmarks VALUES ( %d, %d, '%s' )", $this->id, mysql_real_escape_string($wordId), mysql_real_escape_string($type));
		mysql_query($query, $link) or die('Query failed: ' . mysql_error());
	}

	public function unsetBookmark() {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to unsetBookmark for an anonymous user.');
		}

		$link = getDbConnection();

		// Performing SQL query
		$query = sprintf("DELETE FROM bookmarks WHERE User_Id = %d", $this->id);
		mysql_query($query, $link) or die('Query failed: ' . mysql_error());
	}
	
	public function getUserList() {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to get user list for an anonymous user.');
		}

		return(new UserList($this));
	}
	
	public function getPendingList() {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to get pending list for an anonymous user.');
		}
		
		return(new PendingList($this));
	}
	
	public function getTries($category, $level) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to getTries for an anonymous user.');
		}

		$link = getDbConnection();
		
		$query = sprintf("SELECT SUM(results.Tries) AS total FROM results, words, categories, levels " .
				"WHERE User_Id=%d " .
				"AND results.Word_Id=words.Id " .
				"AND words.Category_Id = categories.Id " .
				"AND words.Level_Id = levels.Id", $this->id);
		
		if($category != null) {
			$query .= sprintf(" AND categories.Category = '%s'", mysql_real_escape_string($category));
		}
		
		if($level != null) {
			$query .= sprintf(" AND levels.Level = '%s'", mysql_real_escape_string($level));
		}
		
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());
		$row = mysql_fetch_array($result);
		if(is_numeric($row['total'])) {
			$total = $row['total'];
			
		} else {
			$total = 0;
		}

		// Free resultset
		mysql_free_result($result);

		return($total);
	}

	public function getCorrect($category, $level) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to getCorrect for an anonymous user.');
		}

		$link = getDbConnection();
		
		$query = sprintf("SELECT SUM(results.Correct) AS total FROM results, words, categories, levels " .
				"WHERE User_Id=%d " .
				"AND results.Word_Id=words.Id " .
				"AND words.Category_Id = categories.Id " .
				"AND words.Level_Id = levels.Id", $this->id);
		
		if($category != null) {
			$query .= sprintf(" AND categories.Category = '%s'", mysql_real_escape_string($category));
		}
		
		if($level != null) {
			$query .= sprintf(" AND levels.Level = '%s'", mysql_real_escape_string($level));
		}
		
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());
		
		$row = mysql_fetch_array($result);
		if(is_numeric($row['total'])) {
			$total = $row['total'];
			
		} else {
			$total = 0;
		}

		// Free resultset
		mysql_free_result($result);

		return($total);
	}

	public function getWords($sortBy) {
		if ($this->isAnonymous()) {
			throw new Exception('Attempted to getWords for an anonymous user.');
		}

		$link = getDbConnection();
	
		$query = sprintf("SELECT Word, words.Id AS Id, Category, Level, Tries, Correct, Streak, NOT ISNULL(userList.User_Id) AS UserWord " .
				"FROM words " .
				"JOIN categories ON words.Category_Id = categories.Id " .
				"JOIN levels ON words.Level_Id = levels.Id " .
				"LEFT JOIN results " .
				"ON words.Id = results.Word_Id " .
				"AND results.User_Id = %d " .
				"LEFT JOIN userList " .
				"ON words.Id = userList.Word_Id " .
				"AND userList.User_Id = %d " .
				"ORDER BY %s" , $this->getId(), $this->getId(), mysql_real_escape_string($sortBy));
		$result = mysql_query($query, $link) or die('Query failed: ' . mysql_error());

		return($result);
	}
}
?>
