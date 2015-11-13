/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : ogengine

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2012-11-29 23:57:27
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `account`
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `AccountUID` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `Login` varchar(32) DEFAULT NULL,
  `Password` varchar(32) DEFAULT NULL,
  `Email` varchar(127) DEFAULT NULL,
  `Banned` smallint(1) DEFAULT NULL,
  `Online` smallint(1) DEFAULT NULL,
  `GuildUID` int(16) DEFAULT NULL,
  `PartyUID` int(16) DEFAULT NULL,
  `Level` int(16) DEFAULT NULL,
  `MaxHp` int(16) DEFAULT NULL,
  `Hp` int(16) DEFAULT NULL,
  `MaxMp` int(16) DEFAULT NULL,
  `Mp` int(16) DEFAULT NULL,
  `S_str` int(16) DEFAULT NULL,
  `S_def` int(16) DEFAULT NULL,
  `S_int` int(16) DEFAULT NULL,
  `S_agi` int(16) DEFAULT NULL,
  `Points` int(16) DEFAULT NULL,
  `Money` int(16) DEFAULT NULL,
  `Vip` smallint(2) DEFAULT NULL,
  `Class` smallint(2) DEFAULT NULL,
  `Exp` int(16) DEFAULT NULL,
  `MaxExp` int(16) DEFAULT NULL,
  `MapUID` int(16) DEFAULT NULL,
  `CoordX` int(16) DEFAULT NULL,
  `CoordY` int(16) DEFAULT NULL,
  PRIMARY KEY (`AccountUID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of account
-- ----------------------------
INSERT INTO `account` VALUES ('1', 'Firefox', '123456', 'firefoxlucas@hotmail.com.br', '0', '0', '1', '1', '1', '100', '100', '100', '100', '30', '30', '30', '30', '1000', '1000', '0', '1', '20', '250', '1', '17', '6');
INSERT INTO `account` VALUES ('2', 'Teste', '123456', 'firefoxlucas@hotmail.com.br', '0', '0', '0', '0', '20', '100', '100', '100', '100', '30', '30', '30', '30', '1000', '10000', '1', '1', '200', '250', '1', '12', '9');

-- ----------------------------
-- Table structure for `guild`
-- ----------------------------
DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild` (
  `GuildUID` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `GuildMasterAccountUID` int(12) unsigned DEFAULT NULL,
  `GuildName` varchar(16) DEFAULT NULL,
  `UIDAccount01` int(12) DEFAULT NULL,
  `UIDAccount02` int(12) DEFAULT NULL,
  `UIDAccount03` int(12) DEFAULT NULL,
  `UIDAccount04` int(12) DEFAULT NULL,
  `UIDAccount05` int(12) DEFAULT NULL,
  `UIDAccount06` int(12) DEFAULT NULL,
  `UIDAccount07` int(12) DEFAULT NULL,
  `UIDAccount08` int(12) DEFAULT NULL,
  `UIDAccount09` int(12) DEFAULT NULL,
  `UIDAccount10` int(12) DEFAULT NULL,
  `UIDAccount11` int(12) DEFAULT NULL,
  `UIDAccount12` int(12) DEFAULT NULL,
  `UIDAccount13` int(12) DEFAULT NULL,
  `UIDAccount14` int(12) DEFAULT NULL,
  `UIDAccount15` int(12) DEFAULT NULL,
  PRIMARY KEY (`GuildUID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of guild
-- ----------------------------
INSERT INTO `guild` VALUES ('1', '1', 'GameGuild', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for `map`
-- ----------------------------
DROP TABLE IF EXISTS `map`;
CREATE TABLE `map` (
  `MapUID` varchar(16) DEFAULT NULL,
  `FileMap` varchar(16) DEFAULT NULL,
  `FileMapTiles` varchar(16) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of map
-- ----------------------------
INSERT INTO `map` VALUES ('1', 'maptest.map', 'tiles.bmp');

-- ----------------------------
-- Table structure for `party`
-- ----------------------------
DROP TABLE IF EXISTS `party`;
CREATE TABLE `party` (
  `PartyUID` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `PartyMasterAccountUID` int(12) DEFAULT NULL,
  `PartyName` varchar(16) DEFAULT NULL,
  `UIDAccount01` int(12) DEFAULT NULL,
  `UIDAccount02` int(12) DEFAULT NULL,
  `UIDAccount03` int(12) DEFAULT NULL,
  `UIDAccount04` int(12) DEFAULT NULL,
  PRIMARY KEY (`PartyUID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of party
-- ----------------------------
INSERT INTO `party` VALUES ('1', '1', 'GameParty', '0', '0', '0', '0');
