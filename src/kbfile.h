/***************************************************************************
                          kbfile.h  -  description
                             -------------------
    begin                : Die Sep 23 2003
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

#ifndef FILEITEM_H
#define FILEITEM_H

#include <qlistview.h>
#include "kbitem.h"

class KbFileInfo;

/**
  *@author mkulke
  */

class KbFile : public KbItem  {
public:
	KbFile(KbFileInfo kfi, QListView * parent, QListViewItem * after);
	KbFile(QListView * parent, QListViewItem * after, QString file, QString path, QString date, off64_t size, unsigned int date_int);
	~KbFile();
	int rtti() const;
};

#endif
