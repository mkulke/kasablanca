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
 
#include "kbfileinfo.h"

#include "kbtransferitem.h"

KbTransferItem::KbTransferItem(QListView *taskview, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : QListViewItem(taskview, after)
{
	mp_srcsession = srcsession;
	mp_dstsession = dstsession;
	mp_src = src;
	mp_dst = dst;
	m_status = clear;
	m_transfererror = false;
	m_xfered = 0;

	setText(0, src->fileName());
}

KbTransferItem::KbTransferItem(QListViewItem *root, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : QListViewItem(root, after)
{
	mp_srcsession = srcsession;
	mp_dstsession = dstsession;
	mp_src = src;
	mp_dst = dst;
	m_status = clear;
	m_transfererror = false;
	m_xfered = 0;

	setText(0, src->fileName());
}

KbTransferItem::~KbTransferItem()
{
	delete mp_src;
	delete mp_dst;
}

void KbTransferItem::Info()
{
}

void KbTransferItem::ShowProgress(KbStatusTip *)
{
}

void KbTransferItem::Init()
{
	setSelectable(false);
	listView()->setOpen(this, true);
	setSelected(false);
	repaint();
	mp_srcsession->Occupy();
	mp_dstsession->Occupy();
	mp_srcsession->SetCurrentTransfer(this);
	mp_dstsession->SetCurrentTransfer(this);	
}

void KbTransferItem::Finish()
{	
	mp_srcsession->SetCurrentTransfer(NULL);
	mp_dstsession->SetCurrentTransfer(NULL);
	mp_srcsession->Free();
	mp_dstsession->Free();
}
