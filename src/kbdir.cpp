/***************************************************************************
                          kbdir.cpp  -  description
                             -------------------
    begin                : Sam Sep 20 2003
    copyright            : (C) 2003 by mkulke
    email                : sikor_sxe@radicalapproach.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 
 
#include <kglobal.h>
#include <kiconloader.h>
#include "kbfileinfo.h"

#include "kbdir.h"

KbDir::KbDir(KbFileInfo kfi, QListView * parent, QListViewItem * after) : KbItem(kfi, parent, after)
{	
	setPixmap(0, KGlobal::iconLoader()->loadIcon("folder",KIcon::Small));	

	if ((kfi.fileName() == ".") || (kfi.fileName() == "..")) delete this;
}

KbDir::KbDir(QListView * parent, QListViewItem * after, QString file, QString path, QString date, off64_t size, unsigned int date_int) : KbItem(parent, after)
{
	setText(0,file);

    QString s;
    s.setNum(size);
    setText(1, s);

    setText(2, date);

    m_path = path;
    m_file = file;
    m_date = date;
    m_size = size;

	m_date_int = date_int;
}
KbDir::~KbDir()
{
}

int KbDir::rtti() const
{
	return KbItem::dir;
}
