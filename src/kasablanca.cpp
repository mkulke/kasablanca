/***************************************************************************
                          kasablanca.cpp  -  description
                             -------------------
    begin                : Die Aug 19 20:54:14 CEST 2003
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

#include <kaction.h>
#include <kapp.h>
#include <kapplication.h>
#include <kdeversion.h>
#include <kedittoolbar.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kicontheme.h>
#include <kkeydialog.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kstdaction.h>
#include <ksystemtray.h>
#include <kconfigdialog.h>
#include <kpassivepopup.h>
#include <klineedit.h>

#include <qtextedit.h>
#include <qcolor.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qheader.h>
#include <qtoolbutton.h>
#include <qaction.h>

#include "Q_colorspreferencesdialog.h"
#include "Q_generalpreferencesdialog.h"
#include "Q_userinterfacepreferencesdialog.h"
#include "importdialog.h"
#include "customconnectdialog.h"
#include "fileexistsdialog.h"
#include "bookmarkdialog.h"
#include "kbtransferdir.h"
#include "kbtransferfile.h"
#include "kbdirinfo.h"
#include "ftpsession.h"
#include "kbconfig.h"
#include "kbtaskview.h"
#include "kasablanca.h"
#include "kbstatustip.h"

using namespace std;

Kasablanca::Kasablanca() : KMainWindow( 0, "Kasablanca" ), mp_view(new KasablancaMainWindow(this))
{
	//first, load the system tray icon
	mp_systemtray = new KSystemTray(this);
	mp_systemtray->setPixmap( KSystemTray::loadIcon( "kasablanca" ) );
	if ( KbConfig::systrayIsEnabled() ) mp_systemtray->show();
	
	mp_statustip = new KbStatusTip(mp_systemtray);
	
	mp_session_a = new FtpSession(this, "session a");
	mp_session_b = new FtpSession(this, "session b");
	mp_sessionlist = new list<FtpSession*>;
	mp_sessionlist->push_back(mp_session_a);
	mp_sessionlist->push_back(mp_session_b);

	setAcceptDrops(true);
	setCentralWidget(mp_view);
	setupGUI();
	setupActions();
	setupStatusBar();
	setupMenu();
	statusBar()->show();

	setAutoSaveSettings();

	/* when the qmainwindow is closed the object gets deleted,
	so the destructers of kbprocesses are called. */

	setWFlags(WDestructiveClose);

	/* if initbookmarks returns false, an error box appears */

	InitBookmarks();

	//if (!InitBookmarks() != 1) KMessageBox::error(0, i18n("Could not open Kasablanca bookmark xml."));

	connect(mp_view->BrowserB, SIGNAL(selectionChanged()), SLOT(SLOT_SelectionChanged()));
	connect(mp_view->BrowserA, SIGNAL(selectionChanged()), SLOT(SLOT_SelectionChanged()));
	connect(mp_view->TaskView, SIGNAL(selectionChanged()), SLOT(SLOT_SelectionChanged()));

	mp_session_a->SetLogWindow(mp_view->LogWindowA);
	mp_session_a->SetBrowser(mp_view->BrowserA);
	mp_session_a->SetConnectButton(mp_view->ConnectButtonA);
	mp_session_a->SetRefreshButton(mp_view->RefreshButtonA);
	mp_session_a->SetTransferButton(mp_view->TransferButtonA);
	mp_session_a->SetCmdLine(mp_view->CommandLineA);
	mp_session_a->SetCwdLine(mp_view->CwdLineA);
	mp_session_a->SetBookmarksMenu(&m_bookmarksmenu_a);
	mp_session_a->SetStatusLine(mp_statusline_a);
	mp_session_a->SetEncryptionIcon(mp_encryptionicon_a);
	mp_session_a->SetRclickMenu(&m_rclickmenu_a);
	mp_session_a->SetSessionList(mp_sessionlist);

	connect(mp_view->RefreshButtonA, SIGNAL(clicked()), mp_session_a, SLOT(SLOT_RefreshButton()));
	connect(mp_view->ConnectButtonA, SIGNAL(clicked()), mp_session_a, SLOT(SLOT_ConnectButton()));
	connect(mp_view->TransferButtonA, SIGNAL(clicked()), mp_session_a, SLOT(SLOT_TransferButton()));
	connect(mp_view->CwdLineA, SIGNAL(returnPressed()), mp_session_a, SLOT(SLOT_CwdLine()));
	connect(mp_view->CommandLineA, SIGNAL(returnPressed()), mp_session_a, SLOT(SLOT_CmdLine()));

	mp_session_b->SetLogWindow(mp_view->LogWindowB);
	mp_session_b->SetBrowser(mp_view->BrowserB);
	mp_session_b->SetConnectButton(mp_view->ConnectButtonB);
	mp_session_b->SetRefreshButton(mp_view->RefreshButtonB);
	mp_session_b->SetTransferButton(mp_view->TransferButtonB);
	mp_session_b->SetCmdLine(mp_view->CommandLineB);
	mp_session_b->SetCwdLine(mp_view->CwdLineB);
	mp_session_b->SetBookmarksMenu(&m_bookmarksmenu_b);
	mp_session_b->SetStatusLine(mp_statusline_b);
	mp_session_b->SetEncryptionIcon(mp_encryptionicon_b);
	mp_session_b->SetRclickMenu(&m_rclickmenu_b);
	mp_session_b->SetSessionList(mp_sessionlist);

	connect(mp_view->RefreshButtonB, SIGNAL(clicked()), mp_session_b, SLOT(SLOT_RefreshButton()));
	connect(mp_view->ConnectButtonB, SIGNAL(clicked()), mp_session_b, SLOT(SLOT_ConnectButton()));
	connect(mp_view->TransferButtonB, SIGNAL(clicked()), mp_session_b, SLOT(SLOT_TransferButton()));
	connect(mp_view->CwdLineB, SIGNAL(returnPressed()), mp_session_b, SLOT(SLOT_CwdLine()));
	connect(mp_view->CommandLineB, SIGNAL(returnPressed()), mp_session_b, SLOT(SLOT_CmdLine()));
	connect(mp_session_a, SIGNAL(gui_update()), SLOT(SLOT_SelectionChanged()));
	connect(mp_session_b, SIGNAL(gui_update()), SLOT(SLOT_SelectionChanged()));
	connect(mp_session_a, SIGNAL(gui_queueitems(KbDirInfo*, FtpSession*, FtpSession*, bool)),
	 	SLOT(SLOT_QueueItems(KbDirInfo*, FtpSession*, FtpSession*, bool)));
	connect(mp_session_b, SIGNAL(gui_queueitems(KbDirInfo*, FtpSession*, FtpSession*, bool)),
	 	SLOT(SLOT_QueueItems(KbDirInfo*, FtpSession*, FtpSession*, bool)));
	connect(mp_session_a, SIGNAL(gui_succeedtransfer(QListViewItem*)), SLOT(SLOT_NextTransfer(QListViewItem*)));
	connect(mp_session_b, SIGNAL(gui_succeedtransfer(QListViewItem*)), SLOT(SLOT_NextTransfer(QListViewItem*)));
	connect(mp_session_a, SIGNAL(gui_clearqueue(FtpSession*)), SLOT(SLOT_ClearQueue(FtpSession*)));
	connect(mp_session_b, SIGNAL(gui_clearqueue(FtpSession*)), SLOT(SLOT_ClearQueue(FtpSession*)));
	
	QAction *deleteShortcut = new QAction( QPixmap(), "&Delete",
                                      Key_Delete, this, "delete" );
									  
	connect(deleteShortcut, SIGNAL(activated()), this, SLOT(SLOT_SkipTasks()));
	
	mp_session_a->Disconnect();
	mp_session_b->Disconnect();

	applyConfig();
}

