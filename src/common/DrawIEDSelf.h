#pragma once

/*
绘制单个IED，最详细，包括虚端子及光纤物理端口
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

	int						m_nSpace;	//两条虚端子间距
	int						m_nLength;	//每条虚端子线长度
	VTList					m_vtList;	//虚端子列表

	QString					m_strIED_Name;
	QString					IEDKey;
};
