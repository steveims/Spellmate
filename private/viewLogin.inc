<html>
<head>
<title>Spell : Login</title>
<link rel=StyleSheet href="style.css" type="text/css">
</head>
<body>

<h3>Welcome to Spellmate</h3>

<?php
	if ( isset($message) ) {
		echo "<div class=\"warning\">" . $message . "</div>";
	}
?>

Please login to get started.<p>

<form name="f" action="action.php" method="POST">
<input type="hidden" name="action" value="login" />

<table>
<tr><td>Username</td><td><input name="username" type="text" size="32"/></td></tr>
<tr><td>Password</td><td><input name="password" type="password" size="32" /></td></tr>
<tr><td colspan="2"><center><input type="submit" value="Login" /></center></td></tr>
</table>
</form>

<script language="JavaScript"><!--
document.f.username.focus();
//--></script>

<hr>
<font size="-3">Questions?  Please contact <a href="mailto:steve_ims@yahoo.com">steve_ims@yahoo.com</a>.</font>

</body></html>
