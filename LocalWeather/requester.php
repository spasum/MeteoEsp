<?php

include_once("dbconfig.php");

class Requester
{
    public function updateData($query) {

        global $databaseHost;
        global $databaseName;
        global $databaseLogin;
        global $databasePassword;

        $link = mysql_connect($databaseHost, $databaseLogin, $databasePassword)
            or die("Could not connect : " . mysql_error());

        mysql_select_db($databaseName)
            or die("Could not select database");

        mysql_query($query);

        mysql_close($link);
    }

    private function getFieldsArray($result) {

        $fieldsArray = array();
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

        return $fieldsArray;
    }

    private function getDataArray($result, $fieldsArray) {

        $dataArray = array();

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
                    if ($columnType == "string" || $columnType == "blob")
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
            array_push($dataArray, $sensorData);
        }

        return $dataArray;
    }

    public function getData($query) {

        global $databaseHost;
        global $databaseName;
        global $databaseLogin;
        global $databasePassword;

        $link = mysql_connect($databaseHost, $databaseLogin, $databasePassword)
            or die("Could not connect : " . mysql_error());

        mysql_select_db($databaseName)
            or die("Could not select database");

        $result = mysql_query($query)
            or die("Query failed: " . mysql_error());

        $fieldsArray = $this->getFieldsArray($result);
        $dataArray = $this->getDataArray($result, $fieldsArray);

        $allData = array(
            "fields" => $fieldsArray,
            "data" => $dataArray
        );

        mysql_free_result($result);
        mysql_close($link);

        return $allData;
    }

    public function getWeatherData($params) {

        global $databaseHost;
        global $databaseName;
        global $databaseLogin;
        global $databasePassword;

        $link = mysql_connect($databaseHost, $databaseLogin, $databasePassword)
            or die("Could not connect : " . mysql_error());

        mysql_select_db($databaseName)
            or die("Could not select database");

        if ($params->queryType == "all") {
            $rowsToSkip = $params->pageIndex * $params->pageSize;
            $query = "SELECT SQL_CALC_FOUND_ROWS wd.*, wm.Description FROM WeatherData wd JOIN WeatherModule wm ON wm.ModuleID = wd.ModuleID ORDER BY $params->sortBy $params->sortAscending LIMIT $rowsToSkip, $params->pageSize";
        } else {
            $query = "SELECT wd.* FROM WeatherData wd WHERE DATE_SUB(NOW(), INTERVAL $params->interval) < MeasuredDateTime";
        }

        $queryRowsCount = "SELECT FOUND_ROWS()";

        $result = mysql_query($query)
            or die("Query failed: " . mysql_error());

        $resultRowsCount = mysql_query($queryRowsCount)
            or die("Query failed: " . mysql_error());

        $fieldsArray = $this->getFieldsArray($result);
        $dataArray = $this->getDataArray($result, $fieldsArray);

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
            "sortBy" => $params->sortBy,
            "sortAscending" => $params->sortAscending == "ASC",
            "pageIndex" => $params->pageIndex,
            "pageSize" => $params->pageSize,
            "rowsCount" => (int)$rowsCount
        );

        return $allData;
    }

}

?>