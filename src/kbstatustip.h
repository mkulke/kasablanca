//
// C++ Interface: kbstatustip
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KBSTATUSTIP_H
#define KBSTATUSTIP_H

#include <qtooltip.h>

/**
@author Magnus Kulke
*/
class KbStatusTip : public QToolTip
{
public:
	KbStatusTip(QWidget * widget);
	void maybeTip(const QPoint &pos);
	void ShowStatus(QString msg);
};

#endif
