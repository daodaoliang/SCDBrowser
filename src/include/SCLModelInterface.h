#ifndef SCLModelInterface_H
#define SCLModelInterface_H

#include <QString>
#include <QVector>

class SCLModelInterface
{
public:
	virtual int   AddVoltageLevel(QString voltageLevelName) = 0;
	virtual int   DeleteVoltageLevel(QString voltageLevelName) = 0;
	virtual int   AddTransformerSubstation(QString voltageLevelName,QString transSubName,QString SCDFilePath) = 0;
	virtual int   DeleteTransformerSubstation(QString voltageLevelName,QString transSubName) = 0;
	virtual int   UpdateTransformerSubstationModel(QString voltageLevelName,QString transSubName,QString SCDFilePath) = 0;
	virtual int   UpdateSingalTripStates(QString voltageLevelName,QString transSubName,QString stateFilePath) = 0;
	virtual int   IsVoltageLevelExist(QString voltageLevelName) = 0;
	virtual int   IsTransformerSubstationExist(QString voltageLevelName,QString transSubName) = 0;

	virtual void GetVoltageLevelList(QVector<QString> *list) = 0;
	virtual void GetTransformerSubstationList(QString voltageLevelName,QVector<QString> *list) = 0;
	virtual void GetIEDCabinetList(QString voltageLevelName,QString transSubName,QVector<QString> *list) = 0;
	virtual void GetIEDList(QString voltageLevelName,QString transSubName,QString IEDCabinetName,QVector<QString> *list) = 0;
	virtual QString GetSCDVersion() = 0;
};

#endif