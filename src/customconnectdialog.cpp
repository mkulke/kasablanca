/***************************************************************************
                          customconnectdialog.cpp  -  description
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
 
#include <klocale.h>

#include <qstring.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
 
#include "kbsiteinfo.h"
#include "customconnectdialog.h"

CustomConnectDialog::CustomConnectDialog(QWidget *parent, const char *name ) : KDialogBase(parent, name, 
	true, i18n( "Custom Site" ), KDialogBase::Ok | KDialogBase::Cancel | KDialogBase::Help, KDialogBase::Ok, true)
{
	setHelp("ftphelp");
	mp_dialog = new KasablancaCustomConnectDialog(this);
	setMainWidget(mp_dialog);
	
	connect(mp_dialog->AnonymousCheckBox, SIGNAL(toggled(bool)), SLOT(SLOT_AnonymousToggled(bool)));
}

CustomConnectDialog::~CustomConnectDialog()
{
}

void CustomConnectDialog::slotOk()
{
	mp_site->SetName(mp_dialog->InfoEdit->text());
	mp_site->SetInfo(mp_dialog->InfoEdit->text());
	mp_site->SetUser(mp_dialog->UserLineEdit->text());
	mp_site->SetPass(mp_dialog->PassLineEdit->text());
	mp_site->SetTls(mp_dialog->EncryptionComboBox->currentItem());
	mp_site->SetPasv(mp_dialog->ModeComboBox->currentItem() ^ 1);
	mp_site->SetDefaultDirectory(mp_dialog->DefaultDirectoryEdit->text());
	mp_site->SetAlternativeFxp(mp_dialog->AlternativeFxpCheckBox->isOn());
	mp_site->SetCorrectPasv(mp_dialog->CorrectPasvCheckBox->isOn());
	
	accept();
}

void CustomConnectDialog::SLOT_AnonymousToggled(bool on)
{
	mp_dialog->UserLineEdit->setEnabled(!on);
	mp_dialog->PassLineEdit->setEnabled(!on);
	
	if(on) {
		m_user = mp_dialog->UserLineEdit->text();
		m_pass = mp_dialog->PassLineEdit->text();
		mp_dialog->UserLineEdit->setText("anonymous");
		mp_dialog->PassLineEdit->setText("some@email.org");
	} 
	else 
	{
		mp_dialog->UserLineEdit->setText(m_user);
		mp_dialog->PassLineEdit->setText(m_pass);
	}
}
