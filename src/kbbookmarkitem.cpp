//
// C++ Implementation: kbbookmarkitem
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kbbookmarkitem.h"
#include "kbsiteinfo.h"

KbBookmarkItem::KbBookmarkItem(QListView *view, QListViewItem *after, KbSiteInfo *site) : QListViewItem(view, after)
{
	mp_siteinfo = site;
	setText(0, mp_siteinfo->GetName());
}


KbBookmarkItem::~KbBookmarkItem()
{
}


