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

<form name="f" action="admin.php" method="POST">
<input type="hidden" name="action" value="login" />

<table>
<tr><td>Password</td><td><input name="password" type="password" size="32" /></td></tr>
<tr><td colspan="2"><center><input type="submit" value="Login" /></center></td></tr>
</table>
</form>

<script language="JavaScript"><!--
document.f.password.focus();
//--></script>

</body></html>
