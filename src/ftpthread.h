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
#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#define KB_THREAD_TIMEOUT 1000

#include <list>
#include <qthread.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qvaluevector.h>
#include "eventhandler.h"
#include "kbfileinfo.h"

using namespace std;

class ftplib;
class QObject;
class KbDirInfo;

typedef list<KbFileInfo*> filist;
typedef pair<off64_t, bool> xferpair;
typedef pair<filist, filist> dirpair;

/**
@author Magnus Kulke
*/
class FtpThread : public QThread 
{
public:
   FtpThread();
   ~FtpThread();
	static void CallbackLog(char *log, void *arg, bool out);
	static int CallbackXfer(off64_t xfered, void *arg);
	void SetEventReceiver(QObject* eventreceiver);
	void ClearQueue();
	bool Connect(QString host);
	bool Login(QString user, QString pass);
	bool Quit();
	bool Pwd();
	bool Chdir(QString path);
	bool Cdup();
	bool Dir(bool force = false);
	bool Scandir(KbDirInfo* dir);
	bool Rm(QString name);
	bool Rmdir(QString name);
	bool Authtls();
	bool Pasv(bool flag);
	bool EncryptData(bool flag, bool force = false);
	bool Transfer_Fxp(QString src, QString dst, FtpThread* dstftp, int srctls, int dsttls, off64_t resume = 0, int alt = 0);
	bool Mkdir(QString path);
	bool Rename(QString src, QString dst);
	bool Raw(QString cmd);
	bool Transfer_Get(QString src, QString dst, int tls, off64_t resume = 0);
	bool Transfer_Put(QString src, QString dst, int tls, off64_t resume = 0);
	bool Transfer_Changedir(QString dir, int tls);
	bool Transfer_Mkdir(QString dir);
	void Event(EventHandler::EventType type, void *data = NULL);
	ftplib* Ftp() { return mp_ftp; };
	void FxpReportResult(bool result);
	bool FxpDisableTls();
private:
	enum task
	{
		connect = 0,
		negotiateencryption,
		login,
		quit,
		pwd,
		chdir,
		cdup,
		dir,
		scandir,
		rm,
		rmdir,
		authtls,
		dataencryption,
		mkdir,
		rename,
		raw,
		transfer_changedir,
		transfer_get,
		transfer_mkdir,
		transfer_put,
		transfer_fxp
	};	
	void run();
	bool FormatFilelist(const char *filename,
		QString current,
		filist *filetable,
		filist *dirtable
	);
	void InitInternals();
	void Connect_thread(); 
	void Login_thread(); 
	void Pwd_thread();
	void Quit_thread();
	void Chdir_thread();
	void Cdup_thread();
	void Dir_thread();
	void Scandir_thread();
	void Delete_thread();
	void Rm_thread();
	void Rmdir_thread();
	void Authtls_thread();
	void Dataencryption_thread();
	void Mkdir_thread();
	void Rename_thread();
	void Raw_thread();
	void Transfer_Changedir_thread();
	void Transfer_Get_thread();
	void Transfer_Put_thread();
	void Transfer_Fxp_thread();
	void Transfer_Mkdir_thread();
	bool Scandir_recurse(KbDirInfo *dir, QString path);
	bool Delete_recurse(QString name);
	bool ConnectionLost();
private:
	QMutex* mp_mutex;
	QObject* mp_eventreceiver;
	ftplib* mp_ftp;
	QString m_pwd;
	bool m_dataencrypted;
	KbDirInfo* mp_scandir;
	filist m_dirlist, m_filelist;
	dirpair m_dircontent;
	QValueList<task> m_tasklist;
	QStringList m_stringlist;
	QValueList<int> m_intlist;
	QValueList<off64_t> m_ulonglist;
	QValueList<FtpThread*> m_ftplist;
	QValueVector<dirpair> m_cache_vector;
	QStringList m_cache_list;
public:
	QString m_linebuffer;
};

#endif
