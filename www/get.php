<table style="border: 1px solid gray;">

<?php

	$link = mysql_connect("localhost", "phpmyadmin", "raspberry")
		or die("Could not connect : " . mysql_error());

	mysql_select_db("phpmyadmin") or die("Could not select database");

	$sql = "SELECT * FROM climate WHERE measuredatetime < NOW() AND measuredatetime > NOW() - INTERVAL 1 HOUR";

	$result = mysql_query($sql)
		or die("Query failed : " . mysql_error());

	while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {

		print "<tr>";

		foreach ($line as $col_value) {
			print "<td style='border: 1px solid gray;'>$col_value</td>";
		}

		print "</tr>";
	}

	mysql_close($link);

?>

</table>