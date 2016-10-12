
SET NAMES utf8;

CREATE DATABASE DBSSMS CHARACTER SET utf8;;

USE DBSSMS;



CREATE TABLE t_Role
(
 rolename     VARCHAR(20) PRIMARY KEY NOT NULL,
 privileges0  INT UNSIGNED,
 privileges1  INT UNSIGNED,
 privileges2  INT UNSIGNED,
 privileges3  INT UNSIGNED,
 privileges4  INT UNSIGNED,
 privileges5  INT UNSIGNED, 
 privileges6  INT UNSIGNED,
 privileges7  INT UNSIGNED,
 privileges8  INT UNSIGNED,
 privileges9  INT UNSIGNED,
 privileges10  INT UNSIGNED,
 privileges11  INT UNSIGNED,
 privileges12  INT UNSIGNED, 
 privileges13  INT UNSIGNED, 
 privileges14  INT UNSIGNED, 
 privileges15  INT UNSIGNED,
 privileges16  INT UNSIGNED,
 privileges17  INT UNSIGNED, 
 privileges18  INT UNSIGNED, 
 privileges19  INT UNSIGNED     
 );
 
 
 CREATE TABLE t_User
(
 userName     VARCHAR(20) PRIMARY KEY NOT NULL,
 pass         VARCHAR(20) NOT NULL,
 roleName     VARCHAR(20) NOT NULL
);

CREATE TABLE t_TicketType
(
 typeNo    INT UNSIGNED PRIMARY KEY,
 typeName  VARCHAR(32) NOT NULL,
 stateName VARCHAR(32) NOT NULL,
 iconName  VARCHAR(48) NOT NULL,
 transValidFlagNew      INT UNSIGNED, 
 transValidFlagPrint    INT UNSIGNED,
 transValidFlagSave     INT UNSIGNED,  
 transValidFlagTypical  INT UNSIGNED, 
 transValidFlagDelete   INT UNSIGNED
);

CREATE TABLE t_TicketIDStrategy
(
 curYear     VARCHAR(8),
 curYearNum  INT UNSIGNED NOT NULL ,
 curMonth    VARCHAR(8),
 curMonthNum INT UNSIGNED NOT NULL ,
 totalNum    INT UNSIGNED NOT NULL ,
 IDModelString VARCHAR(32) NOT NULL
);

CREATE TABLE t_SecMeasStrategy
(
 strategyName              VARCHAR(20) PRIMARY KEY NOT NULL,
 thisIEDFlag               INT UNSIGNED NOT NULL ,  
 thisIEDPortFlag           INT UNSIGNED NOT NULL ,   
 thisIEDFuncFlag           INT UNSIGNED NOT NULL ,
 thisIEDGooseOutSignalFlag INT  UNSIGNED NOT NULL ,
 thisIEDSVOutSignalFlag    INT  UNSIGNED NOT NULL , 
 thisIEDGooseInSignalFlag  INT  UNSIGNED NOT NULL ,
 thisIEDSVInSignalFlag     INT  UNSIGNED NOT NULL ,  
 relateIEDFlag               INT UNSIGNED NOT NULL ,  
 relateIEDPortFlag           INT UNSIGNED NOT NULL ,   
 relateIEDFuncFlag           INT UNSIGNED NOT NULL ,
 relateIEDGooseOutSignalFlag INT  UNSIGNED NOT NULL ,
 relateIEDSVOutSignalFlag    INT  UNSIGNED NOT NULL , 
 relateIEDGooseInSignalFlag  INT  UNSIGNED NOT NULL ,
 relateIEDSVInSignalFlag     INT  UNSIGNED NOT NULL
);

CREATE TABLE t_Ticket
(
 ticketId           VARCHAR(32) PRIMARY KEY NOT NULL,
 voltLevelName      VARCHAR(32) NOT NULL,
 subStationName     VARCHAR(32) NOT NULL,
 taskName           VARCHAR(256),
 taskOverview       VARCHAR(1024),
 startTaskDateTime  VARCHAR(20),
 endTaskDateTime    VARCHAR(20),
 executorName       VARCHAR(20),
 verifierName       VARCHAR(20),
 verSCD             VARCHAR(20),
 IEDNames           VARCHAR(512),
 tickeType          INT UNSIGNED NOT NULL ,
 reportData         BLOB,
 reportTemplateName VARCHAR(32),
 graphicNames       VARCHAR(1024)
);
 
CREATE TABLE t_ReportTemplate
(
 reportTemplatelId   INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
 reportTemplateName VARCHAR(20) NOT NULL,
 reportData         BLOB
);

