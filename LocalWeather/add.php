<?php

include_once("dbconfig.php");

function getParam($object, $name)
{
	if (isset($object[$name]) && !empty($object[$name]))
		return $object[$name];
	return null;
}

function valueOrNull($value)
{
	if ($value == 0)
		return 'null';
	return $value;
}

$id = 0;
$input = file_get_contents('php://input');

if (empty($input))
{
	$data = array(
		'error' => 'Please use JSON to add data.'
	);
	print json_encode($data);
	return;
}

$object = json_decode($input, true);

$moduleid = (int)getParam($object, "moduleid");
$modulename = getParam($object, "modulename");

$meet = (int)getParam($object, "meet") == 1;
$ip = getParam($object, "ip");
$mac = getParam($object, "mac");

$temperature1 = valueOrNull((float)getParam($object, "temperature1"));
$humidity1 = valueOrNull((float)getParam($object, "humidity1"));
$pressure1 = valueOrNull((float)getParam($object, "pressure1"));

$temperature2 = valueOrNull((float)getParam($object, "temperature2"));
$humidity2 = valueOrNull((float)getParam($object, "humidity2"));
$pressure2 = valueOrNull((float)getParam($object, "pressure2"));

$temperature3 = valueOrNull((float)getParam($object, "temperature3"));
$humidity3 = valueOrNull((float)getParam($object, "humidity3"));
$pressure3 = valueOrNull((float)getParam($object, "pressure3"));

$illumination = valueOrNull((float)getParam($object, "illumination"));

if ($moduleid == 0) {
	$data = array(
		'error' => 'Wrong module ID.'
	);
	print json_encode($data);
	return;
}

try
{
	$link = mysql_connect($databaseHost, $databaseLogin, $databasePassword);
}
catch (Exception $e)
{
	$data = array(
		'error' => mysql_error()
	);
	print json_encode($data);
	return;
}

$sql = "INSERT INTO WeatherData (ModuleID, ModuleName, Temperature1, Temperature2, Temperature3, Humidity1, Humidity2, Humidity3, Pressure1, Pressure2, Pressure3, Illumination) VALUES ($moduleid, '$modulename', $temperature1, $temperature2, $temperature3, $humidity1, $humidity2, $humidity3, $pressure1, $pressure2, $pressure3, $illumination)";
try
{
	mysql_select_db($databaseName);
	mysql_query($sql);
	$id = mysql_insert_id();
}
catch (Exception $e)
{
	$data = array(
		'error' => mysql_error()
	);
	print json_encode($data);
	return;
}

if ($meet)
{
    try
    {
        $sql = "SELECT COUNT(*) as Total FROM WeatherModule where MAC = '$mac'";
        $result = mysql_query($sql);
        $moduleData = mysql_fetch_assoc($result);
        $moduleExists = $moduleData["Total"] == 1;
        mysql_free_result($result);

        if ($moduleExists)
        {
            $sql = "UPDATE WeatherModule SET IP = '$ip', ModuleName = '$modulename', ModuleID = $moduleid LastSeenDateTime = CURRENT_TIMESTAMP WHERE MAC = '$mac'";
            mysql_query($sql);
        }
        else
        {
            $sql = "INSERT INTO WeatherModule (ModuleID, ModuleName, IP, MAC) VALUES ($moduleid, '$modulename', '$ip', '$mac')";
            mysql_query($sql);
        }
    }
    catch (Exception $e)
    {
        $data = array(
            'error' => mysql_error()
        );
        print json_encode($data);
        return;
    }
}

date_default_timezone_set('Europe/Kaliningrad');
$data = array(
	'id' => $id,
	'moduleid' => $moduleid,
	'modulename' => $modulename,
	'temperature1' => $temperature1,
	'temperature2' => $temperature2,
	'temperature3' => $temperature3,
	'humidity1' => $humidity1,
	'humidity2' => $humidity2,
	'humidity3' => $humidity3,
	'pressure1' => $pressure1,
	'pressure2' => $pressure2,
	'pressure3' => $pressure3,
	'illumination' => $illumination,
	'year' => (int)date('Y'),
	'month' => (int)date('m'),
	'day' => (int)date('d'),
	'hour' => (int)date('H'),
	'minute' => (int)date('i'),
	'second' => (int)date('s')
);

print json_encode($data);

mysql_close($link);

?>