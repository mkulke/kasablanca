/***************************************************************************
                          fileexistsdialog.cpp  -  description
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

#include "fileexistsdialog.h"

FileExistsDialog::FileExistsDialog(QWidget *parent, const char *name ) : KasablancaFileExistsDialog(parent,name)
{
}
FileExistsDialog::~FileExistsDialog(){
}

void FileExistsDialog::Overwrite()
{
    done(overwrite);
}

void FileExistsDialog::Resume()
{
    done(resume);
}

void FileExistsDialog::Skip()
{
    done(skip);
}

void FileExistsDialog::Rename()
{
    done(rename);
}
