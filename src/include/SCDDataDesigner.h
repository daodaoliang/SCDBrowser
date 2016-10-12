#ifndef SCDDataDesigner_H
#define SCDDataDesigner_H

#include "stuct.h"

use namespace std

class SCDDataDesigner : public SCDDataInterface
{
public:
	virtual bool   Load(string filePath);
	virtual string GetVoltageLevelName();
	virtual string GetSubstationName();
	virtual void   GetIEDs(vector<> &vtIEDs);
	virtual void   GetInputSignals(string IEDKey,vector<> &vtSignals);
	virtual void   GetOutputSignals(string IEDKey);
	virtual void   GetLinks(string IEDKey,vector<> &vtLinks);
	virtual void   GetTrips(string IEDKey,vector<> &vtTrips);	
		
  SCDDataDesigner();
	~SCDDataDesigner();	
private:
	PSCDSTRU m_pSCD;
};
