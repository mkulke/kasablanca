//
// C++ Interface: ftpsession
//
// Description: 
//
//
// Author: Magnus Kulke <sikor_sxe@radicalapproach.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FTPSESSION_H
#define FTPSESSION_H

#include <kprocess.h>
#include <qstring.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qobject.h>
#include <qcolor.h>
#include <list>

#include "kbfileinfo.h"

using namespace std;

class FtpThread;
class EventHandler;
class QMutex;
class QTextEdit;
class QToolButton;
class QListView;
class KbSiteInfo;
class QPopupMenu;
class KLineEdit;
class QLabel;
class QPoint;
class KbDirInfo;
class QHeader;
class KbTransferItem;

typedef pair<QString, bool> logentries;

/**
@author Magnus Kulke
*/
class FtpSession : public QObject
{
Q_OBJECT
public:
	enum filecheck
	{
		off = 0,
		skip,
		clear,
		resume
	};
public:
	FtpSession(QObject *parent = 0, const char *name = 0);
	~FtpSession();
	void SetLogWindow(QTextEdit* logwindow) { mp_logwindow = logwindow; };
	void SetRefreshButton(QToolButton* refreshbutton) { mp_refreshbutton = refreshbutton; };
	void SetConnectButton(QToolButton* connectbutton) { mp_connectbutton = connectbutton; };
	void SetTransferButton(QToolButton* transferbutton) { mp_transferbutton = transferbutton; }
	void SetBrowser(QListView* browser) { mp_browser = browser; };
	void SetStatusLine(QLabel* statusline) { mp_statusline = statusline; };
	void SetEncryptionIcon(QLabel* encryptionicon) { mp_encryptionicon = encryptionicon; };
	void SetBookmarksMenu(QPopupMenu *bookmarksmenu) { mp_bookmarksmenu = bookmarksmenu; };
	void SetRclickMenu(QPopupMenu *rclickmenu) { mp_rclickmenu = rclickmenu; };
	void SetSessionList(list<FtpSession*> *sessionlist) { mp_sessionlist = sessionlist; };
	void SetCurrentTransfer(KbTransferItem* currenttransfer) { mp_currenttransfer = currenttransfer; };
	bool Connected() { return m_connected; };
	void Disconnect();
	void Connect();
	bool Occupied() { return m_occupied; };
	void Occupy();
	void Free();
	QString WorkingDir();
 	void Transfer(KbTransferItem *item);
	int CheckFile(KbTransferItem *item);
	void Abort();
	FtpThread* Ftp() { return mp_ftpthread; };
	KbSiteInfo* SiteInfo() { return mp_siteinfo; };
	void SetColors(QColor local, QColor success, QColor failure, QColor background);
	void SetFont(QFont font);
	void SetOnFileExistsDefault(filecheck onfileexistsdefault) {m_onfileexistsdefault = onfileexistsdefault; };
	void EnableCmdLine(bool b);
	void FilterHiddenFiles(bool b);
	void SetCmdLine(KLineEdit* cmdline);
	void SetCwdLine(KLineEdit* cwdline);
private:
	FtpThread *mp_ftpthread;
	EventHandler *mp_eventhandler;
	KbSiteInfo *mp_siteinfo;
	QTextEdit *mp_logwindow;
	QToolButton *mp_connectbutton, *mp_refreshbutton, *mp_transferbutton;
	KLineEdit *mp_cwdline, *mp_cmdline;
	QPopupMenu* mp_bookmarksmenu, *mp_rclickmenu;
	QListView *mp_browser;
	QLabel *mp_statusline, *mp_encryptionicon;
	QString m_remoteworkingdir; 
	QDir m_localworkingdir;
	QColor m_colorsuccess, m_colorfailure, m_colorlocal;
	QFont m_logwindowfont;
	int m_sortpref;
	bool m_connected, m_occupied, m_sortascending, m_startqueue;
	list<logentries> m_loglist;
	QPixmap m_iconencrypted, m_iconunencrypted;
	QHeader *mp_header;
	list<FtpSession*> *mp_sessionlist;
	KbTransferItem *mp_currenttransfer;
	filecheck m_onfileexistsdefault;
	bool m_encrypted;
	
public slots:
	void SLOT_Log(QString log, bool out);
	void SLOT_Xfered(off64_t xfered, bool encrypted);
	void SLOT_ActionMenu(int i);
	void SLOT_ConnectMenu(int i);
	void SLOT_HeaderClicked(int section);
	void SLOT_LocalProcessExited(KProcess* proc) { delete proc; };
	void SLOT_ItemClicked(QListViewItem*);
	void SLOT_ItemRClicked(QListViewItem * item, const QPoint & point, int col);
	void SLOT_Connect(bool success);
	void SLOT_Login(bool success);
	void SLOT_Pwd(bool success, QString pwd);
	void SLOT_Misc(bool success);
	void SLOT_AuthTls(bool success);
	void SLOT_EncryptData(bool success);
	void SLOT_Dir(bool success, list<KbFileInfo*> dirlist, list<KbFileInfo*> filelist);
	void SLOT_ConnectButton();
	void SLOT_RefreshButton();
	void SLOT_TransferButton();
	void SLOT_CwdLine();
	void SLOT_CmdLine();
	void SLOT_Finish();
	void SLOT_ConnectionLost();
	void SLOT_Transfer(bool success);
	void SLOT_Scandir(bool success, KbDirInfo* dir);
	
private:
	void PrintLog(bool success);
	void QueueItems();
	void RefreshBrowser();
	void UpdateLocal(QString cwd = "");
	//bool RmdirLocal(QString dir);
	void GetFile(KbTransferItem *item, filecheck fc);
	void PutFile(KbTransferItem *item, filecheck fc);
	void FxpFile(KbTransferItem *item, filecheck fc);
	void ChangeDirectory(QString path);
	bool CheckLocalDirectory(QString path);
	bool MakeLocalDirectory(QString path);
	bool CopyLocalFile(KbTransferItem* item);
	bool ScandirLocal(KbDirInfo *dir, QString path);
	void SortItems();
	void MakeDirectory(QString dir);
	void timerEvent(QTimerEvent*);
signals:
	void gui_update();
	void gui_queueitems(KbDirInfo* dir, FtpSession* src, FtpSession* dst, bool startqueue);
	void gui_succeedtransfer(QListViewItem* item);
	void gui_clearqueue(FtpSession* session);
};

#endif
