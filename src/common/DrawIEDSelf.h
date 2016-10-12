#pragma once

/*
���Ƶ���IED������ϸ����������Ӽ���������˿�
*/
#include <QGraphicsScene>
#include "hmigraphicsscene.h"
#include "hmidll_global.h"
#include "GlobalDefine.h"

class HMIDLL_EXPORT CDrawIEDSelf
{
public:
	CDrawIEDSelf(HmiGraphicsScene *scene,QMenu *menu,QString &name,const QString &iedKey);
	~CDrawIEDSelf(void);

	void		Draw();
protected:
	void		SortByString(bool bAscend);
private:
	HmiGraphicsScene		*mainScene;
	QMenu					*itemMenu;

	int						m_nSpace;	//��������Ӽ��
	int						m_nLength;	//ÿ��������߳���
	VTList					m_vtList;	//������б�

	QString					m_strIED_Name;
	QString					IEDKey;
};