void Kasablanca::setupGUI()
{
	mp_view->TaskView->setSorting(-1);
	mp_view->BrowserA->setSorting(-1);
	mp_view->BrowserB->setSorting(-1);

	mp_view->TransferButtonA->setIconSet(KGlobal::iconLoader()->loadIconSet("forward",KIcon::Toolbar));
	mp_view->TransferButtonB->setIconSet(KGlobal::iconLoader()->loadIconSet("back",KIcon::Toolbar));
	mp_view->RefreshButtonA->setIconSet(KGlobal::iconLoader()->loadIconSet("reload",KIcon::Toolbar));
	mp_view->RefreshButtonB->setIconSet(KGlobal::iconLoader()->loadIconSet("reload",KIcon::Toolbar));
	mp_view->ConnectButtonA->setIconSet(KGlobal::iconLoader()->loadIconSet("connect_no",KIcon::Toolbar));
	mp_view->ConnectButtonB->setIconSet(KGlobal::iconLoader()->loadIconSet("connect_no",KIcon::Toolbar));
}

void Kasablanca::applyConfig()
{
	FtpSession::filecheck def = FtpSession::off;
	if (!KbConfig::onFileExistsIsEnabled()) def = FtpSession::off;
	else if(KbConfig::onFileExistsOverwrite()) def = FtpSession::clear;
	else if(KbConfig::onFileExistsResume()) def = FtpSession::resume;
	else if(KbConfig::onFileExistsSkip()) def = FtpSession::skip;

	m_skiplist.setPattern(KbConfig::skiplist());
	m_skiplistenabled = KbConfig::skiplistIsEnabled();
	m_onqueuefinishedenabled = KbConfig::onQueueFinishedIsEnabled();
	m_onqueuefinished = KbConfig::onQueueFinished();

	list<FtpSession*>::iterator end_session = mp_sessionlist->end();
	for (list<FtpSession*>::iterator i = mp_sessionlist->begin(); i != end_session; i++)
	{
		(*i)->SetColors(KbConfig::localColor(), KbConfig::successColor(), KbConfig::failureColor(), KbConfig::backgroundColor());
		(*i)->SetOnFileExistsDefault(def);
		(*i)->EnableCmdLine(KbConfig::commandLineIsEnabled());
		(*i)->SetFont(KbConfig::logwindowFont());
		(*i)->FilterHiddenFiles(KbConfig::hideHiddenFilesIsEnabled());
	}

	// make the system tray switch on/off when settings are changed
	if (KbConfig::systrayIsEnabled()) mp_systemtray->show();
	else mp_systemtray->hide();
}

