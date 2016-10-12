#include "SecurityMeasuresEditor.h"

SecurityMeasuresEditor::SecurityMeasuresEditor(QVector<SecurityMeasures> *smList,QString volName,QString subName,QVector<IEDInfo> *IEDInfos)
{
	voltageLevelName     = volName;
	substationName       = subName;
    securityMeasuresList = smList;
	maintenanceIEDs      = IEDInfos;
	smManger = new SecurityMeasuresManger(securityMeasuresList);
}

SecurityMeasuresEditor::~SecurityMeasuresEditor()
{

}

QVector<SecurityMeasures> *SecurityMeasuresEditor::GetSecurityMeasuresList()
{
	return securityMeasuresList;
}

QVector<IEDInfo> *SecurityMeasuresEditor::GetMaintenanceIEDs()   
{

	return maintenanceIEDs;
}

SecurityMeasuresManger *SecurityMeasuresEditor::GetSecurityMeasuresManger()
{
	return smManger;
}

QString  SecurityMeasuresEditor::GetVolatageLevelName()
{
	return voltageLevelName;
}

QString  SecurityMeasuresEditor::GetSubstationName()
{
	return substationName;
}

QString  SecurityMeasuresEditor::FromIEDInfoToIEDKey(IEDInfo info)
{
	QString IEDKey =  voltageLevelName + ".";
	IEDKey += substationName;
	IEDKey += ".";
	IEDKey += info.panel;
	IEDKey += ".";
	IEDKey += info.IED;

	return IEDKey;
}



