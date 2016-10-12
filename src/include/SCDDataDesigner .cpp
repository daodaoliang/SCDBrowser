#include "SCDDataDesigner.h"

SCDDataDesigner::SCDDataDesigner()
{
	  ;
}

SCDDataDesigner::~SCDDataDesigner()
{
	  ;
}

bool SCDDataDesigner::Load(string filePath)
{
	  return SAXSCLImportW(sPath.c_str(), &pSCD);
}

string SCDDataDesigner::GetVoltageLevelName()
{
	  return "";
}

string SCDDataDesigner::GetSubstationName()
{
	  return "";
}

void SCDDataDesigner::GetIEDs(vector<> &vtIEDs)
{
	  return ;
}

void SCDDataDesigner::GetInputSignals(string IEDKey,vector<> &vtSignals)
{
	  return ;
}

void SCDDataDesigner::GetOutputSignals(string IEDKey)
{
	  return ;
}

void SCDDataDesigner::GetLinks(string IEDKey,vector<> &vtLinks)
{
	  return ;
}

void SCDDataDesigner::GetTrips(string IEDKey,vector<> &vtTrips)
{
	  return ;
}
	