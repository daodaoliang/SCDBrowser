#include "VoltageLevelAnalysor.h"

CVoltageLevelAnalysor::CVoltageLevelAnalysor(void)
{

}

CVoltageLevelAnalysor::~CVoltageLevelAnalysor(void)
{

}

int CVoltageLevelAnalysor::analysisVoltageLevelByIED(const QString& IEDName, const QString& IEDDesc)
{
	if (IEDName.isEmpty())// || IEDName.size() < 6)
	{
		return VOLTAGELEVEL_OTHERS;
	}
	if (IEDDesc.contains("Ö÷±ä") || IEDName.at(1) == 'T')
	{
		return VOLTAGELEVEL_MAINTRANS;
	}
	else if (IEDDesc.contains("6kv", Qt::CaseInsensitive))
	{
		return VOLTAGELEVEL_6KV;
	} 
	else if(IEDDesc.contains("110kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("11") == 0)
	{
		return VOLTAGELEVEL_110KV;
	}
	else if(IEDDesc.contains("35kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("35") == 0)
	{
		return VOLTAGELEVEL_35KV;
	}
	else if(IEDDesc.contains("66kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("66") == 0)
	{
		return VOLTAGELEVEL_66KV;
	}
	else if(IEDDesc.contains("10kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("10") == 0)
	{
		return VOLTAGELEVEL_10KV;
	}
	else if(IEDDesc.contains("220kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("22") == 0)
	{
		return VOLTAGELEVEL_220KV;
	}
	else if(IEDDesc.contains("330kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("33") == 0)
	{
		return VOLTAGELEVEL_330KV;
	}
	else if(IEDDesc.contains("500kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("50") == 0)
	{
		return VOLTAGELEVEL_500KV;
	}
	else if(IEDDesc.contains("750kv", Qt::CaseInsensitive) || IEDName.mid(2,2).compare("75") == 0)
	{
		return VOLTAGELEVEL_750KV;
	}
	else if(IEDDesc.contains("1000kv", Qt::CaseInsensitive))
	{
		return VOLTAGELEVEL_1000KV;
	}
	else
	{
		return VOLTAGELEVEL_OTHERS;
	}
}
