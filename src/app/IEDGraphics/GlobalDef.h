#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "SCLModelSvrAdapterInterface.h"

enum VTermType {
	term_null = 0,
	term_input,
	term_output,
	term_all
};

enum IEDType {
	P = 0,			//保护;
	C,				//测控;
	ST,				//智能;
	MU,				//合并;
	MS,				//智合;
	PC,				//保测;
	NO				//无类型;
};

typedef struct VirtualTerminal
{
	int			iOrd;
	QString		strUri;
	QString		strName;
	QString		strDesc;
	QString		strPath;
	QString		strKeyValue;

	QVirtualLink	belongVLink;
	QPhysLink		belongPLink;

	VTermType		Termtype;//表示输入还是输出虚端子;
}struVT;

typedef QList<VirtualTerminal>		VTList;

const qreal Pi = 3.1415926;

#define			IED_WIDTH		400

#endif