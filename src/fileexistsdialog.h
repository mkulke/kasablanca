/***************************************************************************
                          fileexistsdialog.h  -  description
                             -------------------
    begin                : Mi Nov 19 2003
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

#ifndef FILEEXISTSDIALOG_H
#define FILEEXISTSDIALOG_H

#include <qwidget.h>
#include "Q_fileexistsdialog.h"

/**
  *@author mkulke
  */

class FileExistsDialog : public KasablancaFileExistsDialog  {
   Q_OBJECT
public: 
	FileExistsDialog(QWidget *parent=0, const char *name=0);
	~FileExistsDialog();
	enum retval
	{
		skip = 0,
		resume,
		rename,
		overwrite
	};
public slots:
    void Resume();
    void Overwrite();
    void Skip();
	 void Rename();
};

#endif
