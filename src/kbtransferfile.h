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
#ifndef TRANSFERFILE_H
#define TRANSFERFILE_H

#include <kbtransferitem.h>

class KbStatusTip;

/**
@author Magnus Kulke
*/
class KbTransferFile : public KbTransferItem
{
public:
	KbTransferFile(QListView *taskview, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst);
	KbTransferFile(QListViewItem *root, QListViewItem *after, FtpSession *srcsession, FtpSession *dstsession, KbFileInfo *src, KbFileInfo *dst);
    ~KbTransferFile();
	int rtti() const;
	void Info();
	void ShowProgress(KbStatusTip *statustip);
private:
	int m_time_old;
	off64_t m_xfered_old;
	int m_percentage;
	virtual void paintCell( QPainter *painter, const QColorGroup &colorGroup, int column,
		int width, int alignment );
	void PaintPercentageBar(QPainter *painter, int width);
};

#endif
