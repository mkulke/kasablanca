//
// C++ Implementation: ftpsession
//
// Description: 
//
//
// Author: Magnus Kulke <sikor_sxe@radicalapproach.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 

#include <klocale.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <klineedit.h>

#include <qtextedit.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qwidget.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qstringlist.h>
#include <qevent.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qpushbutton.h>

#include "fileexistsdialog.h"
#include "customconnectdialog.h"
#include "ftpthread.h"
#include "eventhandler.h"
#include "kbsiteinfo.h"
#include "ftpsession.h"
#include "kasablanca.h"
#include "kbdir.h"
#include "kbdirinfo.h"
#include "kbfile.h"
#include "kbfileinfo.h"
#include "kbconfig.h"
#include "ftplib.h"

FtpSession::FtpSession(QObject *parent, const char *name)
 : QObject(parent, name)
{
	installEventFilter(this);

	mp_ftpthread = new FtpThread();
	mp_eventhandler = new EventHandler(this, "event handler");
	mp_siteinfo = new KbSiteInfo();
	
	m_connected = false;
	m_occupied = false;
	m_encrypted = false;
	m_startqueue = false;
	m_sortascending = true;
	mp_currenttransfer = NULL;
	m_sortpref = 0;
	
	m_colorsuccess = green;
	m_colorfailure = red;
	m_colorlocal = yellow;
		
	mp_eventhandler->SetFtpThread(mp_ftpthread); 
	mp_ftpthread->SetEventReceiver(mp_eventhandler);

	m_iconencrypted = KGlobal::iconLoader()->loadIconSet("encrypted",KIcon::Small).pixmap(QIconSet::Small,QIconSet::Normal);
   m_iconunencrypted = KGlobal::iconLoader()->loadIconSet("encrypted",KIcon::Small).pixmap(QIconSet::Small,QIconSet::Disabled);
	
	connect(mp_eventhandler, SIGNAL(ftp_log(QString, bool)), SLOT(SLOT_Log(QString, bool)));
	connect(mp_eventhandler, SIGNAL(ftp_connect(bool)), SLOT(SLOT_Connect(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_login(bool)), SLOT(SLOT_Login(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_xfered(off64_t, bool)), SLOT(SLOT_Xfered(off64_t, bool)));
	connect(mp_eventhandler, SIGNAL(ftp_quit(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_chdir(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_raw(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_get(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_put(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_fxp(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_rm(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_authtls(bool)), SLOT(SLOT_AuthTls(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_misc(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_mkdir(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_rename(bool)), SLOT(SLOT_Misc(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_pwd(bool, QString)), SLOT(SLOT_Pwd(bool, QString)));
	connect(mp_eventhandler, SIGNAL(ftp_dir(bool, list<KbFileInfo*>, list<KbFileInfo*>)), 
		SLOT(SLOT_Dir(bool, list<KbFileInfo*>, list<KbFileInfo*>)));
	connect(mp_eventhandler, SIGNAL(ftp_encryptdata(bool)), SLOT(SLOT_EncryptData(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_finished()), SLOT(SLOT_Finish()));
	connect(mp_eventhandler, SIGNAL(ftp_connectionlost()), SLOT(SLOT_ConnectionLost()));
	connect(mp_eventhandler, SIGNAL(ftp_transfer(bool)), SLOT(SLOT_Transfer(bool)));
	connect(mp_eventhandler, SIGNAL(ftp_scandir(bool, KbDirInfo*)), SLOT(SLOT_Scandir(bool, KbDirInfo*)));
}

FtpSession::~FtpSession()
{
}

void FtpSession::SetCmdLine(KLineEdit* cmdline)
{ 
	KCompletion *comp = cmdline->completionObject();
 	connect(cmdline, SIGNAL(returnPressed(const QString&)), comp, SLOT(addItem(const QString&)));
	cmdline->setCompletionMode(KGlobalSettings::CompletionAuto);
	mp_cmdline = cmdline;
}

void FtpSession::SetCwdLine(KLineEdit* cwdline)
{ 
	KCompletion *comp = cwdline->completionObject();
 	connect(cwdline, SIGNAL(returnPressed(const QString&)), comp, SLOT(addItem(const QString&)));
	cwdline->setCompletionMode(KGlobalSettings::CompletionAuto);
	mp_cwdline = cwdline;
}

void FtpSession::SLOT_Log(QString log, bool out) 
{ 
	if (out) m_loglist.push_back(make_pair(log, true));
	else m_loglist.push_back(make_pair(log, false));
}

void FtpSession::SLOT_Xfered(off64_t xfered, bool encrypted)
{
	//if (encrypted) mp_encryptionicon->setPixmap(m_iconencrypted);
	//else mp_encryptionicon->setPixmap(m_iconunencrypted);	

	if ((encrypted) && (!m_encrypted)) mp_encryptionicon->setPixmap(m_iconencrypted);
	else if ((!encrypted) && (m_encrypted)) mp_encryptionicon->setPixmap(m_iconunencrypted);	
	m_encrypted = encrypted;
	
	if (mp_currenttransfer) mp_currenttransfer->Xfered(xfered);
}

void FtpSession::SLOT_HeaderClicked(int section)
{
	m_sortascending = m_sortascending ^ true;
	m_sortpref = section;
	SortItems();
}

void FtpSession::SLOT_ActionMenu(int i)
{
	if (Occupied()) 
	{	
		qWarning("ERROR: triggered action while occupied");
		return;
	}
	if (i == Kasablanca::Mkdir)
	{
		bool b;
		QString name = KInputDialog::getText(i18n("Enter directory name"), i18n("Enter directory name:"), "", &b);
		if (!b) return;
		if (Connected())
		{
			Occupy();
			mp_ftpthread->Mkdir(name);
			RefreshBrowser();
			mp_ftpthread->start();
		}
		else 
		{
			m_localworkingdir.mkdir(name);
			UpdateLocal();
		}
	}
	else if (i == Kasablanca::Delete)
	{	
		if (Connected())
		{
			Occupy();
			QListViewItemIterator it(mp_browser);
			while (it.current())
			{
				if (it.current()->isSelected())
				{
					KbItem* item = static_cast<KbItem*>(it.current());
					int warning = KMessageBox::warningContinueCancel(0, i18n("Delete this item?"), item->File());
					if (warning == KMessageBox::Continue)
					{
						if (item->rtti() == KbItem::dir) mp_ftpthread->Rmdir(item->File());
						else if (item->rtti() == KbItem::file) mp_ftpthread->Rm(item->File());
					}
				}
				it++;
			}
			RefreshBrowser();
			mp_ftpthread->start();
		}
		else 
		{
			QListViewItemIterator it(mp_browser);
			while (it.current())
			{
				if (it.current()->isSelected())
				{
					KbItem* item = static_cast<KbItem*>(it.current());
					int warning = KMessageBox::warningContinueCancel(0, i18n("Delete this item?"), item->File());
					if (warning == KMessageBox::Continue)
					{
						if (KbConfig::deleteMovesIntoTrashIsEnabled())
						{
							system(QString("mv " + m_localworkingdir.absPath() + "/" + item->File() +
								+ " " + KGlobalSettings::trashPath()).latin1());
						}
						else
						{
							if (item->rtti() == KbItem::dir) 
							{
								QString dir = "rm -rf " + m_localworkingdir.absFilePath(item->File());
								system(dir.latin1()); 
							}
							//RmdirLocal(item->File());
							else if (item->rtti() == KbItem::file) m_localworkingdir.remove(item->File());
						}
					}
				}
				it++;
			}
			UpdateLocal();
		}
	}
	else if (i == Kasablanca::Rename)
	{
		if (Connected())
		{
			QListViewItemIterator it(mp_browser);
			while (it.current())
			{
				if (it.current()->isSelected()) 
				{	
					bool b;
					KbItem* item = static_cast<KbItem*>(it.current());
					QString name = KInputDialog::getText(i18n("Enter new name"), i18n("Enter new name:"), item->File(), &b);
					if (b) 
					{
						Occupy();
						mp_ftpthread->Rename(item->File(), name);
					}
					else return;
				}
				it++;
			}
			RefreshBrowser();
			mp_ftpthread->start();
		}
		else 
		{
			QListViewItemIterator it(mp_browser);
			while (it.current())
			{
				if (it.current()->isSelected()) 
				{	
					bool b;
					KbItem* item = static_cast<KbItem*>(it.current());
					QString name = KInputDialog::getText(i18n("Enter new name"), i18n("Enter new name:"), item->File(), &b);
					if (b) m_localworkingdir.rename(item->text(0), name);
					else return;
				}
				it++;
			}
			UpdateLocal();
		}
	}
	else if (i == Kasablanca::Queue) 
	{
		m_startqueue = false;
		QueueItems();
	}
	else if (i == Kasablanca::Transfer) 
	{
		m_startqueue = true;
		QueueItems();
	}
	else if (i == Kasablanca::Bookmark) 
	{
		KbSiteInfo newsite = *mp_siteinfo;
		bool b;
		QString name = KInputDialog::getText(i18n("Enter bookmark name"), i18n("Enter bookmark name:"), "", &b);
		if (!b) return;
		if (name == "") name = "New site";
		newsite.SetName(name);
		newsite.SetDefaultDirectory(m_remoteworkingdir);
		static_cast<Kasablanca*>(parent())->m_bookmarks.push_back(newsite);
		KbSiteInfo::WriteBookmarks(static_cast<Kasablanca*>(parent())->m_bookmarks);
		static_cast<Kasablanca*>(parent())->InitBookmarks();
	}
}

void FtpSession::SLOT_ConnectMenu(int i)
{
	if (i == 0) 
	{
		CustomConnectDialog dlg;
    	mp_siteinfo->Clear();
    	dlg.mp_site = mp_siteinfo;
    	if (dlg.exec() == QDialog::Rejected) return; 	
    	else if (!mp_siteinfo->IsLegit()) 
		{
			KMessageBox::error(0,i18n("That site information is not legit."));
			return;
		}
	}
	else 
	{
		list<KbSiteInfo>::iterator it = static_cast<Kasablanca*>(parent())->m_bookmarks.begin();
		for (int x = 0; x < i - 1; x++) it++;
		mp_siteinfo = &(*it);
		qWarning("INFO: connecting to bookmark entry named: %s", mp_siteinfo->GetName().latin1());
	}
	Connect();
	Occupy();
	m_loglist.clear();
	mp_ftpthread->Connect(mp_siteinfo->GetInfo());
	if (mp_siteinfo->GetTls() > 0) mp_ftpthread->Authtls();
	if (mp_siteinfo->GetPasv() > 0) mp_ftpthread->Pasv(true);
	else mp_ftpthread->Pasv(false);
	if (mp_siteinfo->GetCorrectPasv()) mp_ftpthread->Ftp()->SetCorrectPasv(true);
	else mp_ftpthread->Ftp()->SetCorrectPasv(false);
	mp_ftpthread->Login(mp_siteinfo->GetUser(), mp_siteinfo->GetPass());
	if (QString(mp_siteinfo->GetDefaultDirectory()) != "") mp_ftpthread->Chdir(mp_siteinfo->GetDefaultDirectory());
	RefreshBrowser();
	mp_ftpthread->start();
}

void FtpSession::SLOT_Finish()
{
	while (!mp_ftpthread->wait(KB_THREAD_TIMEOUT)) qWarning("WARNING: thread timeout, should *not* happen.");

	if (mp_currenttransfer) Transfer(mp_currenttransfer); 
	else Free();
}

void FtpSession::SLOT_ItemClicked(QListViewItem * item)
{
	if (Occupied()) 
	{	
		qWarning("ERROR: item clicked while occupied");
		return;
	}
	if (item->rtti() == KbItem::file) return;
	else if (Connected())
	{
		Occupy();
		if (item->text(0) == "..") mp_ftpthread->Cdup();
		else mp_ftpthread->Chdir(item->text(0));
		mp_ftpthread->Pwd();
		if (mp_siteinfo->GetTls() > 1) mp_ftpthread->EncryptData(true);
		mp_ftpthread->Dir(false);
		mp_ftpthread->start();
	}
	else UpdateLocal(item->text(0));
}

void FtpSession::SLOT_ItemRClicked(QListViewItem *, const QPoint & point, int)
{
	mp_rclickmenu->exec(point);
}

void FtpSession::SLOT_CmdLine()
{
	if (Occupied()) 
	{	
		qWarning("ERROR: entered command while occupied");
		return;
	}
	else if (Connected())
	{
		Occupy();
		mp_ftpthread->Raw(mp_cmdline->text());
		mp_cmdline->setText("");
		mp_ftpthread->start();
	}
	else
	{
		KProcess* p = new KProcess();
 		p->setWorkingDirectory(m_localworkingdir.absPath());
		*p << QStringList::split(" ", mp_cmdline->text());
		connect(p, SIGNAL(processExited(KProcess*)), SLOT(SLOT_LocalProcessExited(KProcess*)));
		if (p->start() == TRUE) mp_cmdline->setText("");
	}
}

void FtpSession::SLOT_ConnectButton()
{
	/* this button is also an abort button */

	if (Occupied()) 
	{					
		if (mp_currenttransfer) 
		{
			int answer = KMessageBox::warningYesNo(0, i18n("Cancel transfer?"));
			if (answer == KMessageBox::Yes)
			{
				FtpSession *srcsession = mp_currenttransfer->SrcSession();
				FtpSession *dstsession = mp_currenttransfer->DstSession();
			
				mp_currenttransfer->Finish();
				delete mp_currenttransfer;
				if (srcsession->Connected()) srcsession->Abort();
				if (dstsession->Connected()) dstsession->Abort();
			}
		}
		else if (Connected()) Abort();
	}
	
	/* when connected issue disconnect */
	
	else if (Connected())
	{
		Disconnect();
		Occupy();
		mp_ftpthread->Quit();
		mp_ftpthread->start();
	}
	
	/* when disconnected show connect menu */
	
	else if (!Connected()) mp_bookmarksmenu->exec(mp_connectbutton->mapToGlobal(QPoint(0,0)));
}

void FtpSession::SetColors(QColor local, QColor success, QColor failure, QColor background)
{
	m_colorlocal = local;
	m_colorsuccess = success;
	m_colorfailure = failure;
	mp_logwindow->setPaletteBackgroundColor(background);	
}

void FtpSession::SetFont(QFont font)
{
	mp_logwindow->setFont(font);
}

void FtpSession::Abort()
{
	mp_logwindow->setColor(m_colorlocal);
	mp_logwindow->append(i18n("Aborted ftp operation"));
	mp_ftpthread->terminate();
	mp_ftpthread->wait(KB_THREAD_TIMEOUT);
	mp_ftpthread->ClearQueue();
	Disconnect();
	Free();
}
void FtpSession::SLOT_CwdLine()
{
	if (Occupied()) 
	{	
		qWarning("ERROR: entered cwd while occupied");
		return;
	}
	if (Connected())
	{	
		Occupy();
		mp_ftpthread->Chdir(mp_cwdline->text());
		mp_ftpthread->Pwd();
		if (mp_siteinfo->GetTls() > 1) mp_ftpthread->EncryptData(true);
		mp_ftpthread->Dir(false);
		mp_ftpthread->start();
	}
	else UpdateLocal(mp_cwdline->text());		
}

void FtpSession::SLOT_RefreshButton()
{
	if (Occupied()) 
	{	
		qWarning("ERROR: refresh button pressed while occupied");
		return;
	}
	if (Connected())
	{	
		Occupy();
		RefreshBrowser();
		mp_ftpthread->start();
	}
	else UpdateLocal();
}

void FtpSession::QueueItems()
{
	KbDirInfo *dir = new KbDirInfo(WorkingDir());
			
	QListViewItemIterator iit(mp_browser->lastItem());//, QListViewItemIterator::Selected);
	while (iit.current())
	{
		QListViewItem *item = iit.current();
		if (item->isSelected())
		{
			if (item->rtti() == KbItem::dir) dir->AddDirectory( KbFileInfo(static_cast<KbFile*>(item), "/"));
			else if (item->rtti() == KbItem::file) dir->AddFile(new  KbFileInfo(static_cast<KbFile*>(item), "/"));
			mp_browser->setSelected(item, false);
		}
		iit--;
	}
	
	if (Connected())
	{
		Occupy();
		mp_ftpthread->Scandir(dir);
		mp_ftpthread->start();
	}
	else
	{
		list<KbDirInfo*>::iterator end_dir = dir->Dirlist()->end();
		for(list<KbDirInfo*>::iterator i = dir->Dirlist()->begin(); i != end_dir; i++)
		{
			ScandirLocal(*i, '/' + (*i)->fileName());
		}
		/* temporary hack, as there are only 2 ftpsessions available yet */
	
		FtpSession* dst;
		list<FtpSession*>::iterator end_session = mp_sessionlist->end();
		for (list<FtpSession*>::iterator i = mp_sessionlist->begin(); i != end_session; i++) if (*i != this) dst = *i;
		
		/* /temporary hack */
		
		emit gui_queueitems(dir, this, dst, m_startqueue);
	}
}

void FtpSession::SLOT_TransferButton()
{
	if (Occupied()) 
	{	
		qWarning("ERROR: transfer button pressed while occupied");
		return;
	}
	m_startqueue = true;
	QueueItems();
}

void FtpSession::SLOT_Scandir(bool success, KbDirInfo* dir)
{
	if (success) 
	{	
		/* temporary hack, as there are only 2 ftpsessions available yet */
	
		FtpSession* dst;
		list<FtpSession*>::iterator end_dir = mp_sessionlist->end();
		for (list<FtpSession*>::iterator i = mp_sessionlist->begin(); i != end_dir; i++) if (*i != this) dst = *i;
		
		/* /temporary hack */
		
		emit gui_queueitems(dir, this, dst, m_startqueue);
	}
	else qWarning("INFO: scandir error");
}

void FtpSession::SLOT_Connect(bool success)
{
	if (!success) 
	{
		m_loglist.push_back(make_pair(i18n("connection failed"), false));
		Disconnect();
	}
	PrintLog(success);	
}

void FtpSession::SLOT_EncryptData(bool success)
{
	PrintLog(success);	
}

void FtpSession::SLOT_AuthTls(bool success)
{
	if (!success) 
	{
		Disconnect();
		m_loglist.push_back(make_pair(i18n("server does not support encryption"), false));
	}
	PrintLog(success);
}
	
void FtpSession::SLOT_Misc(bool success)
{
	PrintLog(success);
}

void FtpSession::SLOT_Login(bool success)
{
	PrintLog(success);	
	if (!success) Disconnect();
}

void FtpSession::SLOT_ConnectionLost()
{
	PrintLog(false);	
	Disconnect();
}

void FtpSession::SLOT_Dir(bool success, list<KbFileInfo*> dirlist, list<KbFileInfo*> filelist)
{
	PrintLog(success);
	if (success)
	{	
		while (QListViewItem* tmpviewitem = mp_browser->firstChild()) delete tmpviewitem;
		QListViewItem* dirup = new QListViewItem(mp_browser, "..");
		dirup->setPixmap(0, KGlobal::iconLoader()->loadIcon("folder",KIcon::Small));
		dirup->setSelectable(false);	
		
		list<KbFileInfo*>::iterator end_dir = dirlist.end();		
		for (list<KbFileInfo*>::iterator i = dirlist.begin(); i != end_dir; i++)
		{
			new KbDir(**i, mp_browser, mp_browser->lastItem());			
			//if (!KbConfig::dirCachingIsEnabled()) delete *i;
		}	
		list<KbFileInfo*>::iterator end_file = filelist.end();	
		for (list<KbFileInfo*>::iterator i = filelist.begin(); i != end_file; i++)
		{
			new KbFile(**i, mp_browser, mp_browser->lastItem());	
			//if (!KbConfig::dirCachingIsEnabled()) delete *i;
		}	
		SortItems();
		if (KbConfig::hideHiddenFilesIsEnabled()) FilterHiddenFiles(true);
		emit gui_update();
	}
}

void FtpSession::SLOT_Pwd(bool success, QString pwd)
{
	PrintLog(success);
	if (success) 
	{
		m_remoteworkingdir = pwd;
		
		/* strip trailing "/", thanks allanonl for pointing out */
	
		if(pwd.endsWith("/") && pwd.length() > 1) 
		{
			qWarning("INFO: stripping trailing /");
			m_remoteworkingdir.truncate(pwd.length() - 1);
		}
		mp_cwdline->setText(pwd);
	}
}


void FtpSession::SortItems()
{
	QListViewItem* x = mp_browser->firstChild();
	mp_browser->takeItem(x);

	mp_browser->setSorting(m_sortpref, m_sortascending);
	mp_browser->sort();
	mp_browser->setSorting(-1);

	mp_browser->insertItem(x);	
}

void FtpSession::PrintLog(bool)
{
	list<logentries>::iterator i;
	for (i = m_loglist.begin(); i != m_loglist.end(); i++)
	{
		QString line = (*i).first;
			
		if ((*i).second == true)
		{
			if ((line.startsWith("4")) || (line.startsWith("5"))) mp_logwindow->setColor(m_colorfailure);
			else mp_logwindow->setColor(m_colorsuccess);
			mp_logwindow->append((*i).first);
		}
		else
		{
			mp_logwindow->setColor(m_colorlocal);
			if (line.startsWith("PASS")) mp_logwindow->append("PASS *");
			else mp_logwindow->append(line);
		}	
	}	
	m_loglist.clear();
}

void FtpSession::Connect()
{
	mp_rclickmenu->setItemEnabled(Kasablanca::Bookmark, true);
	mp_bookmarksmenu->setEnabled(false);
	mp_connectbutton->setIconSet(KGlobal::iconLoader()->loadIconSet("connect_established",KIcon::Toolbar));
	mp_statusline->setText(i18n("Occupied"));
	m_connected = true;
	emit gui_clearqueue(this);
}

void FtpSession::Disconnect()
{
	UpdateLocal();
	mp_rclickmenu->setItemEnabled(Kasablanca::Bookmark, false);
	mp_bookmarksmenu->setEnabled(true);
	mp_connectbutton->setIconSet(KGlobal::iconLoader()->loadIconSet("connect_no",KIcon::Toolbar));
	mp_statusline->setText(i18n("Disconnected"));
	m_connected = false;
	emit gui_clearqueue(this);
}

void FtpSession::Occupy()
{
	qWarning("INFO: %s gui blocked", name());

	mp_rclickmenu->setEnabled(false);
	mp_browser->setEnabled(false);
	mp_cmdline->setEnabled(false);
	mp_cwdline->setEnabled(false);
	mp_refreshbutton->setEnabled(false);
	if (m_connected) mp_statusline->setText(mp_siteinfo->GetName() + i18n(" is occupied"));
	else mp_statusline->setText("Local " + i18n(" is occupied"));
	m_occupied = true;
	emit gui_update();
}

void FtpSession::Free()
{
	qWarning("INFO: %s gui freed", name());
	
	mp_rclickmenu->setEnabled(true);
	mp_browser->setEnabled(true);
	mp_cmdline->setEnabled(true);
	mp_cwdline->setEnabled(true);
	mp_refreshbutton->setEnabled(true);
	if (m_connected) mp_statusline->setText(i18n("Connected to ") + mp_siteinfo->GetName());
	else mp_statusline->setText(i18n("Disconnected"));	
	if ((mp_siteinfo->GetTls() > 0) && (m_connected)) mp_encryptionicon->setPixmap(m_iconencrypted);
	else mp_encryptionicon->setPixmap(m_iconunencrypted);	
	m_occupied = false;
	emit gui_update();
}

void FtpSession::RefreshBrowser()
{
	mp_ftpthread->Pwd();
	if (mp_siteinfo->GetTls() > 1) mp_ftpthread->EncryptData(true, true);
	mp_ftpthread->Dir(true);
}

void FtpSession::UpdateLocal(QString cwd)
{
	QFileInfoList filelist, dirlist;

	if (cwd != "") if (!m_localworkingdir.cd(cwd)) return;
	if (!m_localworkingdir.exists()) return;
		
	mp_browser->sortColumn();
	m_localworkingdir.setSorting(QDir::Name);
	
	while (QListViewItem* tmpviewitem = mp_browser->firstChild()) delete tmpviewitem;

	QListViewItem* dirup = new QListViewItem(mp_browser, "..");
	dirup->setPixmap(0, KGlobal::iconLoader()->loadIcon("folder",KIcon::Small));
	dirup->setSelectable(false);

	m_localworkingdir.setFilter(QDir::Dirs | QDir::Hidden);
	dirlist = *m_localworkingdir.entryInfoList();
	
	QFileInfoListIterator dit(dirlist);
	dit.atFirst();
	while (dit.current())
	{
		new KbDir(KbFileInfo(*dit.current()), mp_browser, mp_browser->lastItem());
		++dit;
	}
	
	m_localworkingdir.setFilter(QDir::Files | QDir::Hidden);
	filelist = *m_localworkingdir.entryInfoList();
	
	QFileInfoListIterator fit(filelist);
	fit.atFirst();
	while (fit.current())
	{
		new KbFile(KbFileInfo(*fit.current()), mp_browser, mp_browser->lastItem());
		++fit;
	}
	
	mp_cwdline->setText(m_localworkingdir.absPath());
	
	SortItems();
	if (KbConfig::hideHiddenFilesIsEnabled()) FilterHiddenFiles(true);
}

/*bool FtpSession::RmdirLocal(QString dir)
{
	QStringList filelist, dirlist;
	QString olddir;
	
	olddir = m_localworkingdir.path();
	if (!m_localworkingdir.cd(dir)) return false;
	
	filelist = m_localworkingdir.entryList("*", QDir::Files | QDir::Hidden); 
	for (QStringList::Iterator it = filelist.begin(); it != filelist.end(); ++it) 
	{
		if (!m_localworkingdir.remove(*it)) return false;
	}
	
	dirlist = m_localworkingdir.entryList("*", QDir::Dirs | QDir::Hidden);
	for (QStringList::Iterator it = dirlist.begin(); it != dirlist.end(); ++it) 
	{
		if ((*it != ".") && (*it != "..")) 
		{
			if (!RmdirLocal(*it)) return false;
		}
	}
	
	if (!m_localworkingdir.cd(olddir)) return false;
	if (!m_localworkingdir.rmdir(dir)) return false;
	
	return true;
}*/

bool FtpSession::ScandirLocal(KbDirInfo *dir, QString path)
{
	QFileInfoList filelist, dirlist;
	QString olddir;
	
	qWarning("dir: path: %s file: %s", dir->dirPath(true).latin1(), dir->fileName().latin1());
	
	olddir = WorkingDir();
	if (!m_localworkingdir.cd(dir->fileName())) return false;
	
	m_localworkingdir.setFilter(QDir::Files | QDir::Hidden);
	filelist = *m_localworkingdir.entryInfoList();
	
	QFileInfoListIterator fit(filelist);
	fit.atFirst();
	while (fit.current())
	{
		KbFileInfo *kfi = new KbFileInfo(*fit.current());
		kfi->SetDirPath(path);
		dir->AddFile(kfi);
		qWarning("file: path: %s file: %s", kfi->dirPath(true).latin1(), kfi->fileName().latin1());
		++fit;
	}
	
	m_localworkingdir.setFilter(QDir::Dirs | QDir::Hidden);
	dirlist = *m_localworkingdir.entryInfoList();
	
	list<KbFileInfo> kfilist;
	
	QFileInfoListIterator dit(dirlist);
	dit.atFirst();
	while (dit.current())
	{
		QFileInfo qfi = *dit.current();
		KbFileInfo kfi(qfi);
		kfi.SetDirPath(path);
		kfilist.push_back(kfi);
		++dit;
	}
	
	list<KbFileInfo>::iterator end_kfi = kfilist.end();
	
	for(list<KbFileInfo>::iterator kfiit = kfilist.begin(); kfiit != end_kfi; kfiit++)
	{	
		KbDirInfo* newdir = dir->AddDirectory(*kfiit);
		if (newdir) if (!ScandirLocal(newdir, path + '/' + newdir->fileName())) return false;
	}
	
	if (!m_localworkingdir.cd(olddir)) return false;
	
	return true;
}

QString FtpSession::WorkingDir()
{
	if (Connected()) return m_remoteworkingdir;
	else return m_localworkingdir.absPath();
}

bool FtpSession::CheckLocalDirectory(QString path)
{
	if (Connected())
	{
		qWarning("ERROR: CheckLocalDirectory on connected session called");
		return false;
	}

	if (path == m_localworkingdir.absPath()) return true;
	else
	{
		bool result;
		result = m_localworkingdir.cd(path);
		if (result) UpdateLocal();
		return result;
	}
}

bool FtpSession::MakeLocalDirectory(QString path)
{
	if (Connected())
	{
		qWarning("ERROR: MakeLocalDirectory on connected session called");
		return false;
	}
	
	bool result;
	result = m_localworkingdir.mkdir(path);
	if (result) UpdateLocal();
	return result;
}

void FtpSession::MakeDirectory(QString dir)
{
	bool result = mp_ftpthread->Transfer_Mkdir(dir);			
	if (result) mp_ftpthread->start();
	else qWarning("ERROR: thread error, thread was still busy.");
}

bool FtpSession::CopyLocalFile(KbTransferItem* item)
{
	QTextStream srcstream;
	QTextStream dststream;
	QFile srcfile(item->SrcFileInfo()->filePath());
	QFile dstfile(item->DstFileInfo()->filePath());		
	if (!dstfile.open(IO_WriteOnly))
	{
		qWarning("ERROR: writing to local destination file not allowed");
		return false;
	}
	dststream.setDevice(&dstfile);	
	if (!srcfile.open(IO_ReadOnly)) 
	{
		qWarning("ERROR: reading from local source file not allowed");
		return false;
	}	
	srcstream.setDevice(&srcfile);
	dststream << srcstream.read();
	srcfile.close(); 
	dstfile.close();
	
	return true;
}

void FtpSession::ChangeDirectory(QString path)
{
	bool result = mp_ftpthread->Transfer_Changedir(path, (mp_siteinfo->GetTls() > 1));			
	if (result) mp_ftpthread->start();
	else qWarning("ERROR: thread error, thread was still busy.");
}

void FtpSession::SLOT_Transfer(bool success)
{
	if (success) mp_currenttransfer->IncrementStatus();
	else mp_currenttransfer->Abort();
}

int FtpSession::CheckFile(KbTransferItem *item)
{	
	item->DstFileInfo()->SetSize(0);
	QListViewItemIterator it(mp_browser);
	
	while (it.current())
	{
		if (it.current()->text(0) == item->DstFileInfo()->fileName()) 
		{
			FileExistsDialog dlg;
			KbItem* kbi;
			QString newname;
			bool b;
			int result;
			
			kbi = static_cast<KbItem*>(it.current());
			item->DstFileInfo()->SetSize(kbi->Size());
			if (kbi->rtti() == KbItem::dir)
			{
				dlg.ResumeButton->setEnabled(false);
				dlg.OverwriteButton->setEnabled(false);
			}
			else if (kbi->rtti() == KbItem::file)
			{
				if (item->DstFileInfo()->Size() >= item->SrcFileInfo()->Size()) dlg.ResumeButton->setEnabled(false);
			}
			
			if (m_onfileexistsdefault != off)
			{			
				if (m_onfileexistsdefault == clear) item->DstFileInfo()->SetSize(0);
				if ((m_onfileexistsdefault == resume) &&
					(item->DstFileInfo()->Size() >= item->SrcFileInfo()->Size())
				) return skip;
				return m_onfileexistsdefault;
			}
			else result = dlg.exec();
			
			switch (result)
			{
				case FileExistsDialog::overwrite:
					item->DstFileInfo()->SetSize(0);
					return clear;
					break;
				case FileExistsDialog::resume:
					return resume;
					break;
				case FileExistsDialog::rename:
					newname = KInputDialog::getText(i18n("Enter New Name"), i18n("Enter New Name:"), kbi->File() + i18n("_alt"), &b);
					if (!b) return skip;
					else
					{
						item->DstFileInfo()->setFile(newname);
						return CheckFile(item);
					}
					break;
				case FileExistsDialog::skip:
					return skip;
					break;
			}
		}
		it++;
	}	
	return clear;
}

void FtpSession::Transfer(KbTransferItem *item)
{	
	if (item->Status() == KbTransferItem::clear)
	{
		item->Info();
		item->Init();	
		if (!item->SrcSession()->Connected()) 
		{
			if (item->SrcSession()->CheckLocalDirectory(item->SrcFileInfo()->dirPath(true)) == false)
			{
				emit gui_succeedtransfer(mp_currenttransfer);
				return;
			}
			else item->IncrementStatus();
		}
		else item->SrcSession()->ChangeDirectory(item->SrcFileInfo()->dirPath(true));
	}
	
	if (item->Status() == KbTransferItem::src_ready)
	{
		if (!item->DstSession()->Connected()) 
		{
			if (item->DstSession()->CheckLocalDirectory(item->DstFileInfo()->dirPath(true)) == false)
			{
				emit gui_succeedtransfer(mp_currenttransfer);
				return;
			}
			else item->IncrementStatus();
		}
		else item->DstSession()->ChangeDirectory(item->DstFileInfo()->dirPath(true));
	}
	
	if (item->Status() == KbTransferItem::dst_ready)
	{
		if (item->rtti() == KbTransferItem::dir)
		{
			if (item->DstSession()->Connected()) item->DstSession()->MakeDirectory(item->DstFileInfo()->fileName());
			else
			{
				if (item->DstSession()->MakeLocalDirectory(item->DstFileInfo()->fileName()) == false)
				{
					emit gui_succeedtransfer(mp_currenttransfer);
					return;
				}
				else item->IncrementStatus();
			}
		}
		else if (item->rtti() == KbTransferItem::file)
		{
			filecheck result = static_cast<filecheck>(item->DstSession()->CheckFile(item));
		
			startTimer(1024);
			item->StartTimer();
			
			if (result == skip)
			{
				qWarning("INFO: skipped transfer");
				item->IncrementStatus();
			}
			else if (item->SrcSession()->Connected()) 
			{
				if (item->DstSession()->Connected())
				{
					item->SrcSession()->FxpFile(item, result);
				}
				else item->SrcSession()->GetFile(item, result);
			}
			else
			{
				if (item->DstSession()->Connected()) item->DstSession()->PutFile(item, result);	
				else
				{
					if (CopyLocalFile(item) == false)
					{
						emit gui_succeedtransfer(mp_currenttransfer);
						return;
					}	
					else item->IncrementStatus();
				}
			}	
		}		
	}
	
	if (item->Status() == KbTransferItem::done) 
	{
		killTimers();
		emit gui_succeedtransfer(mp_currenttransfer);
	}
}

void FtpSession::FxpFile(KbTransferItem *item, filecheck fc)
{
	bool result;
	int srctls, dsttls, alternativefxp;
	QString localfile, remotefile;
	off64_t offset;

	remotefile = item->SrcFileInfo()->fileName();
	localfile = mp_currenttransfer->DstSession()->WorkingDir() + '/' + item->DstFileInfo()->fileName();
	srctls = mp_siteinfo->GetTls();
	dsttls = item->DstSession()->SiteInfo()->GetTls();
	FtpThread* dstftp = item->DstSession()->Ftp();
	offset = item->DstFileInfo()->Size();
	
	alternativefxp = mp_siteinfo->GetAlternativeFxp() | mp_currenttransfer->DstSession()->mp_siteinfo->GetAlternativeFxp();
	
	if (fc == resume) result = mp_ftpthread->Transfer_Fxp(remotefile, localfile, dstftp, srctls, dsttls, offset, alternativefxp);
	else result = mp_ftpthread->Transfer_Fxp(remotefile, localfile, dstftp, srctls, dsttls, 0, alternativefxp);
	
	if (result) mp_ftpthread->start();
	else qWarning("ERROR: thread error, thread was still busy.");
}

void FtpSession::GetFile(KbTransferItem *item, filecheck fc)
{
	bool result;
	int tls;
	QString localfile, remotefile;

	remotefile = item->SrcFileInfo()->fileName();
	localfile = mp_currenttransfer->DstSession()->WorkingDir() + '/' + item->DstFileInfo()->fileName();
	tls = mp_siteinfo->GetTls();
	
	if (fc == resume) result = mp_ftpthread->Transfer_Get(remotefile, localfile, tls, item->DstFileInfo()->Size());
	else result = mp_ftpthread->Transfer_Get(remotefile, localfile, tls);
	
	if (result) mp_ftpthread->start();
	else qWarning("ERROR: thread error, thread was still busy.");
}

void FtpSession::PutFile(KbTransferItem *item, filecheck fc)
{
	bool result;
	int tls;
	QString localfile, remotefile;

	remotefile = item->SrcFileInfo()->fileName();
	localfile = mp_currenttransfer->SrcSession()->WorkingDir() + '/' + item->DstFileInfo()->fileName();
	tls = mp_siteinfo->GetTls();
	
	if (fc == resume) result = mp_ftpthread->Transfer_Put(localfile, remotefile, tls, item->DstFileInfo()->Size());
	else result = mp_ftpthread->Transfer_Put(localfile, remotefile, tls);
	
	if (result) mp_ftpthread->start();
	else qWarning("ERROR: thread error, thread was still busy.");
}

void FtpSession::timerEvent(QTimerEvent*)
{
	if (mp_currenttransfer) mp_currenttransfer->ShowProgress(static_cast<Kasablanca*>(parent())->statusTip());
}

void FtpSession::EnableCmdLine(bool b) 
{ 
	if (b) mp_cmdline->show();
	else mp_cmdline->hide();
}

void FtpSession::FilterHiddenFiles(bool b)
{
	QListViewItemIterator it(mp_browser);
	while (it.current()) 
	{		
		if ((it.current() != mp_browser->firstChild()) && (static_cast<KbItem*>(it.current())->File().startsWith(".")))
		{
			if (b) it.current()->setVisible(false);
			else it.current()->setVisible(true);
		}
		++it;
	}
}

#include "ftpsession.moc"
