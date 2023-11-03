<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta name="author" content="Charlie Shapiro">
        <title>VRS CA ROOM DATA</title>
        <meta name="description" content="This page shows the current room data being logged at VRS in their CA rooms">
        <link href="style.css" rel="stylesheet" type="text/css" media="screen" />
    </head>
    <body>
    	<header>
    		<h1>VRS CA ROOM DATA</h1>
    	</header>

        <?php

        mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

        echo "good";

        $mysqli = new mysqli("127.0.0.1", "root", "232416cpstne$SEED", "database", 3306);

        echo "still good";

        echo $mysqli->host_info . "\n";

        $mysqli->query("DROP TABLE IF EXISTS test");
        $mysqli->query("CREATE TABLE test(id INT)");

        ?>

    </body>
</html>