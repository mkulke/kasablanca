//
// C++ Implementation: kbstatustip
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qcursor.h>

#include "kbstatustip.h"

KbStatusTip::KbStatusTip(QWidget * widget) : QToolTip(widget)
{
}

void KbStatusTip::maybeTip(const QPoint &)
{
	tip(parentWidget()->rect(), "kasablanca");
}

void KbStatusTip::ShowStatus(QString msg)
{
	if ((QCursor::pos().x() > parentWidget()->mapToGlobal(parentWidget()->pos()).x())
		&& (QCursor::pos().y() > parentWidget()->mapToGlobal(parentWidget()->pos()).y())
		&& (QCursor::pos().x() < (parentWidget()->mapToGlobal(parentWidget()->pos()).x() + parentWidget()->width()))
		&& (QCursor::pos().y() < (parentWidget()->mapToGlobal(parentWidget()->pos()).y() + parentWidget()->height())))
	{
		tip(parentWidget()->rect(), msg);
	}
}
