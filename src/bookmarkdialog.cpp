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
#include <qdom.h>
#include <qfile.h>
#include <qdir.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qtabwidget.h>
#include <qaction.h>

#include <kcombobox.h>
#include <kcompletion.h>
#include <klocale.h>
#include <klineedit.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <klistview.h>

#include "bookmarkdialog.h"
#include "kbbookmarkitem.h"

BookmarkDialog::BookmarkDialog(QWidget *parent, const char *name)
: KDialogBase(parent, name, true, i18n( "Bookmarks" ),
              KDialogBase::Ok | 
				  KDialogBase::Help | 
				  KDialogBase::Cancel | 
				  KDialogBase::User1 | 
				  KDialogBase::User2,
              KDialogBase::Ok, true , KGuiItem(i18n("New")), KGuiItem(i18n("Remove")))
{
	mp_dialog = new KasablancaBookmarkDialog(this);
	setMainWidget(mp_dialog);
	setHelp("ftphelp");
	
	m_bookmarklist = KbSiteInfo::ParseBookmarks();
	mp_dialog->BookmarkListView->header()->hide();
	mp_dialog->BookmarkListView->setSorting(-1);
	
	KbBookmarkItem *after = NULL;
	
	list<KbSiteInfo>::iterator end_bookmarks = m_bookmarklist.end();
	for (list<KbSiteInfo>::iterator i = m_bookmarklist.begin(); i != end_bookmarks; i++)
		after = new KbBookmarkItem(mp_dialog->BookmarkListView, after, &(*i));
		
	 connect(mp_dialog->NameEdit, SIGNAL(textChanged(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->InfoEdit, SIGNAL(textChanged(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->UserEdit, SIGNAL(textChanged(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->PassEdit, SIGNAL(textChanged(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->DefaultDirectoryEdit, SIGNAL(textChanged(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->EncryptionComboBox, SIGNAL(activated(const QString&)), SLOT(SLOT_TextChanged(const QString&)));
	 connect(mp_dialog->ModeComboBox, SIGNAL(activated(const QString&)), SLOT(SLOT_TextChanged(const QString&))); 
	 connect(mp_dialog->AlternativeFxpCheckBox, SIGNAL(clicked()), SLOT(SLOT_StateChanged()));
	 connect(mp_dialog->CorrectPasvCheckBox, SIGNAL(clicked()), SLOT(SLOT_StateChanged()));
	 
	 connect(mp_dialog->BookmarkListView, SIGNAL(selectionChanged()), SLOT(SLOT_SelectionChanged()));
	 
	 m_newitemselected = false;
	 
	 enableButton(KDialogBase::User2, false);
	 
	 QAction *deleteShortcut = new QAction( QPixmap(), "&Delete",
                                      Key_Delete, this, "delete" );
									  
	 connect( deleteShortcut, SIGNAL( activated() ) , this, SLOT( slotUser2() ) );
}

BookmarkDialog::~BookmarkDialog()
{
}

#include "bookmarkdialog.moc"

void BookmarkDialog::SLOT_SelectionChanged()
{
	KbBookmarkItem *kbb;
	KbSiteInfo *s;
	kbb = static_cast<KbBookmarkItem*>(mp_dialog->BookmarkListView->selectedItem());
	if (!kbb)
	{
		EnableInput(false);
		return;
	}
	
	m_newitemselected = true;
	s = kbb->GetSiteInfo();
	
	EnableInput(true);
	RefreshEntry(s);
}

void BookmarkDialog::EnableInput(bool b)
{
    mp_dialog->NameEdit->setEnabled(b);
    mp_dialog->UserEdit->setEnabled(b);
    mp_dialog->PassEdit->setEnabled(b);
    mp_dialog->InfoEdit->setEnabled(b);
    mp_dialog->EncryptionComboBox->setEnabled(b);
    mp_dialog->ModeComboBox->setEnabled(b);
	 mp_dialog->AlternativeFxpCheckBox->setEnabled(b);
	 mp_dialog->CorrectPasvCheckBox->setEnabled(b);
	 mp_dialog->DefaultDirectoryEdit->setEnabled(b);
	 enableButton(KDialogBase::User2, b);
}

void BookmarkDialog::slotOk()
{
	list<KbSiteInfo> newbookmarklist;
	
	slotApply();
	
   QListViewItemIterator it(mp_dialog->BookmarkListView);
   while (it.current()) 
	{
		newbookmarklist.push_back(*(static_cast<KbBookmarkItem*>(it.current())->GetSiteInfo()));
   	++it;
	}
	
	KbSiteInfo::WriteBookmarks(newbookmarklist);
	accept();
}

void BookmarkDialog::slotUser1()
{
	KbBookmarkItem* newentry;
	
	KbSiteInfo newsite;
	newsite.SetName("New Site");
	newsite.SetInfo("newftp:21");
	newsite.SetUser("anonymous");
	newsite.SetPass("bla@bla.com");
	newsite.SetPasv(1);
	newsite.SetTls(0);
	newsite.SetAlternativeFxp(0);
	newsite.SetCorrectPasv(0);
	newsite.SetDefaultDirectory("");
		
	m_bookmarklist.push_back(newsite);
	
	newentry = new KbBookmarkItem(mp_dialog->BookmarkListView, mp_dialog->BookmarkListView->lastItem(), &m_bookmarklist.back());
	mp_dialog->BookmarkListView->setSelected(newentry, true);
}

void BookmarkDialog::slotUser2()
{
	delete mp_dialog->BookmarkListView->selectedItem();
}

void BookmarkDialog::RefreshEntry(KbSiteInfo* site)
{
	mp_dialog->NameEdit->setText(site->GetName());
	mp_dialog->UserEdit->setText(site->GetUser());
	mp_dialog->PassEdit->setText(site->GetPass());
	mp_dialog->InfoEdit->setText(site->GetInfo());
	mp_dialog->EncryptionComboBox->setCurrentItem(site->GetTls());
	mp_dialog->ModeComboBox->setCurrentItem(site->GetPasv());
	mp_dialog->AlternativeFxpCheckBox->setChecked(site->GetAlternativeFxp());
	mp_dialog->CorrectPasvCheckBox->setChecked(site->GetCorrectPasv());
	mp_dialog->DefaultDirectoryEdit->setText(site->GetDefaultDirectory());
	
	m_newitemselected = false;
}

void BookmarkDialog::SLOT_StateChanged()
{
	if (!m_newitemselected) ApplyChanges();
}

void BookmarkDialog::SLOT_TextChanged(const QString&)
{
	if (!m_newitemselected) ApplyChanges();
}

void BookmarkDialog::ApplyChanges()
{
	KbSiteInfo *siteinfo;
	KbBookmarkItem *kbb;
	
	kbb = static_cast<KbBookmarkItem*>(mp_dialog->BookmarkListView->selectedItem());
	if (!kbb) return;
	siteinfo = kbb->GetSiteInfo();	
	
	siteinfo->SetName(mp_dialog->NameEdit->text());
	siteinfo->SetUser(mp_dialog->UserEdit->text());
   siteinfo->SetPass(mp_dialog->PassEdit->text());
   siteinfo->SetInfo(mp_dialog->InfoEdit->text());
   siteinfo->SetTls(mp_dialog->EncryptionComboBox->currentItem());
   siteinfo->SetPasv(mp_dialog->ModeComboBox->currentItem());
	siteinfo->SetAlternativeFxp(mp_dialog->AlternativeFxpCheckBox->isOn());
	siteinfo->SetCorrectPasv(mp_dialog->CorrectPasvCheckBox->isOn());
	siteinfo->SetDefaultDirectory(mp_dialog->DefaultDirectoryEdit->text());

	kbb->setText(0, mp_dialog->NameEdit->text());
}
