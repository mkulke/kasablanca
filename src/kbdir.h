/***************************************************************************
                          kbdir.h  -  description
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

#ifndef DIRITEM_H
#define DIRITEM_H

#include "kbitem.h"
#include <qlistview.h>

/**
  *@author mkulke
  */

class KbDir : public KbItem  {
public:
	KbDir(KbFileInfo kfi, QListView * parent, QListViewItem * after);
	KbDir(QListView * parent, QListViewItem * after, QString file, QString path, QString date, off64_t size, unsigned int date_int);
	~KbDir();
	int rtti() const;
};

#endif
