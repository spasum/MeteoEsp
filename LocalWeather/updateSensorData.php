<?php

//update Sensor data in database

include_once("dbconfig.php");

$id = (int)$_REQUEST["id"];
$chartVisibility = (int)$_REQUEST["chartVisibility"];

$link = mysql_connect($databaseHost, $databaseLogin, $databasePassword)
    or die("Could not connect : " . mysql_error());

mysql_select_db($databaseName)
    or die("Could not select database");

$query = "UPDATE WeatherSensor SET ChartVisibility = $chartVisibility WHERE ID = $id";
mysql_query($query);

$allData = array(
    "result" => true
);

print json_encode($allData, JSON_UNESCAPED_UNICODE);

mysql_close($link);

?>