void Kasablanca::saveSettings()
{
	KbConfig::writeConfig();
}

void Kasablanca::setupActions()
{
	KStdAction::quit(kapp, SLOT(quit()), actionCollection());

	//m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
	m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

	KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
	//KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
	KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

	/*
	// this doesn't do anything useful.  it's just here to illustrate
	// how to insert a custom menu and menu item
	KAction *custom = new KAction(i18n("Cus&tom Menuitem"), 0,
											this, SLOT(optionsPreferences()),
											actionCollection(), "custom_action");
	*/

	new KAction(i18n("Edit"), CTRL+Key_E, this, SLOT(SLOT_EditBookmarks()), actionCollection(), "edit bookmarks action");

	new KAction(i18n("Import"), CTRL+Key_I, this, SLOT(SLOT_ImportBookmarks()), actionCollection(), "import bookmarks action");

	createGUI();
}

void Kasablanca::optionsShowToolbar()
{
    if (m_toolbarAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void Kasablanca::optionsShowStatusbar()
{
    if (m_statusbarAction->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void Kasablanca::optionsConfigureKeys()
{
    KKeyDialog::configure(actionCollection(), mp_view);
}

void Kasablanca::optionsConfigureToolbars()
{
    // use the standard toolbar editor
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    saveMainWindowSettings(KGlobal::config());
# endif
#else
    saveMainWindowSettings(KGlobal::config());
#endif
}

void Kasablanca::optionsPreferences()
{
	if (KConfigDialog::showDialog("settings")) return;

	KConfigDialog* dialog = new KConfigDialog(0, "settings", KbConfig::self(), KDialogBase::IconList,
		KDialogBase::Ok | KDialogBase::Apply | KDialogBase::Cancel | KDialogBase::Help);

	dialog->setHelp("globalhelp");

	KasablancaGeneralPreferencesDialog *general = new KasablancaGeneralPreferencesDialog(0, i18n("General"));
	KasablancaColorsPreferencesDialog *colors = new KasablancaColorsPreferencesDialog(0, i18n("Colors"));
	KasablancaUserInterfacePreferencesDialog *ui = new KasablancaUserInterfacePreferencesDialog(0, i18n("User Interface"));
	dialog->addPage(general, i18n("General"), "kasablanca");
	dialog->addPage(colors, i18n("Colors"), "colors");
	dialog->addPage(ui, i18n("User Interface"), "winprops");
	connect(dialog, SIGNAL(settingsChanged()), this, SLOT(applyConfig()));
	dialog->show();
}

void Kasablanca::newToolbarConfig()
{
    // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
    // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
    createGUI();

#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    applyMainWindowSettings(KGlobal::config());
# endif
#else
    applyMainWindowSettings(KGlobal::config());
#endif
}

void Kasablanca::setupMenu()
{
	QWidget *w;
	w = factory()->container("connect menu", this);
	if (w == NULL) qWarning("ERROR: 'connect menu' not found in .ui file");
	else
	{
		QPopupMenu *menu = static_cast<QPopupMenu *>(w);
		menu->insertItem(i18n("Session A"),  &m_bookmarksmenu_a);
		menu->insertItem(i18n("Session B"), &m_bookmarksmenu_b);
	}
	w = factory()->container("action menu", this);
	if (w == NULL) qWarning("ERROR: 'action menu' not found in .ui file");
	else
	{
		QPopupMenu *menu = static_cast<QPopupMenu *>(w);
		menu->insertItem(i18n("Session A"),  &m_rclickmenu_a);
		menu->insertItem(i18n("Session B"), &m_rclickmenu_b);
		menu->insertItem(i18n("Queue"), &m_rclickmenu_t);
	}

	m_rclickmenu_a.insertItem(i18n("Transfer"), Transfer);
	m_rclickmenu_a.insertItem(i18n("Put in queue"), Queue);
	m_rclickmenu_a.insertSeparator();
	m_rclickmenu_a.insertItem(i18n("Delete"), Delete);
	m_rclickmenu_a.insertItem(i18n("Rename"), Rename);
	m_rclickmenu_a.insertItem(i18n("Mkdir"), Mkdir);
	m_rclickmenu_a.insertSeparator();
	m_rclickmenu_a.insertItem(i18n("Bookmark site"), Bookmark);

	m_rclickmenu_b.insertItem(i18n("Transfer"), Transfer);
	m_rclickmenu_b.insertItem(i18n("Put in queue"), Queue);
	m_rclickmenu_b.insertSeparator();
	m_rclickmenu_b.insertItem(i18n("Delete"), Delete);
	m_rclickmenu_b.insertItem(i18n("Rename"), Rename);
	m_rclickmenu_b.insertItem(i18n("Mkdir"), Mkdir);
	m_rclickmenu_b.insertSeparator();
	m_rclickmenu_b.insertItem(i18n("Bookmark site"), Bookmark);

	m_rclickmenu_t.insertItem(i18n("Start Queue"), Start);
	m_rclickmenu_t.insertSeparator();
	m_rclickmenu_t.insertItem(i18n("Skip Item(s)"), Skip);

	connect(&m_rclickmenu_a, SIGNAL(activated(int)), mp_session_a, SLOT(SLOT_ActionMenu(int)));
	connect(&m_bookmarksmenu_a, SIGNAL(activated(int)), mp_session_a, SLOT(SLOT_ConnectMenu(int)));
	connect(mp_view->BrowserA, SIGNAL(doubleClicked(QListViewItem*)), mp_session_a, SLOT(SLOT_ItemClicked(QListViewItem*)));
	connect(mp_view->BrowserA, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
		mp_session_a, SLOT(SLOT_ItemRClicked(QListViewItem *, const QPoint &, int)));
	connect(mp_view->BrowserA->header(), SIGNAL (clicked(int)), mp_session_a, SLOT (SLOT_HeaderClicked(int)));

	connect(&m_rclickmenu_b, SIGNAL(activated(int)), mp_session_b, SLOT(SLOT_ActionMenu(int)));
	connect(&m_bookmarksmenu_b, SIGNAL(activated(int)), mp_session_b, SLOT(SLOT_ConnectMenu(int)));
	connect(mp_view->BrowserB, SIGNAL(doubleClicked(QListViewItem*)), mp_session_b, SLOT(SLOT_ItemClicked(QListViewItem*)));
	connect(mp_view->BrowserB, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
		mp_session_b, SLOT(SLOT_ItemRClicked(QListViewItem *, const QPoint &, int)));
	connect(mp_view->BrowserB->header(), SIGNAL(clicked(int)), mp_session_b, SLOT(SLOT_HeaderClicked(int)));

	connect( mp_view->TaskView, SIGNAL (rightButtonPressed( QListViewItem *, const QPoint &, int )), this,
			SLOT (SLOT_ItemRightClickedT(QListViewItem *, const QPoint &, int )));

	m_rclickmenu_t.connectItem(Start, this, SLOT(SLOT_ProcessQueue()));
	m_rclickmenu_t.connectItem(Skip, this, SLOT(SLOT_SkipTasks()));
}

void Kasablanca::setupStatusBar()
{
    m_iconencrypted = KGlobal::iconLoader()->loadIconSet("encrypted",KIcon::Small).pixmap(QIconSet::Small,QIconSet::Normal);
    m_iconunencrypted = KGlobal::iconLoader()->loadIconSet("encrypted",KIcon::Small).pixmap(QIconSet::Small,QIconSet::Disabled);

    mp_statusline_a = new QLabel("", statusBar());
    mp_encryptionicon_a = new QLabel(statusBar());
    mp_encryptionicon_a->setPixmap(m_iconunencrypted);

    mp_statusline_b = new QLabel("", statusBar());
    mp_encryptionicon_b = new QLabel(statusBar());
    mp_encryptionicon_b->setPixmap(m_iconunencrypted);

    statusBar()->addWidget(mp_statusline_a, 1);
    statusBar()->addWidget(mp_encryptionicon_a, 0);
    statusBar()->addWidget(mp_statusline_b, 1, true);
    statusBar()->addWidget(mp_encryptionicon_b, 0, true);
}

Kasablanca::~Kasablanca()
{
}

void Kasablanca::InitBookmarks()
{
	int count;
	m_bookmarks.clear();
	m_bookmarks = KbSiteInfo::ParseBookmarks();

	m_bookmarksmenu_a.clear();
	m_bookmarksmenu_b.clear();

	m_bookmarksmenu_a.insertItem(i18n("Custom"),0);
	m_bookmarksmenu_b.insertItem(i18n("Custom"),0);
	count = 1;

	m_bookmarksmenu_a.insertSeparator();
	m_bookmarksmenu_b.insertSeparator();

	list<KbSiteInfo>::iterator end_bookmarks = m_bookmarks.end();
	for (list<KbSiteInfo>::iterator i = m_bookmarks.begin(); i != end_bookmarks; i++)
	{
		m_bookmarksmenu_a.insertItem((*i).GetName(),count);
		m_bookmarksmenu_b.insertItem((*i).GetName(),count);
		count++;
	}
}

void Kasablanca::SLOT_QueueItems(KbDirInfo *dir, FtpSession* src, FtpSession* dst, bool startqueue)
{
	QueueItemsRecurse(dir, src, dst);
	if ((mp_view->TaskView->firstChild()) && (startqueue))
	{
		if (!src->Connected()) ProcessQueue(static_cast<KbTransferItem*>(mp_view->TaskView->firstChild()));
		else static_cast<KbTransferItem*>(mp_view->TaskView->firstChild())->Init();
	}
}

void Kasablanca::QueueItemsRecurse(KbDirInfo *dir, FtpSession* src, FtpSession* dst, QListViewItem* parent)
{
	list<KbFileInfo*>* filelist;
	list<KbDirInfo*>* dirlist;
	QListViewItem *after, *lastchild;

	filelist = dir->Filelist();
	dirlist = dir->Dirlist();

	if (KbConfig::prioritylistIsEnabled())
	{
		dirlist->sort(KbDirInfo::PrioritySort);
		filelist->sort(KbFileInfo::PrioritySort);
	}

	lastchild = mp_view->TaskView->LastChild();

	after = NULL;
	list<KbFileInfo*>::iterator end_file = filelist->end();
	for(list<KbFileInfo*>::iterator fileIterator = filelist->begin(); fileIterator != end_file; fileIterator++)
	{
		if ((m_skiplist.search((*fileIterator)->fileName()) < 0) || (!m_skiplistenabled))
		{
			KbFileInfo *srcfi = new KbFileInfo(**fileIterator);
			KbFileInfo *dstfi = new KbFileInfo(**fileIterator);
			srcfi->SetDirPath(src->WorkingDir() + srcfi->dirPath());
			dstfi->SetDirPath(dst->WorkingDir() + dstfi->dirPath());
			if (parent) after = new KbTransferFile(parent, after, src, dst, srcfi, dstfi);
			else new KbTransferFile(mp_view->TaskView, lastchild /*mp_view->TaskView->lastItem()*/, src, dst, srcfi, dstfi);
		}
		else qWarning("INFO: entry ignored due to matched skiplist regexp");
	}

	after = NULL;
	list<KbDirInfo*>::iterator end_dir = dirlist->end();
	for(list<KbDirInfo*>::iterator dirIterator = dirlist->begin(); dirIterator != end_dir; dirIterator++)
	{
		if ((m_skiplist.search((*dirIterator)->fileName()) < 0) || (!m_skiplistenabled))
		{
			KbFileInfo *srcfi = new KbFileInfo(*(*dirIterator));
			KbFileInfo *dstfi = new KbFileInfo(*(*dirIterator));
			srcfi->SetDirPath(src->WorkingDir() + srcfi->dirPath());
			dstfi->SetDirPath(dst->WorkingDir() + dstfi->dirPath());
			if (parent) after = new KbTransferDir(parent, after, src, dst, srcfi, dstfi);
			else after = new KbTransferDir(mp_view->TaskView, lastchild /*mp_view->TaskView->lastItem()*/, src, dst, srcfi, dstfi);
			QueueItemsRecurse(*dirIterator, src, dst, after);
		}
		else qWarning("INFO: entry ignored due to matched skiplist regexp");
	}
	delete dir;
}

void Kasablanca::SLOT_EditBookmarks()
{
    BookmarkDialog dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        InitBookmarks();
    }
}

void Kasablanca::SLOT_ImportBookmarks()
{
	ImportDialog dlg;
	dlg.exec();

	int cnt=0;
	list<KbSiteInfo> newbookmarklist;

	list<KbSiteInfo>::iterator end_session = dlg.Bookmarks()->end();
	for (list<KbSiteInfo>::iterator i = dlg.Bookmarks()->begin(); i != end_session; i++)
	{
		m_bookmarks.push_back( *i );
		cnt++;
	}
	KbSiteInfo::WriteBookmarks(m_bookmarks);
}

void Kasablanca::SLOT_ClearQueue(FtpSession*)
{
	mp_view->TaskView->clear();
}

void Kasablanca::SLOT_ItemRightClickedT(QListViewItem *, const QPoint & point, int)
{
	m_rclickmenu_t.exec(point);
}

void Kasablanca::SLOT_SkipTasks()
{
    QListViewItemIterator it(mp_view->TaskView);
    while ( it.current() )
    {
        if (it.current()->isSelected()) delete it.current();
        else ++it;
    }
}

void Kasablanca::SLOT_SelectionChanged()
{
	int counter_a = 0;
	int counter_b = 0;
	bool flag;

	QListViewItemIterator ita(mp_view->BrowserA);
 	while ( ita.current() )
 	{
   	if (ita.current()->isSelected()) counter_a++;
   	++ita;
 	}

	flag = ((counter_a) && (!mp_session_a->Occupied()) && (!mp_session_b->Occupied()));

	m_rclickmenu_a.setItemEnabled(Transfer, flag);
	m_rclickmenu_a.setItemEnabled(Queue, flag);
	mp_view->TransferButtonA->setEnabled(flag);

	/* when at least one item is selected, Delete and Rename are
	enabled. */

	m_rclickmenu_a.setItemEnabled(Rename, (counter_a >= 1));
	m_rclickmenu_a.setItemEnabled(Delete, (counter_a >= 1));

	QListViewItemIterator itb(mp_view->BrowserB);
 	while ( itb.current() )
 	{
   	if (itb.current()->isSelected()) counter_b++;
   	++itb;
 	}

	flag = ((counter_b) && (!mp_session_a->Occupied()) && (!mp_session_b->Occupied()));

	m_rclickmenu_b.setItemEnabled(Transfer, flag);
	m_rclickmenu_b.setItemEnabled(Queue, flag);
	mp_view->TransferButtonB->setEnabled(flag);

	/* when at least one item is selected, Delete and Rename are
	enabled. */

	m_rclickmenu_b.setItemEnabled(Rename, (counter_b >= 1));
	m_rclickmenu_b.setItemEnabled(Delete, (counter_b >= 1));
}

void Kasablanca::SLOT_NextTransfer(QListViewItem* item)
{	
	QListViewItem *next;
	next = NextTransfer(item);
	if (next) ProcessQueue(static_cast<KbTransferItem*>(next));
	else QueueFinished();
}

void Kasablanca::QueueFinished()
{
	if (m_onqueuefinishedenabled)
	{
		KProcess* p = new KProcess();
		*p << QStringList::split(" ", m_onqueuefinished);
		connect(p, SIGNAL(processExited(KProcess*)), SLOT(SLOT_LocalProcessExited(KProcess*)));
		p->start();
	}
	if (KbConfig::systrayIsEnabled()) KPassivePopup::message("Transfer is finished.", mp_systemtray);
	//QToolTip::remove(mp_systemtray);
}

QListViewItem* Kasablanca::NextTransfer(QListViewItem* item)
{
	FtpSession *src, *dst;
	QListViewItem *parent, *next, *retval;
	src = static_cast<KbTransferItem*>(item)->SrcSession();
	dst = static_cast<KbTransferItem*>(item)->DstSession();
	next = item->nextSibling();
	parent = item->parent();
	retval = NULL;
	if (item->childCount() == 0) // if the item contains no subelements - delete it, if it doesn't - start with the next
	{
		static_cast<KbTransferItem*>(item)->Finish();
		if (mp_view->TaskView->childCount() > 0) delete item;
		if (next) retval = next;
		else if (parent) retval = NextTransfer(parent);
		else
		{
			src->SLOT_RefreshButton();
			dst->SLOT_RefreshButton();
		}
	}
	else retval = item->firstChild();

	return retval;
}

void Kasablanca::SLOT_ProcessQueue()
{
	KbTransferItem *ti = static_cast<KbTransferItem*>(mp_view->TaskView->firstChild());
	if (ti) ProcessQueue(ti);
}

void Kasablanca::ProcessQueue(KbTransferItem* item)
{
	FtpSession *srcsession = item->SrcSession();
	srcsession->Transfer(item);
}

