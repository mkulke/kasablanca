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
 
#include "kbtransferdir.h"

KbTransferDir::KbTransferDir(QListView *taskview, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : KbTransferItem(taskview, after, srcsession, dstsession, src, dst)
{
	setPixmap(0, KGlobal::iconLoader()->loadIcon("folder",KIcon::Small));
}

KbTransferDir::KbTransferDir(QListViewItem *root, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : KbTransferItem(root, after, srcsession, dstsession, src, dst)
{
	setPixmap(0, KGlobal::iconLoader()->loadIcon("folder",KIcon::Small));
}

KbTransferDir::~KbTransferDir()
{
}

int KbTransferDir::rtti() const
{
    return KbTransferItem::dir;
}

void KbTransferDir::Info()
{
	qWarning("INFO: transfer dir from %s to %s", mp_srcsession->name(), mp_dstsession->name());
	qWarning("INFO: mp_src->fileName() = %s", mp_src->fileName().latin1());
	qWarning("INFO: mp_dst->fileName() = %s", mp_dst->fileName().latin1());
	qWarning("INFO: mp_src->dirPath() = %s", mp_src->dirPath(true).latin1());
	qWarning("INFO: mp_dst->dirPath() = %s", mp_dst->dirPath(true).latin1());
}


