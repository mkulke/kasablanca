//
// C++ Interface: importdialog
//
// Description:
//
//
// Author: Christoph Thielecke <u15119@hs-harz.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <kdialogbase.h>

#include <list>

#include "kbsiteinfo.h"

#include "Q_importdialog.h"

/**
@author Christoph Thielecke
*/
class ImportDialog : public KDialogBase
{
public:
	ImportDialog(QWidget *parent=0, const char *name=0);
	~ImportDialog();
	list<KbSiteInfo>* Bookmarks() { return &m_bookmarks; };
private:
	list<KbSiteInfo> m_bookmarks;
	int m_lines;
	KasablancaImportDialog *mp_dialog;
	void slotOk();
	void importGftpBookmarks(QString fileName);
};

#endif
