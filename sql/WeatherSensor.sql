--
-- Table structure for table `WeatherSensor`
--

CREATE TABLE IF NOT EXISTS `WeatherSensor` (
  `ID` int(11) NOT NULL,
  `SensorName` varchar(50) NOT NULL,
  `ChartVisibility` bit(1) DEFAULT b'1',
  `TableVisibility` bit(1) DEFAULT b'1',
  `Description` varchar(50) DEFAULT NULL,
  `Units` varchar(50) DEFAULT NULL,
  `ChartTitle` varchar(50) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `WeatherSensor`
--

INSERT INTO `WeatherSensor` VALUES
(3, 'Temperature1', b'1', b'1', 'Температура 1', '°C', 'Температура (°C)'),
(4, 'Temperature2', b'1', b'1', 'Температура 2', '°C', 'Температура (°C)'),
(5, 'Temperature3', b'0', b'1', 'Температура 3', '°C', 'Температура (°C)'),
(6, 'Humidity1', b'1', b'1', 'Влажность 1', '%', 'Относительная влажность (%)'),
(7, 'Humidity2', b'0', b'1', 'Влажность 2', '%', 'Относительная влажность (%)'),
(8, 'Humidity3', b'0', b'1', 'Влажность 3', '%', 'Относительная влажность (%)'),
(9, 'Pressure1', b'0', b'1', 'Давление 1', 'mmHg', 'Атмосферное давление (mmHg)'),
(10, 'Pressure2', b'1', b'1', 'Давление 2', 'mmHg', 'Атмосферное давление (mmHg)'),
(11, 'Pressure3', b'0', b'1', 'Давление 3', 'mmHg', 'Атмосферное давление (mmHg)'),
(12, 'Illumination', b'0', b'1', 'Освещенность', 'lx', 'Освещенность (lx)'),
(13, 'CO2', b'0', b'1', 'Уровень CO2', 'ppm', 'Уровень CO2');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `WeatherSensor`
--
ALTER TABLE `WeatherSensor`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `WeatherSensor`
--
ALTER TABLE `WeatherSensor`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=15;