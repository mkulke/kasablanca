//
// C++ Interface: kbbookmarkitem
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KBBOOKMARKITEM_H
#define KBBOOKMARKITEM_H

#include <qlistview.h>

class KbSiteInfo;

/**
@author Magnus Kulke
*/
class KbBookmarkItem : public QListViewItem
{
public:
	KbBookmarkItem(QListView *view, QListViewItem *after, KbSiteInfo *site);
	~KbBookmarkItem();
	KbSiteInfo* GetSiteInfo() { return mp_siteinfo; };
private: 
	KbSiteInfo* mp_siteinfo;

};

#endif
