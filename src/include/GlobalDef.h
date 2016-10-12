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
	P = 0,			//����;
	C,				//���;
	ST,				//����;
	MU,				//�ϲ�;
	MS,				//�Ǻ�;
	PC,				//����;
	NO				//������;
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

	VTermType			Termtype;//��ʾ���뻹����������;
}struVT;

typedef QList<VirtualTerminal>		VTList;

const qreal Pi = 3.1415926;

#define			IED_WIDTH		400

#endif