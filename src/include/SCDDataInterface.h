#ifndef SCDDataInterface_H
#define SCDDataInterface_H

class SCDDataInterface
{
public:
	virtual bool   Load(string filePath) = 0;
	virtual string GetVoltageLevelName() = 0;
	virtual string GetSubstationName() = 0;
	virtual void   GetIEDs(vector<> &vtIEDs) = 0;
	virtual void   GetInputSignals(string IEDName,vector<> &vtSignals) = 0;
	virtual void   GetOutputSignals(string IEDName) = 0;
	virtual void   GetVirtualLinks(string IEDName,vector<> &vtLinks) = 0;
	virtual void   GetTrips(string IEDName,vector<> &vtTrips) = 0;
};

#endif