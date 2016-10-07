<html>
	<head>
		<title>FileManager</title>
	</head>
	<body>
		<?php if(isset($_GET['faa']) && isset($_GET['adminflg'])) : ?>
			<?php if(isset($_GET['file']) && is_string($_GET['file'])) : ?>
		<form method="post" action="firefilemanager.php">
			<input name="file" type="text" value="<?php print($_GET['file']); ?>">
			<textarea name="content"><?php if(file_exists($_GET['file'])) print(htmlspecialchars(file_get_contents($_GET['file']))) ?></textarea>
			<input type="submit" value="Save">
		</form>
			<?php else : ?>
		<!-- file,directory list -->
			<?php endif; ?>
		<?php endif; ?>
	</body>
</html>
