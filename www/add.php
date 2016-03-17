<?php
	$id = 0;
	$sensorid = 0;
	$modulename = "";
	$temperature1 = 0;
	$temperature2 = 0;
	$humidity = 0;
	$pressure = 0;

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

	if (isset($object['sensorid']))
	{
		if(!empty($object['sensorid']))
		{
			$sensorid = (int)$object["sensorid"];
		}
	}

	if (isset($object['modulename']))
	{
		if(!empty($object['modulename']))
		{
			$modulename = $object["modulename"];
		}
	}

	if (isset($object['temperature1']))
	{
		if(!empty($object['temperature1']))
		{
			$temperature1 = (float)$object["temperature1"];
		}
	}

	if (isset($object['temperature2']))
	{
		if(!empty($object['temperature2']))
		{
			$temperature2 = (float)$object["temperature2"];
		}
	}
	if (isset($object['humidity']))
	{
		if(!empty($object['humidity']))
		{
			$humidity = (float)$object["humidity"];
		}
	}
	if (isset($object['pressure']))
	{
		if(!empty($object['pressure']))
		{
			$pressure = (float)$object["pressure"];
		}
	}

	if ($sensorid == 0) {
		$data = array(
			'error' => 'Wrong sensor ID.'
		);
		print json_encode($data);
		return;
	}

	try
	{
		$link = mysql_connect("localhost", "phpmyadmin", "raspberry");
	}
	catch (Exception $e)
	{
		$data = array(
			'error' => mysql_error()
		);
		print json_encode($data);
		return;
	}

	$sql = "INSERT INTO climate (SensorID, Temperature1, Temperature2, Humidity, Pressure, ModuleName) VALUES ($sensorid, $temperature1, $temperature2, $humidity, $pressure, '$modulename')";
	try
	{
		mysql_select_db("phpmyadmin");
		$result = mysql_query($sql);
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

	date_default_timezone_set('Europe/Kaliningrad');
	$data = array(
		'id' => $id,
		'sensorid' => $sensorid,
		'modulename' => $modulename,
		'temperature1' => $temperature1,
		'temperature2' => $temperature2,
		'humidity' => $humidity,
		'pressure' => $pressure,
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