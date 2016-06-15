<?php

//update Module data in database

include_once("requester.php");

$mac = $_REQUEST["mac"];

$requester = new Requester;

if (isset($_REQUEST["description"])) {
    $description = $_REQUEST["description"];
    $description = iconv('utf-8', 'windows-1251', $description);
    $requester->updateData("UPDATE WeatherModule SET Description = '$description' WHERE MAC = '$mac'");
}

$allData = $requester->getData("SELECT * FROM WeatherModule");

print json_encode($allData, JSON_UNESCAPED_UNICODE);

?>