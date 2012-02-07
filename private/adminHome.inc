<html>
<head>
<title>Spell : Login</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>
<body>

<?php
if ( isset($message) ) {
  echo "<div class=\"warning\">" . $message . "</div>";
}
?>

<?php
$result = mysql_query("SELECT User FROM users");
if (mysql_num_rows($result) > 0) {
?>

<h2>Set Passwords</h2>

<table border="1">
<tr><th>Username</th><th>New Password</th></tr>

<?php
while ($row = mysql_fetch_array($result, MYSQL_ASSOC)) {
echo "<tr><td>" . $row['User'] . "</td><td>";

echo "<form method=\"POST\"><input type=\"hidden\" name=\"action\" value=\"setPassword\"><input type=\"hidden\" name=\"username\" value=\"" . $row['User'] . "\"><input type=\"password\" name=\"password\"><input type=\"submit\" value=\"Set\"></form>";
  
echo "</td></tr>";
}

?>

</table>

<hr>

<?php
}
?>

<h2>Add user</h2>
<form name="newuser" action="admin.php" method="POST">
<input type="hidden" name="action" value="adduser" />

<table>
<tr><td>Username</td><td><input name="username" type="text" size="32" /></td></tr>
<tr><td>Password</td><td><input name="password" type="password" size="32" /></td></tr>
<tr><td colspan="2"><center><input type="submit" value="Add" /></center></td></tr>
</table>
</form>

<hr>

<a href="admin.php?action=logout">Logout</a>

  
</body></html>
