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
#ifndef TRANSFERITEM_H
#define TRANSFERITEM_H

#include <kiconloader.h>
#include <kglobal.h>
#include <qlistview.h>
#include <qfileinfo.h>

#include "ftpsession.h"
#include "kbfileinfo.h"

class FtpSession;
class KbStatusTip;

/**
@author Magnus Kulke
*/
class KbTransferItem : public QListViewItem
{
public:
	enum type
   {
		file = 1001,
   	dir
   };
	KbTransferItem(QListViewItem *root, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo* src, KbFileInfo* dst);
	KbTransferItem(QListView *taskview, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo* src, KbFileInfo* dst);
   ~KbTransferItem();

	virtual void Info();
	virtual void ShowProgress(KbStatusTip *statustip);
	void Finish();
	void IncrementStatus() { m_status = m_status + 1; };
	void Init();
	void Xfered(off64_t xfered) { m_xfered = xfered; };
	void StartTimer() { m_time.start(); };
	FtpSession* SrcSession() { return mp_srcsession; };
	KbFileInfo* SrcFileInfo() { return mp_src; };
	FtpSession* DstSession() { return mp_dstsession; };
	KbFileInfo* DstFileInfo() { return mp_dst; };
	int Status() { return m_status; };
	void Abort() { m_status = done; };
	enum status
	{
		clear = 0,
		src_ready,
		dst_ready,
		done
	};	 
	
protected:
	FtpSession *mp_srcsession, *mp_dstsession;
	KbFileInfo *mp_src, *mp_dst;
	int m_status;
	bool m_transfererror;
	off64_t m_xfered;
	QTime m_time;
};

#endif