CREATE TABLE t_SecurityMeasure
(
 ticketId           VARCHAR(32) PRIMARY KEY NOT NULL,
 panelName          VARCHAR(32) NOT NULL,
 IEDName            VARCHAR(32) NOT NULL,
 tripName           VARCHAR(256),
 state              INT UNSIGNED NOT NULL
);

CREATE TABLE t_Graphic
(
 graphicName        VARCHAR(64) PRIMARY KEY NOT NULL, 
 SCDVersion         VARCHAR(32),
 graphicType        VARCHAR(32) NOT NULL,
 graphicData        BLOB
);


ALTER TABLE `t_Role` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_User` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_TicketType` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_TicketIDStrategy` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_SecMeasStrategy` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_Ticket` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_ReportTemplate` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
ALTER TABLE `t_ReportTemplate` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'root' WITH GRANT OPTION;
FLUSH   PRIVILEGES;

INSERT INTO t_Role (rolename,privileges0,privileges1,privileges2,privileges3,privileges4,privileges5,privileges6,privileges7) VALUE('系统管理员',1,1,1,1,1,1,1,1);

INSERT INTO t_User (userName,pass,roleName) VALUE('陈鑫','1','系统管理员');


INSERT INTO t_TicketType (typeNo,typeName,stateName,iconName,transValidFlagNew,transValidFlagPrint,transValidFlagSave,transValidFlagTypical,transValidFlagDelete) VALUE(0,'新建票','已新建','images/FileNew.png',0,1,0,0,1);
INSERT INTO t_TicketType (typeNo,typeName,stateName,iconName,transValidFlagNew,transValidFlagPrint,transValidFlagSave,transValidFlagTypical,transValidFlagDelete) VALUE(1,'打印票','已打印','images/PrintAreaMenu.png',0,0,1,1,1);
INSERT INTO t_TicketType (typeNo,typeName,stateName,iconName,transValidFlagNew,transValidFlagPrint,transValidFlagSave,transValidFlagTypical,transValidFlagDelete) VALUE(2,'存档票','已存档','images/PrintColumns.png',0,0,0,1,1);
INSERT INTO t_TicketType (typeNo,typeName,stateName,iconName,transValidFlagNew,transValidFlagPrint,transValidFlagSave,transValidFlagTypical,transValidFlagDelete) VALUE(3,'典型票','已存为典型','images/ConvertTableToText.png',0,0,1,0,1);
INSERT INTO t_TicketType (typeNo,typeName,stateName,iconName,transValidFlagNew,transValidFlagPrint,transValidFlagSave,transValidFlagTypical,transValidFlagDelete) VALUE(4,'作废票','已作废','images/PrintPreviewClose.png',0,0,1,0,0);

INSERT INTO t_TicketIDStrategy (curYear,curYearNum,curMonth,curMonthNum,totalNum,IDModelString) VALUE('2013',100,'1',10,1000,"YYYY-MM_NNNN");


INSERT INTO t_SecMeasStrategy (strategyName,thisIEDFlag,thisIEDPortFlag,thisIEDFuncFlag,thisIEDGooseOutSignalFlag,thisIEDSVOutSignalFlag,thisIEDGooseInSignalFlag,thisIEDSVInSignalFlag,relateIEDFlag,relateIEDPortFlag,relateIEDFuncFlag,relateIEDGooseOutSignalFlag,relateIEDSVOutSignalFlag,relateIEDGooseInSignalFlag,relateIEDSVInSignalFlag) VALUE('全部隔离',1,1,1,1,1,1,1,1,1,1,1,1,1,1);
INSERT INTO t_SecMeasStrategy (strategyName,thisIEDFlag,thisIEDPortFlag,thisIEDFuncFlag,thisIEDGooseOutSignalFlag,thisIEDSVOutSignalFlag,thisIEDGooseInSignalFlag,thisIEDSVInSignalFlag,relateIEDFlag,relateIEDPortFlag,relateIEDFuncFlag,relateIEDGooseOutSignalFlag,relateIEDSVOutSignalFlag,relateIEDGooseInSignalFlag,relateIEDSVInSignalFlag) VALUE('远端隔离',0,0,0,0,0,0,0,1,1,1,1,1,1,1);
INSERT INTO t_SecMeasStrategy (strategyName,thisIEDFlag,thisIEDPortFlag,thisIEDFuncFlag,thisIEDGooseOutSignalFlag,thisIEDSVOutSignalFlag,thisIEDGooseInSignalFlag,thisIEDSVInSignalFlag,relateIEDFlag,relateIEDPortFlag,relateIEDFuncFlag,relateIEDGooseOutSignalFlag,relateIEDSVOutSignalFlag,relateIEDGooseInSignalFlag,relateIEDSVInSignalFlag) VALUE('本端隔离',1,1,1,1,1,1,1,0,0,0,0,0,0,0);

