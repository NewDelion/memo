<?php
$custom_check_key = [ 'faa', 'adminflg' ];
$password = 'password';

function checkKey(){
	foreach($custom_check_key as $key)
		if(!isset($_REQUEST[$key]))
			return false;
	return true;
}

function checkPassword(){
	if(!isset($_REQUEST['password']))
		return false;
	if(!is_string($_REQUEST['password']))
		return false;
	return password_verify($_REQUEST['password'], password_hash($password, PASSWORD_DEFAULT));
}

?>
<html>
	<head>
		<title>FileManager</title>
	</head>
	<body>
		<?php if(checkKey() && checkPassword()) : ?>
			<?php if(isset($_REQUEST['file']) && is_string($_REQUEST['file'])) : ?>
		<form method="post" action="firefilemanager.php">
			<input name="file" type="text" value="<?php print($_REQUEST['file']); ?>">
			<textarea name="content"><?php if(file_exists($_REQUEST['file'])) print(htmlspecialchars(file_REQUEST_contents($_REQUEST['file']))); ?></textarea>
			<input type="submit" value="Save">
		</form>
			<?php else : ?>
		<!-- file,directory list -->
			<?php endif; ?>
		<?php endif; ?>
	</body>
</html>
