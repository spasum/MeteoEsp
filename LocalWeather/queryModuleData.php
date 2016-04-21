<?php

//request Module data from database

include_once("requester.php");

$requester = new Requester;
$allData = $requester->getData("SELECT * FROM WeatherModule");

print json_encode($allData, JSON_UNESCAPED_UNICODE);

?>