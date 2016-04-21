-- phpMyAdmin SQL Dump
-- version 4.3.12
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Apr 20, 2016 at 09:11 PM
-- Server version: 5.5.35-33.0-log
-- PHP Version: 5.4.39

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `host1402357`
--

-- --------------------------------------------------------

--
-- Table structure for table `WeatherSensor`
--

CREATE TABLE IF NOT EXISTS `WeatherSensor` (
  `ID` int(11) NOT NULL,
  `SensorName` varchar(50) NOT NULL,
  `ChartVisibility` int(11) DEFAULT '1'
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

--
-- Dumping data for table `WeatherSensor`
--

INSERT INTO `WeatherSensor` (`ID`, `SensorName`, `ChartVisibility`) VALUES
(3, 'Temperature1', 0),
(4, 'Temperature2', 0),
(5, 'Temperature3', 1),
(6, 'Humidity1', 1),
(7, 'Humidity2', 0),
(8, 'Humidity3', 1),
(9, 'Pressure1', 0),
(10, 'Pressure2', 0),
(11, 'Pressure3', 0),
(12, 'Illumination', 1);

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
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=13;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
