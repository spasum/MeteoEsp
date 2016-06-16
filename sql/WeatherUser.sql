--
-- Table structure for table `WeatherUser`
--

CREATE TABLE IF NOT EXISTS `WeatherUser` (
  `ID` int(11) NOT NULL,
  `UserName` varchar(200) NOT NULL,
  `Email` varchar(200) NOT NULL,
  `Password` varchar(200) NOT NULL,
  `LastLoginDateTime` timestamp NULL DEFAULT NULL,
  `CreatedDateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `VerifiedDateTime` timestamp NULL DEFAULT NULL,
  `IsActive` bit(1) DEFAULT b'0',
  `VerificationCode` varchar(200) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `WeatherUser`
--
ALTER TABLE `WeatherUser`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `WeatherUser`
--
ALTER TABLE `WeatherUser`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;