-- phpMyAdmin SQL Dump
-- version 4.3.12
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Mar 24, 2016 at 03:08 PM
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
-- Table structure for table `WeatherData`
--

CREATE TABLE IF NOT EXISTS `WeatherData` (
  `ID` int(11) NOT NULL,
  `SensorID` int(11) NOT NULL,
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
  `MeasuredDateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB AUTO_INCREMENT=879 DEFAULT CHARSET=utf8;

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
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=879;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
