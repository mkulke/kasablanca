//
// C++ Implementation: kbtaskview
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <klocale.h>

#include <qptrlist.h>
#include <qevent.h>

#include "kbtaskview.h"
#include "kbfileinfo.h"
#include "kbtransferitem.h"

KbTaskView::KbTaskView(QWidget *parent, const char *name)
 : KListView(parent, name)
{
	addColumn(i18n("Task"));
	addColumn(i18n("Progress"));
	addColumn(i18n("Speed"));
	addColumn(i18n("Percentage"));
	addColumn(i18n("Remaining"));
	
	setRootIsDecorated(true);
	setSelectionMode(QListView::Extended);
	setDragEnabled(true);
	setAcceptDrops(true);
		
	//connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
	//	this, SLOT(SLOT_DoubleClicked(QListViewItem*, const QPoint&, int)));
}

KbTaskView::~KbTaskView()
{
}

QListViewItem* KbTaskView::LastChild()
{
	QListViewItem* last = NULL;
	QListViewItem* it = lastItem();
	while (it)
	{
		last = it;
		it = it->parent();
	}
	return last;
}

void KbTaskView::SLOT_DoubleClicked(QListViewItem* item, const QPoint&, int)
{
	KbTransferItem *kti = static_cast<KbTransferItem*>(item);
	qWarning("src: %s|%s", kti->SrcFileInfo()->filePath().latin1(), kti->SrcFileInfo()->fileName().latin1());
	qWarning("dst: %s|%s", kti->DstFileInfo()->filePath().latin1(), kti->DstFileInfo()->fileName().latin1());
}

#include "kbtaskview.moc"
