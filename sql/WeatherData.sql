--
-- Table structure for table `WeatherData`
--

CREATE TABLE IF NOT EXISTS `WeatherData` (
  `ID` int(11) NOT NULL,
  `ModuleID` int(11) NOT NULL,
  `ModuleName` varchar(50) NOT NULL,
  `Temperature1` float DEFAULT NULL,
  `Temperature2` float DEFAULT NULL,
  `Temperature3` float DEFAULT NULL,
  `Humidity1` float DEFAULT NULL,
  `Humidity2` float DEFAULT NULL,
  `Humidity3` float DEFAULT NULL,
  `Pressure1` float DEFAULT NULL,
  `Pressure2` float DEFAULT NULL,
  `Pressure3` float DEFAULT NULL,
  `Illumination` float DEFAULT NULL,
  `CO2` float DEFAULT NULL,
  `MeasuredDateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `WeatherData`
--
ALTER TABLE `WeatherData`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `WeatherData`
--
ALTER TABLE `WeatherData`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
