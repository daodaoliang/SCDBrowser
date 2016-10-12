#ifndef TicketTranslator_H
#define TicketTranslator_H


#include <string>
#include <vector>
#include "TicketSvrInterface.h"
#include "SecurityMeasuresTicket.h"
#include "HisTicketDefine.h"

using namespace std;

class TicketTranslator
{
public:
	SliceSecurityMeasuresTicket FromTicketToSliceTicket(SecurityMeasuresTicket ticket)
	{
		SliceSecurityMeasuresTicket sliTicket;
		sliTicket.ticketID      = ticket.GetTicketID().toLocal8Bit().constData();
		sliTicket.voltLevelName = ticket.GetVoltLevelName().toLocal8Bit().constData();
		sliTicket.transSubsName = ticket.GetTransSubsName().toLocal8Bit().constData();
		sliTicket.taskName = ticket.GetTaskName().toLocal8Bit().constData();
		sliTicket.taskOverview = ticket.GetTaskOverview().toLocal8Bit().constData();
		sliTicket.startTaskDateTime = ticket.GetStartTaskDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData();
		//sliTicket.startTaskDateTime = ticket.GetStartTaskDateTime().toString("yyyy-MM-dd hh:mm").toLocal8Bit().constData();
		sliTicket.endTaskDateTime = ticket.GetEndTaskDateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().constData();
		//sliTicket.endTaskDateTime = ticket.GetEndTaskDateTime().toString("yyyy-MM-dd hh:mm").toLocal8Bit().constData();
		sliTicket.executorName = ticket.GetExecutorName().toLocal8Bit().constData();
		sliTicket.verifierName = ticket.GetVerifierName().toLocal8Bit().constData();
		sliTicket.supervisorName = ticket.GetSupervisorName().toLocal8Bit().constData();
		sliTicket.state = (TicketTypeEnum)ticket.GetTicketState();
		sliTicket.verSCD = ticket.GetSCDVersion().toLocal8Bit().constData();
		sliTicket.ticketTemplateName = ticket.GetTicketTemplateName().toLocal8Bit().constData();
	
		int i = 0;
		for(i=0;i<ticket.GetSecurityMeasuresList()->size();i++)
		{
			SliceSecurityMeasures tempSecMeas;
			tempSecMeas.IED = ticket.GetSecurityMeasuresList()->at(i).IED.toLocal8Bit().constData();
			tempSecMeas.IEDCabinet = ticket.GetSecurityMeasuresList()->at(i).panel.toLocal8Bit().constData();
			tempSecMeas.state = ticket.GetSecurityMeasuresList()->at(i).state;
			tempSecMeas.trip = ticket.GetSecurityMeasuresList()->at(i).trip.toLocal8Bit().constData();
			sliTicket.securityMeasuresList.push_back(tempSecMeas);
		}
	
		QVector<IEDInfo> *IEDInfos = ticket.GetMaintenanceIEDs();
	
		for(i=0;i<IEDInfos->size();i++)
		{
			SliceIEDInfo tempIEDInfo;
			tempIEDInfo.IEDName = IEDInfos->at(i).IED.toLocal8Bit().constData();
			tempIEDInfo.IEDCabinetName = IEDInfos->at(i).panel.toLocal8Bit().constData();
			sliTicket.maintenanceIEDs.push_back(tempIEDInfo);
		}
	
		for(i=0;i<ticket.GetIEDGraphicName()->size();i++)
		{
			sliTicket.IEDGraphicNames.push_back(ticket.GetIEDGraphicName()->at(i).toLocal8Bit().constData());
		}
	
		for(i=0;i<ticket.GetReportData()->size();i++)
		{
			sliTicket.reportData.push_back(ticket.GetReportData()->at(i));
		}
		return sliTicket;
	};
	
