<?php

//update Sensor data in database

include_once("requester.php");

$id = (int)$_REQUEST["id"];
$chartVisibility = (int)$_REQUEST["chartVisibility"];

$requester = new Requester;
$requester->updateData("UPDATE WeatherSensor SET ChartVisibility = $chartVisibility WHERE ID = $id");
$allData = $requester->getData("SELECT * FROM WeatherSensor");

print json_encode($allData, JSON_UNESCAPED_UNICODE);

?>