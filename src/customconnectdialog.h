/***************************************************************************
                          customconnectdialog.h  -  description
                             -------------------
    begin                : Mit Aug 20 2003
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

#ifndef CUSTOMCONNECTDIALOG_H
#define CUSTOMCONNECTDIALOG_H

#include "Q_customconnectdialog.h"

#include <kdialogbase.h>

class KbSiteInfo;

/**
  *@author mkulke
  */

class CustomConnectDialog : public KDialogBase  
{
   Q_OBJECT
public:
    CustomConnectDialog(QWidget *parent=0, const char *name=0);
    ~CustomConnectDialog();
    KbSiteInfo *mp_site; 
public slots:
    void slotOk();
    void SLOT_AnonymousToggled(bool on);
private:
    QString m_user;
    QString m_pass;
	 KasablancaCustomConnectDialog *mp_dialog;
};

#endif