	bool FromSliceTicketToTicket(SliceSecurityMeasuresTicket fromTicket,SecurityMeasuresTicket *toTicket)
	{
		toTicket->SetVoltLevelName(fromTicket.voltLevelName.c_str());
		toTicket->SetTransSubsName(fromTicket.transSubsName.c_str());
		toTicket->SetTicketID(fromTicket.ticketID.c_str());
		toTicket->SetTaskName(fromTicket.taskName.c_str());
		toTicket->SetTaskOverview(fromTicket.taskOverview.c_str());
		toTicket->SetExecutorName(fromTicket.executorName.c_str());
		toTicket->SetVerifierName(fromTicket.verifierName.c_str());
		toTicket->SetSupervisorName(fromTicket.supervisorName.c_str());
		toTicket->SetSCDVersion(fromTicket.verSCD.c_str());
		toTicket->SetTicketTemplateName(fromTicket.ticketTemplateName.c_str());
		toTicket->SetTicketState((TicketTypeEnum)fromTicket.state);
		toTicket->SetStartTaskDateTime(QDateTime::fromString(fromTicket.startTaskDateTime.c_str(), "yyyy-MM-dd hh:mm:ss"));
		toTicket->SetEndTaskDateTime(QDateTime::fromString(fromTicket.endTaskDateTime.c_str(), "yyyy-MM-dd hh:mm:ss"));

		for(int j=0;j<fromTicket.maintenanceIEDs.size();j++)
		{
			IEDInfo tempIEDInfo;
			tempIEDInfo.IED = fromTicket.maintenanceIEDs.at(j).IEDName.c_str();
			tempIEDInfo.panel = fromTicket.maintenanceIEDs.at(j).IEDCabinetName.c_str();
			toTicket->GetMaintenanceIEDs()->push_back(tempIEDInfo);
		}

		for(int j=0;j<fromTicket.IEDGraphicNames.size();j++)
		{
			toTicket->GetIEDGraphicName()->push_back(fromTicket.IEDGraphicNames.at(j).c_str());
		}

		for(int j=0;j<fromTicket.securityMeasuresList.size();j++)
		{
			SecurityMeasures tempSecurityMeasures;
            tempSecurityMeasures.panel = fromTicket.securityMeasuresList.at(j).IEDCabinet.c_str();
			tempSecurityMeasures.IED = fromTicket.securityMeasuresList.at(j).IED.c_str();
			tempSecurityMeasures.trip = fromTicket.securityMeasuresList.at(j).trip.c_str();
			tempSecurityMeasures.state = fromTicket.securityMeasuresList.at(j).state;
			toTicket->GetSecurityMeasuresList()->push_back(tempSecurityMeasures);
		}

		for(int j=0;j<fromTicket.reportData.size();j++)
		{
			toTicket->GetReportData()->push_back(fromTicket.reportData.at(j));
		}  
		return true;
	};

	/*SecurityMeasuresTicket FromSliceTicketToTicket(SliceSecurityMeasuresTicket ticket)
	{
		SecurityMeasuresTicket rtnTicket;
		rtnTicket.SetVoltLevelName(ticket.voltLevelName.c_str());
		rtnTicket.SetTransSubsName(ticket.transSubsName.c_str());
		rtnTicket.SetTicketID(ticket.ticketID.c_str());
		rtnTicket.SetTaskName(ticket.taskName.c_str());
		rtnTicket.SetTaskOverview(ticket.taskOverview.c_str());
		rtnTicket.SetExecutorName(ticket.executorName.c_str());
		rtnTicket.SetVerifierName(ticket.verifierName.c_str());
		rtnTicket.SetSupervisorName(ticket.supervisorName.c_str());
		rtnTicket.SetSCDVersion(ticket.verSCD.c_str());
		rtnTicket.SetTicketTemplateName(ticket.ticketTemplateName.c_str());
		rtnTicket.SetTicketState((TicketTypeEnum)ticket.state);
		rtnTicket.SetStartTaskDateTime(QDateTime::fromString(ticket.startTaskDateTime.c_str(), "yyyy-MM-dd hh:mm:ss"));
		rtnTicket.SetEndTaskDateTime(QDateTime::fromString(ticket.endTaskDateTime.c_str(), "yyyy-MM-dd hh:mm:ss"));

		for(int j=0;j<ticket.maintenanceIEDs.size();j++)
		{
			IEDInfo tempIEDInfo;
			tempIEDInfo.IED = ticket.maintenanceIEDs.at(j).IEDName.c_str();
			tempIEDInfo.panel = ticket.maintenanceIEDs.at(j).IEDCabinetName.c_str();
			rtnTicket.GetMaintenanceIEDs()->push_back(tempIEDInfo);
		}

		for(int j=0;j<ticket.IEDGraphicNames.size();j++)
		{
			rtnTicket.GetIEDGraphicName()->push_back(ticket.IEDGraphicNames.at(j).c_str());
		}

		for(int j=0;j<ticket.reportData.size();j++)
		{
			rtnTicket.GetReportData()->push_back(ticket.reportData.at(j));
		}  
		return rtnTicket;
	};*/
};

#endif