//
// C++ Interface: kbtaskview
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KBTASKVIEW_H
#define KBTASKVIEW_H

#include <klistview.h>

/**
@author Magnus Kulke
*/
class KbTaskView : public KListView
{
Q_OBJECT
public:
    KbTaskView(QWidget *parent = 0, const char *name = 0);
    ~KbTaskView();
	 QListViewItem* LastChild();
public slots:
	void SLOT_DoubleClicked(QListViewItem* item, const QPoint& p, int c);
	//virtual bool acceptDrag(QDropEvent* event) const;
	//virtual QDragObject* dragObject();
};

#endif
