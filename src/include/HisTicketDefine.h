#ifndef HISTICKET_DEFINE_H
#define HISTICKET_DEFINE_H

#include <qstring>
#include <qicon>
#include "SecurityMeasuresTicket.h"

enum	TicketTypeEnum {
	type_build,
	type_print,
	type_typical,
	type_saved,
	type_erase,
	type_all
};

struct TicketTypeItem
{
	TicketTypeEnum     typeNo;
	QString            typeName;
	QString            stateName;
	QIcon              typeIcon;
};

//查询条件
class QueryCondition
{
public:
	TicketTypeEnum        GetTicketType(){ return ticketType;};
	QDateTime             GetStartTime(){ return startTime;};
	QDateTime             GetEndTime(){ return endTime;};
	QString               GetVoltLevel(){ return voltLevel;};
	QString               GetTransformStation(){ return transformStation;};
	QString               GetTicketID(){ return ticketID;};
	QString               GetExecutorName(){ return executorName;};
	QString               GetVertifName(){ return vertifName;};
	QVector<IEDInfo>      *GetMaintenanceIEDs(){ return &maintenanceIEDs;};
	QString               GetSCDVersion() { return SCDName;};

	void                  SetTicketType(TicketTypeEnum type){ ticketType = type;};
	void                  SetStartTime(QDateTime dateTime){ startTime = dateTime;};
	void                  SetEndTime(QDateTime dateTime){ endTime = dateTime;};
	void                  SetVoltLevel(QString volt){ voltLevel = volt;};
	void                  SetTransformStation(QString transSta){ transformStation = transSta;};
	void                  SetTicketID(QString id){ ticketID = id;};
	void                  SetExecutorName(QString executor){ executorName = executor;};
	void                  SetVertifName(QString verifer){ vertifName = verifer;};
	void                  SetMaintenanceIEDs(QVector<IEDInfo> *IEDs)
	                      {
		                      maintenanceIEDs.clear();
		                      for(int i=0;i<IEDs->size();i++)
							  {
								  maintenanceIEDs.push_back(IEDs->at(i));
							  }
	                      };
	void                  SetSCDVersion(QString version) { SCDName = version;};


public:
	QueryCondition()
	{
		ticketType  = type_all;
		startTime = QDateTime(QDateTime(QDate(QDateTime::currentDateTime().date().year(),1,1), QTime(9,0)));
		endTime   = QDateTime(QDateTime(QDateTime::currentDateTime()));;
		voltLevel = "全部";
		transformStation = "全部";
		ticketID           = "";
		executorName     = "";
		vertifName       = "";
		maintenanceIEDs.resize(0);
		SCDName          = "";
	};
public:
	TicketTypeEnum        ticketType;
	QDateTime             startTime;
	QDateTime             endTime;
	QString               voltLevel;
	QString               transformStation;
	QString               ticketID;
	QString               executorName;
	QString               vertifName;
    QVector<IEDInfo>      maintenanceIEDs;
	QString               SCDName;
};

class SecurityMeasuresTicketItem
{
public:
	SecurityMeasuresTicket  secuMeasTicket;
	int                     selectedFlag;
};

#endif