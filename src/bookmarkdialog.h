/***************************************************************************
 *   Copyright (C) 2004 by Magnus Kulke                                    *
 *   mkulke@magnusmachine                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <Q_bookmarkdialog.h>
#include <kdialogbase.h>

#include "kbsiteinfo.h"
#include <list>

using namespace std;

/**
@author Magnus Kulke
*/
class BookmarkDialog : public KDialogBase
{
Q_OBJECT
public:
    BookmarkDialog(QWidget *parent = 0, const char *name = 0);
    ~BookmarkDialog();
public slots:
    void slotUser1();
	 void slotUser2();
    void slotOk();
	 void SLOT_TextChanged(const QString& s);
	 void SLOT_StateChanged();
	 void SLOT_SelectionChanged();
private:
	 void ApplyChanges();
    void RefreshEntry(KbSiteInfo *site);
    void EnableInput(bool b);
    KasablancaBookmarkDialog *mp_dialog;
    list<KbSiteInfo> m_bookmarklist;
	 bool m_newitemselected;
};

#endif
