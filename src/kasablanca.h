/***************************************************************************
                          kasablanca.h  -  description
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

#ifndef KASABLANCA_H
#define KASABLANCA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

class KbDir;
class KbFile;
class KToggleAction;
class KSystemTray;
class QWidget;
class QCloseEvent;
class QLabel;
class QPixmap;
class FtpSession;
class KbStatusTip;

#include <kapplication.h>
#include <klistview.h>
#include <kmainwindow.h>
#include <kprocess.h>

#include <qdir.h>
#include <qlistview.h>
#include <qmenubar.h>
#include <qregexp.h>

#include <list>

#include "kbsiteinfo.h"
#include "kbtransferitem.h"
#include "Q_mainwindow.h"

using namespace std;

/** Kasablanca is the base class of the project */

class Kasablanca : public KMainWindow
{
    Q_OBJECT
public:
	enum RightClickMenu
	{
		Transfer = 1001,
		Queue,
		Delete,
		Rename,
		Mkdir,
		Start,
		Skip,
		Bookmark
	};

	Kasablanca();
	~Kasablanca();

	QString m_tempdirname;
	QString m_version;
	list<KbSiteInfo> m_bookmarks;
	KSystemTray* systemTray() const { return mp_systemtray; }
	KbStatusTip* statusTip() const { return mp_statustip; }
	void InitBookmarks();

public slots:
	void SLOT_EditBookmarks();
	void SLOT_ImportBookmarks();
	void SLOT_ItemRightClickedT(QListViewItem * item, const QPoint & point, int col );
	void SLOT_SelectionChanged();
	void SLOT_SkipTasks();
	void SLOT_ProcessQueue();
	void SLOT_QueueItems(KbDirInfo *dir, FtpSession* src, FtpSession* dst, bool startqueue);
	void SLOT_NextTransfer(QListViewItem* item);
	void SLOT_ClearQueue(FtpSession* session);
	void SLOT_LocalProcessExited(KProcess* proc) { delete proc; };

private slots:
	void optionsShowToolbar();
	void optionsShowStatusbar();
	void optionsConfigureKeys();
	void optionsConfigureToolbars();
	void optionsPreferences();
	void newToolbarConfig();
	void applyConfig();

private:
	void QueueItemsRecurse(KbDirInfo *dir, FtpSession* src, FtpSession* dst, QListViewItem* parent = NULL);
	void setupStatusBar();
	void setupMenu();
	void setupGUI();
	void setupActions();
	void saveSettings();
	void ProcessQueue(KbTransferItem* item);
	QListViewItem* NextTransfer(QListViewItem* item);
	void QueueFinished();

	KasablancaMainWindow *mp_view;
	QPopupMenu m_bookmarksmenu_a, m_bookmarksmenu_b;
	QRegExp m_skiplist;
	QPopupMenu m_rclickmenu_a, m_rclickmenu_b, m_rclickmenu_t;
	QLabel *mp_statusline_a, *mp_statusline_b, *mp_encryptionicon_a, *mp_encryptionicon_b;
	QPixmap m_iconencrypted, m_iconunencrypted;
	KToggleAction *m_toolbarAction;
	KToggleAction *m_statusbarAction;
	FtpSession *mp_session_a, *mp_session_b;
	list<FtpSession*> *mp_sessionlist;
	bool m_skiplistenabled, m_onqueuefinishedenabled;
	QString m_onqueuefinished;
	KSystemTray* mp_systemtray;
	KbStatusTip *mp_statustip;
};

#endif
