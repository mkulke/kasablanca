//
// C++ Implementation: KbItem
//
// Description: 
//
//
// Author: mkulke <sikor_sxe@radicalapproach.de>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 

#include <qdatetime.h>
#include "kbfileinfo.h"

#include "kbitem.h"


KbItem::KbItem(KbFileInfo kfi, QListView* parent, QListViewItem* after) : QListViewItem(parent, after)
{
	m_file = kfi.fileName();
	m_path = kfi.dirPath(), 
	m_date = kfi.Date(), 
	m_size = kfi.Size(),
	m_date_int = kfi.DateInt();

	setText(0, m_file);
	setText(1, QString::number(m_size));
	setText(2, m_date);
}

KbItem::KbItem(QListView* parent, QListViewItem* after) : QListViewItem(parent, after)
{
}

KbItem::~KbItem()
{
}

int KbItem::compare(QListViewItem * i, int col, bool ascending) const
{
	if ((this->rtti() == 1001) && (i->rtti() == 1002)) 
	{
		if (ascending) return -1;
		else return 1;
	}
	else if ((this->rtti() == 1002) && (i->rtti() == 1001)) 
	{
		if (ascending) return 1;
		else return -1; 
	}
		
	if (col == 1)
	{
		unsigned int x = this->text(1).toInt();
		unsigned int y = i->text(1).toInt();
		if (x == y) return 0;
		if (x < y) return -1;
		if (x > y) return 1;
	}
	if (col == 2)
	{
		unsigned int x = this->m_date_int;
		unsigned int y = static_cast<KbItem*>(i)->m_date_int;
		if (x == y) return 0;
		if (x < y) return -1;
		if (x > y) return 1;
	}
	return QListViewItem::compare(i, col, ascending);
}

