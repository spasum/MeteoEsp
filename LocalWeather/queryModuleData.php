<?php

//request Weather data from database

include_once("dbconfig.php");

$sortBy = $_REQUEST["sortBy"];
$sortAscending = ($_REQUEST["sortAscending"] == "true") ? "ASC" : "DESC";
$pageIndex = (int)$_REQUEST["pageIndex"];
$pageSize = (int)$_REQUEST["pageSize"];
$queryType = $_REQUEST["queryType"];
$interval = $_REQUEST["interval"];

$link = mysql_connect($databaseHost, $databaseLogin, $databasePassword)
    or die("Could not connect : " . mysql_error());

mysql_select_db($databaseName)
    or die("Could not select database");

if ($queryType == "all") {
    $rowsToSkip = $pageIndex * $pageSize;
    $query = "SELECT SQL_CALC_FOUND_ROWS * FROM WeatherData ORDER BY $sortBy $sortAscending LIMIT $rowsToSkip, $pageSize";
} else {
    $query = "SELECT * FROM WeatherData WHERE DATE_SUB(NOW(), INTERVAL $interval) < MeasuredDateTime";
}

$queryRowsCount = "SELECT FOUND_ROWS()";

$result = mysql_query($query)
    or die("Query failed: " . mysql_error());

$resultRowsCount = mysql_query($queryRowsCount)
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
    $weatherData = (object)[];
    $i = 0;
    foreach ($line as $col_value)
    {
        $columnName = $fieldsArray[$i]["name"];
        $columnType = $fieldsArray[$i]["type"];

        if ($col_value == null)
        {
            $weatherData->$columnName = null;
        }
        else {
            if ($columnType == "string")
                $weatherData->$columnName = iconv('windows-1251', 'utf-8', $col_value);
            if ($columnType == "real")
                $weatherData->$columnName = (float)$col_value;
            if ($columnType == "int")
                $weatherData->$columnName = (int)$col_value;
            if ($columnType == "timestamp")
                $weatherData->$columnName = $col_value;
        }

        $i++;
    }
    array_push($dataArray, $weatherData);
}

$rowsCount = 0;
while ($line = mysql_fetch_array($resultRowsCount, MYSQL_ASSOC))
{
    foreach ($line as $col_value)
    {
        $rowsCount = $col_value;
        break;
    }
}

$allData = array(
    "fields" => $fieldsArray,
    "data" => $dataArray,
    "sortBy" => $sortBy,
    "sortAscending" => $sortAscending == "ASC",
    "pageIndex" => $pageIndex,
    "pageSize" => $pageSize,
    "rowsCount" => (int)$rowsCount
);

print json_encode($allData, JSON_UNESCAPED_UNICODE);

mysql_free_result($result);
mysql_close($link);

?>