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
  
// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 
 
#include <qpainter.h>
#include <qcolor.h>
  
#include "kbstatustip.h"
#include "kbsiteinfo.h"

#include "kbtransferfile.h"

KbTransferFile::KbTransferFile(QListView *taskview, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : KbTransferItem(taskview, after, srcsession, dstsession, src, dst)
{
	setPixmap(0, KGlobal::iconLoader()->loadIcon("files",KIcon::Small));
	m_time_old = -1;
	m_xfered_old = 0;
	m_percentage = 0;
}

KbTransferFile::KbTransferFile(QListViewItem *root, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst) : KbTransferItem(root, after, srcsession, dstsession, src, dst)
{
	setPixmap(0, KGlobal::iconLoader()->loadIcon("files",KIcon::Small));
	m_time_old = -1;
	m_xfered_old = 0;
}

KbTransferFile::~KbTransferFile()
{
}

int KbTransferFile::rtti() const
{
    return KbTransferItem::file;
}

void KbTransferFile::Info()
{
	qWarning("INFO: transfer file from %s to %s", mp_srcsession->name(), mp_dstsession->name());
	qWarning("INFO: mp_src->fileName() = %s", mp_src->fileName().latin1());
	qWarning("INFO: mp_dst->fileName() = %s", mp_dst->fileName().latin1());
	qWarning("INFO: mp_src->dirPath() = %s", mp_src->dirPath(true).latin1());
	qWarning("INFO: mp_dst->dirPath() = %s", mp_dst->dirPath(true).latin1());
}

void KbTransferFile::ShowProgress(KbStatusTip *statustip)
{
	int time = m_time.elapsed();
	int time_dif = time - m_time_old;
	if (time_dif == 0) time_dif = 1;
	off64_t xfer_dif = m_xfered - m_xfered_old;

	off64_t currentsize = (m_xfered + mp_dst->Size()) >> 10;
	off64_t wholesize = mp_src->Size() >> 10;
	off64_t rest = wholesize - currentsize;
	m_percentage = ((currentsize * 100 ) / (wholesize + 1));
	int speed = xfer_dif / time_dif;
	off64_t remaining = rest / (speed + 1);
	
	if ((mp_srcsession->Connected()) && (mp_dstsession->Connected()))
	{
		setText(1, "unknown kb of " + QString::number(wholesize) + "kb");	
		setText(2, "unknown kb/s");	
		setText(4, "unknown");
	}
	else
	{
		setText(1, QString::number(currentsize) + "kb of " + QString::number(wholesize) + "kb");
		setText(2, QString::number(speed) + "kb/s");
		setText(4, QString::number(remaining / 3600) + "h" + QString::number(remaining / 60) + "m" + QString::number(remaining % 60) + "s");
	}
		
	m_time_old = time;
	m_xfered_old = m_xfered;
	
	statustip->ShowStatus(mp_src->fileName()
		+ "," + QString::number(m_percentage) + "%," + QString::number(speed) + "kb/s");
	
	//QToolTip::add(systemtray, mp_src->fileName() 
	//+ "," + QString::number(percentage) + "%," + QString::number(speed) + "kb/s");
}

void KbTransferFile::paintCell( QPainter *painter, const QColorGroup &colorGroup, int column,
		int width, int alignment )
{
	if ((column == 3) 
		&& (m_xfered_old != 0) 
		&& ((!mp_srcsession->Connected()) || (!mp_dstsession->Connected())))
		PaintPercentageBar (painter, width);
	else QListViewItem::paintCell(painter, colorGroup, column, width, alignment);
}

void KbTransferFile::PaintPercentageBar(QPainter * painter, int width)
{
	int len = width * m_percentage / 100;
	painter->fillRect(0, 0, width, height(), Qt::lightGray);
	painter->fillRect(0, 0, len, height(), Qt::gray);
	painter->drawText((width / 2) - 10, height() - 5, QString::number(m_percentage) + "%"); 
}
