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
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <qobject.h>
#include <qvaluelist.h>
#include <list>
#include "kbfileinfo.h"

using namespace std;

class KbDirInfo;
class FtpThread;

/**
@author Magnus Kulke
*/

typedef list<KbFileInfo*> filist;
typedef pair<filist, filist> contentpair;

class EventHandler : public QObject
{
Q_OBJECT
public:
    EventHandler(QObject *parent = 0, const char *name = 0);
    ~EventHandler();
	 void SetFtpThread(FtpThread* thread);
public:
	enum EventType
	{
		error = 1001,
		outlog,
		inlog,
		xfered,
		finished,
		connectionlost,
		misc_success,
		misc_failure,		
		connect_success,
		connect_failure,
		login_success,
		login_failure,
		pwd_success,
		pwd_failure,
		chdir_success,
		chdir_failure,
		dir_success,
		dir_failure,
		scandir_success,
		scandir_failure,
		rm_success,
		rm_failure,
		rmdir_success,
		rmdir_failure,
		quit_success,
		quit_failure,
		authtls_success,
		authtls_failure,
		encryptdata_success,
		encryptdata_failure,
		get_success,
		get_failure,
		mkdir_success,
		mkdir_failure,
		rename_success,
		rename_failure,
		raw_success,
		raw_failure,
		put_success,
		put_failure,
		fxp_success,
		fxp_failure,
		transfer_success,
		transfer_failure
	};
private:
	bool eventFilter(QObject* o, QEvent *e);
private:
	FtpThread* mp_thread;
signals:
	void ftp_error(QString error);
	void ftp_log(QString log, bool out);
	void ftp_xfered(off64_t xfered, bool encrypted);
	void ftp_misc(bool success);
	void ftp_finished();
	void ftp_connect(bool success);
	void ftp_login(bool success);
	void ftp_quit(bool success);
	void ftp_pwd(bool success, QString path);
	void ftp_chdir(bool success);
	void ftp_dir(bool success, list<KbFileInfo*> dirlist, list<KbFileInfo*> filelist);
	void ftp_scandir(bool success, KbDirInfo* dir);
	void ftp_rm(bool success);
	void ftp_rmdir(bool success);
	void ftp_authtls(bool success);
	void ftp_encryptdata(bool success);
	void ftp_get(bool success);
	void ftp_mkdir(bool success);
	void ftp_rename(bool success);
	void ftp_raw(bool success);
	void ftp_put(bool success);
	void ftp_transfer(bool success);
	void ftp_fxp(bool success);
	void ftp_connectionlost();
};

#endif
