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

$query = "SELECT * FROM WeatherSensor WHERE ID = $id";
$result = mysql_query($query)
    or die("Query failed: " . mysql_error());

$fieldsArray = array();
$dataArray = array();

$i = 0;
while ($i < mysql_num_fields($result))
{
    $meta = mysql_fetch_field($result, $i);
    if (!$meta)
    {
    }
    else
    {
        $metaData = array(
            "name" => $meta->name,
            "type" => $meta->type
        );
        array_push($fieldsArray, $metaData);
    }
    $i++;
}

while ($line = mysql_fetch_array($result, MYSQL_ASSOC))
{
    $sensorData = (object)[];
    $i = 0;
    foreach ($line as $col_value)
    {
        $columnName = $fieldsArray[$i]["name"];
        $columnType = $fieldsArray[$i]["type"];

        if ($col_value == null)
        {
            $sensorData->$columnName = null;
        }
        else {
            if ($columnType == "string")
                $sensorData->$columnName = iconv('windows-1251', 'utf-8', $col_value);
            if ($columnType == "real")
                $sensorData->$columnName = (float)$col_value;
            if ($columnType == "int")
                $sensorData->$columnName = (int)$col_value;
            if ($columnType == "timestamp")
                $sensorData->$columnName = $col_value;
        }

        $i++;
    }
    array_push($dataArray, $moduleData);
}

$allData = array(
    "data" => $dataArray
);

print json_encode($allData, JSON_UNESCAPED_UNICODE);

mysql_free_result($result);
mysql_close($link);

?>