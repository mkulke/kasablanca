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

// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 
 
#include <kstandarddirs.h>

#include <qapplication.h>
#include <qevent.h>
#include <qdir.h>

#include <iostream>
#include <list>

#include "kbconfig.h"
#include "ftplib.h"
#include "kbdirinfo.h"
#include "eventhandler.h"
#include "kbfileinfo.h"
#include "ftpthread.h"

using namespace std;

/* 
class description:

the class accepts ftp operations by public methods. when such a method is called,
a certain type value is appended to a list. in the threads main loop (triggered when 
start() is called) the list is processed by calling the ftplibpp functions. depending
on the result of an operation a certain event is posted to the eventreceiver. when
there is a value to be returned, it's named out_. 
*/


FtpThread::FtpThread() : QThread()
{
	mp_eventreceiver = NULL;
	mp_ftp = new ftplib();
	
	mp_ftp->SetCallbackArg(this);
	mp_ftp->SetCallbackLogFunction(FtpThread::CallbackLog);
	mp_ftp->SetCallbackBytes(1024);
	mp_ftp->SetCallbackXferFunction(FtpThread::CallbackXfer);
}

FtpThread::~FtpThread()
{
	delete mp_ftp;
}

/* init the variables the ftp session needs */

void FtpThread::InitInternals()
{
	m_stringlist.clear();
	m_intlist.clear();
	m_ulonglist.clear();
	m_pwd = "";
	m_dataencrypted = false;
	m_cache_vector.clear();
	m_cache_list.clear();
}

/* callback function for the transfer */

int FtpThread::CallbackXfer(off64_t xfered, void *arg)
{
	FtpThread* ftp = static_cast<FtpThread*>(arg);
		
	ftp->Event(EventHandler::xfered, new xferpair(xfered, ftp->m_dataencrypted));
	return 1;
}

/* callback function for the logs */

void FtpThread::CallbackLog(char *log, void *arg, bool out)
{
	/* the incoming log message isn't formatted at all. data
	can be "end.\r\n230 ne" for example. the log gets chopped 
	into lines and for every line an events is posted. for 
	in-logs there's no need for this procedure, as the logs
	always arrive in line-format. */ 

	FtpThread* ftp = static_cast<FtpThread*>(arg);	
		
	if (out)
	{	
		int pos;
		QString buffer = ftp->m_linebuffer + log;
		while ((pos = buffer.find('\n')) != -1)
		{
			ftp->Event(EventHandler::outlog, new QString(buffer.left(pos + 1)));
			buffer.remove(0, pos + 1);
		}
		ftp->m_linebuffer = buffer;
	}
	else ftp->Event(EventHandler::inlog, new QString(log));
}

/* set the receiver for the events the thread posts when a certain ftp operation is done */

void FtpThread::SetEventReceiver(QObject* eventreceiver)
{
	mp_eventreceiver = eventreceiver;
}

/* connect to host */

bool FtpThread::Connect(QString host)
{
	InitInternals();

	if (running()) return false;
	else
	{	
		m_stringlist.append(host);
		m_tasklist.append(FtpThread::connect);
		return true;
	}
}

/* login with user and pass */

bool FtpThread::Login(QString user, QString pass)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(user);
		m_stringlist.append(pass);
		m_tasklist.append(FtpThread::login);
		return true;
	}
}

/* quit the ftp session*/

bool FtpThread::Quit()
{
	if (running()) return false;
	else
	{
		m_tasklist.append(FtpThread::quit);
		return true;
	}
}

/* get file */

bool FtpThread::Transfer_Get(QString src, QString dst, int tls, off64_t resume)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(src);
		m_stringlist.append(dst);
		m_intlist.append(tls);
		m_ulonglist.append(resume);
		m_tasklist.append(FtpThread::transfer_get);
		return true;
	}
}

/* fxp file */

bool FtpThread::Transfer_Fxp(QString src, QString dst, FtpThread *dstftp, int srctls, int dsttls, off64_t resume, int alt)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(src);
		m_stringlist.append(dst);
		m_ftplist.append(dstftp);
		m_intlist.append(srctls);
		m_intlist.append(dsttls);
		m_ulonglist.append(resume);
		m_intlist.append(alt);	
		m_tasklist.append(FtpThread::transfer_fxp);
		return true;
	}
}

/* put file */

bool FtpThread::Transfer_Put(QString src, QString dst, int tls, off64_t resume)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(src);
		m_stringlist.append(dst);
		m_intlist.append(tls);
		m_ulonglist.append(resume);
		m_tasklist.append(FtpThread::transfer_put);
		return true;
	}
}

/* transfer mkdir */

bool FtpThread::Transfer_Mkdir(QString dir)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(dir);
		m_tasklist.append(FtpThread::transfer_mkdir);
		return true;
	}
}

/* rename file */

bool FtpThread::Rename(QString src, QString dst)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(src);
		m_stringlist.append(dst);
		m_tasklist.append(FtpThread::rename);
		return true;
	}
}

/* set wether the data channel is encrypted or not */

bool FtpThread::Pasv(bool flag)
{
	if (running()) return false;
	else
	{
		if (flag) mp_ftp->SetConnmode(ftplib::pasv);
		else mp_ftp->SetConnmode(ftplib::port);
		return true;
	}
}

/* set wether the data channel is encrypted or not */

bool FtpThread::EncryptData(bool flag, bool force)
{
	if (running()) return false;
	else
	{	
		m_intlist.append(flag);
		m_intlist.append(force);
		m_tasklist.append(FtpThread::dataencryption);
		return true;
	}
}

/* retrieve the current workind dir */

bool FtpThread::Pwd()
{
	if (running()) return false;
	else
	{
		m_tasklist.append(FtpThread::pwd);
		return true;
	}
}

/* change working dir to the given path */

bool FtpThread::Chdir(QString path)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(path);
		m_tasklist.append(FtpThread::chdir);
		return true;
	}
}

/* change working dir to the given path, used by transfers */

bool FtpThread::Transfer_Changedir(QString dir, int tls)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(dir);
		m_intlist.append(tls);
		m_tasklist.append(FtpThread::transfer_changedir);
		return true;
	}
}

/* creates a directory */

bool FtpThread::Mkdir(QString path)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(path);
		m_tasklist.append(FtpThread::mkdir);
		return true;
	}
}

/* step above in the working dir */

bool FtpThread::Cdup()
{
	if (running()) return false;
	else
	{
		m_tasklist.append(FtpThread::cdup);
		return true;
	}
}

/* retrieve dir contents */

bool FtpThread::Dir(bool force)
{
	if (running()) return false;
	else
	{
		m_intlist.append(force);
		m_tasklist.append(FtpThread::dir);
		return true;
	}
}

/* attempt negotiating an encrypted session */

bool FtpThread::Authtls()
{
	if (running()) return false;
	else
	{
		m_tasklist.append(FtpThread::authtls);
		return true;
	}
}

/* delete a file  */

bool FtpThread::Rm(QString name)
{
	wait(KB_THREAD_TIMEOUT);
	if (running()) return false;
	else
	{
		m_stringlist.append(name);
		m_tasklist.append(FtpThread::rm);
		return true;
	}
}

/* delete a directory  */

bool FtpThread::Rmdir(QString name)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(name);
		m_tasklist.append(FtpThread::rmdir);
		return true;
	}
}
	
/* issue raw command  */

bool FtpThread::Raw(QString cmd)
{
	if (running()) return false;
	else
	{
		m_stringlist.append(cmd);
		m_tasklist.append(FtpThread::raw);
		return true;
	}
}

/* scan the directory recursively */

bool FtpThread::Scandir(KbDirInfo* dir)
{
	if (running()) return false;
	else
	{
		mp_scandir = dir;
		m_tasklist.append(FtpThread::scandir);
		return true;
	}
}
/* the thread methods */

void FtpThread::Connect_thread()
{
	int result;
	QString host = m_stringlist.front();
	m_stringlist.pop_front();

	result = mp_ftp->Connect(host.latin1());

	//unsigned long int xxx = mp_ftp->mp_netbuf->ipappr;
	//qWarning("INFO %lu.%lu.%lu.%lu", xxx & 0xff, (xxx >> 8) & 0xff, (xxx >> 16) & 0xff, (xxx >> 24) & 0xff); 
	//qWarning("INFO: ip:%lu", mp_ftp->mp_netbuf->ipappr);
	
	if (result) Event(EventHandler::connect_success);
	else 
	{
		ClearQueue();
		Event(EventHandler::connect_failure);
	}
}

void FtpThread::Authtls_thread()
{
	int result;
	
	result = mp_ftp->NegotiateEncryption();

	if (result) Event(EventHandler::authtls_success);
	else 
	{
		ClearQueue();
		Event(EventHandler::authtls_failure);
	}
}

void FtpThread::Login_thread()
{
	int result;
	QString user = m_stringlist.front();
	m_stringlist.pop_front();
	QString pass = m_stringlist.front();
	m_stringlist.pop_front();

	result = mp_ftp->Login(user.latin1(), pass.latin1());
	
	if (result) Event(EventHandler::login_success);
	else
	{
		ClearQueue();
		Event(EventHandler::login_failure);
	}
}

void FtpThread::Dataencryption_thread()
{
	int result, flag, force, cacheindex;

	flag = m_intlist.front();
	m_intlist.pop_front();
	force = m_intlist.front();
	m_intlist.pop_front();
	
	cacheindex = -1;
	
	if (KbConfig::dirCachingIsEnabled())
	{
		cacheindex = m_cache_list.findIndex(m_pwd);
	
		if ((cacheindex != -1) && (!force))
		{
			Event(EventHandler::encryptdata_success);
			return;
		}
	}
	
	if (flag) result = mp_ftp->SetDataEncryption(ftplib::secure);
	else result = mp_ftp->SetDataEncryption(ftplib::unencrypted);
	
	if (result) 
	{
		Event(EventHandler::encryptdata_success);
		m_dataencrypted = flag;
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::encryptdata_failure);
	}	
}

void FtpThread::Transfer_Changedir_thread()
{
	int result;
	char buffer[1024];
	QString dirname;
	
	dirname = locateLocal("appdata", QString::number(rand()) + ".dir");
				
	QString path = m_stringlist.front();
	m_stringlist.pop_front();
	int tls = m_intlist.front();
	m_intlist.pop_front();
	
	if (path == m_pwd)
	{
		Event(EventHandler::transfer_success);
		return;	
	}
	
	result = mp_ftp->Chdir(path.latin1());
	if (result) Event(EventHandler::chdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::chdir_failure);	
		Event(EventHandler::transfer_failure);
		return;
	}

	result = mp_ftp->Pwd(buffer, 1024);
	if (result) 
	{
		Event(EventHandler::pwd_success, new QString(buffer));
		m_pwd = buffer;
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::pwd_failure);
		Event(EventHandler::transfer_failure);
		return;
	}
	
	if (tls)
	{
		result = mp_ftp->SetDataEncryption(ftplib::secure);
		if (result) 
		{
			Event(EventHandler::encryptdata_success);
			m_dataencrypted = true;
		}
		else 
		{
			if (ConnectionLost()) Event(EventHandler::connectionlost);
			else Event(EventHandler::encryptdata_failure);
			Event(EventHandler::transfer_failure);
			return;
		}
	}
		
	result = mp_ftp->Dir(dirname.latin1(), "");
	if (result) 
	{		
		m_dirlist.clear();
		m_filelist.clear();		
		FormatFilelist(dirname.latin1(), m_pwd, &m_dirlist, &m_filelist); 
		m_dircontent.first = m_dirlist;
		m_dircontent.second = m_filelist;
		Event(EventHandler::dir_success, &m_dircontent);
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::dir_failure, &m_dircontent);
	}
	
	if (!result) Event(EventHandler::transfer_failure);
	else Event(EventHandler::transfer_success);
	
	QFile::remove(dirname);	
}

void FtpThread::Transfer_Mkdir_thread()
{
	int result;
	QString dir = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Mkdir(dir.latin1());
	if (result) Event(EventHandler::mkdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::mkdir_failure);	
		Event(EventHandler::transfer_failure);
		return;
	}
	
	Event(EventHandler::transfer_success);
}

void FtpThread::Transfer_Get_thread()
{
	int result;
	
	QString src = m_stringlist.front();
	m_stringlist.pop_front();
	QString dst = m_stringlist.front();
	m_stringlist.pop_front();
	int tls = m_intlist.front();
	m_intlist.pop_front();
	off64_t resume = m_ulonglist.front();
	m_ulonglist.pop_front();
		
	if (tls > 1)
	{
		if (tls == 2) result = mp_ftp->SetDataEncryption(ftplib::unencrypted);
		else result = mp_ftp->SetDataEncryption(ftplib::secure);
		if (result) 
		{
			Event(EventHandler::encryptdata_success);
			m_dataencrypted = (tls > 2);
		}
		else 
		{
			if (ConnectionLost()) Event(EventHandler::connectionlost);
			else Event(EventHandler::encryptdata_failure);
			Event(EventHandler::transfer_failure);
			return;
		}
	}
	
	if (resume == 0) result = mp_ftp->Get(dst.latin1(), src.latin1(), ftplib::image);
	else result = mp_ftp->Get(dst.latin1(), src.latin1(), ftplib::image, resume);
	
	if (result) Event(EventHandler::get_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::get_failure);
		Event(EventHandler::transfer_failure);
		return;
	}
	
	Event(EventHandler::transfer_success);
}

void FtpThread::Transfer_Put_thread()
{
	int result;
	
	QString src = m_stringlist.front();
	m_stringlist.pop_front();
	QString dst = m_stringlist.front();
	m_stringlist.pop_front();
	int tls = m_intlist.front();
	m_intlist.pop_front();
	off64_t resume = m_ulonglist.front();
	m_ulonglist.pop_front();
	
	if (tls > 1)
	{
		if (tls == 2) result = mp_ftp->SetDataEncryption(ftplib::unencrypted);
		else result = mp_ftp->SetDataEncryption(ftplib::secure);
		if (result) 
		{
			Event(EventHandler::encryptdata_success);
			m_dataencrypted = (tls > 2);
		}
		else 
		{		
			if (ConnectionLost()) Event(EventHandler::connectionlost);
			else Event(EventHandler::encryptdata_failure);
			Event(EventHandler::transfer_failure);
			return;
		}
	}
	
	if (resume == 0) result = mp_ftp->Put(src.latin1(), dst.latin1(), ftplib::image);
	else result = mp_ftp->Put(src.latin1(), dst.latin1(), ftplib::image, resume);

	if (result) Event(EventHandler::put_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::put_failure);
		Event(EventHandler::transfer_failure);
		return;
	}
	
	Event(EventHandler::transfer_success);
}

void FtpThread::Quit_thread()
{
	int result;
	
	result = mp_ftp->Quit();
	
	if (result) Event(EventHandler::quit_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::quit_failure);
	}
}

void FtpThread::Transfer_Fxp_thread()
{
	int result = 0;
	ftplib::ftp method;
	
	QString src = m_stringlist.front();
	m_stringlist.pop_front();
	QString dst = m_stringlist.front();
	m_stringlist.pop_front();
	FtpThread* dstftp = m_ftplist.front();
	m_ftplist.pop_front();
	int srctls = m_intlist.front();
	m_intlist.pop_front();
	int dsttls = m_intlist.front();
	m_intlist.pop_front();
	off64_t resume = m_ulonglist.front();
	m_ulonglist.pop_front();
	int alt = m_intlist.front();
	m_intlist.pop_front();
	
	if (srctls > 1)
	{
		if (!FxpDisableTls())
		{
			Event(EventHandler::transfer_failure);
			return;
		}
	}
	
	if (dsttls > 1)
	{
		if (!dstftp->FxpDisableTls())
		{
			Event(EventHandler::transfer_failure);
			return;
		}
	}
	
	if (alt) method = ftplib::alternativefxp;
	else method = ftplib::defaultfxp;
	
	qWarning("bla_thread: %d", alt);
	
	if (resume) qWarning("WARNING: fxp resume isn't supported. overwriting file instead");
	result == ftplib::Fxp(mp_ftp, dstftp->Ftp(), src, dst, ftplib::image, method);
	
	FxpReportResult(result);
	dstftp->FxpReportResult(result);
	
	if (result) Event(EventHandler::transfer_success);
	else Event(EventHandler::transfer_failure);
}

bool FtpThread::FxpDisableTls()
{
	bool result = mp_ftp->SetDataEncryption(ftplib::unencrypted);
	if (result) 
	{
		Event(EventHandler::encryptdata_success);
		m_dataencrypted = false;
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::encryptdata_failure);
	}
	
	return result;
}

void FtpThread::FxpReportResult(bool result)
{
	if (result) Event(EventHandler::fxp_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::fxp_failure);
	}	
}

void FtpThread::Pwd_thread()
{
	char buffer[1024];
	int result;
	
	result = mp_ftp->Pwd(buffer, 1024);
	
	if (result) 
	{
		Event(EventHandler::pwd_success, new QString(buffer));
		m_pwd = buffer;
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::pwd_failure);
	}
}

void FtpThread::Chdir_thread()
{
	int result;
	
	QString path = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Chdir(path.latin1());

	if (result) Event(EventHandler::chdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::chdir_failure);
	}
}

void FtpThread::Mkdir_thread()
{
	int result;
	
	QString path = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Mkdir(path.latin1());

	if (result) Event(EventHandler::mkdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::mkdir_failure);
	}
}

void FtpThread::Cdup_thread()
{
	int result;
	
	result = mp_ftp->Cdup();

	if (result) Event(EventHandler::chdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::chdir_failure);
	}
}

void FtpThread::Rm_thread()
{
	int result;
	
	QString name = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Delete(name.latin1());

	if (result) Event(EventHandler::rm_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::rm_failure);
	}
}

void FtpThread::Rename_thread()
{
	int result;
	
	QString src = m_stringlist.front();
	m_stringlist.pop_front();
	QString dst = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Rename(src.latin1(),dst.latin1());

	if (result) Event(EventHandler::rename_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::rename_failure);
	}
}

void FtpThread::Dir_thread()
{
	int result;
	QString dirname;
	int cacheindex = -1;
	
	int force = m_intlist.front();
	m_intlist.pop_front();
	 	
	dirname = locateLocal("appdata", QString::number(rand()) + ".dir");
	
	if (KbConfig::dirCachingIsEnabled())
	{
		cacheindex = m_cache_list.findIndex(m_pwd);
	
		if ((cacheindex != -1) && (!force))
		{
			m_dircontent = m_cache_vector.at(cacheindex);
			Event(EventHandler::dir_success, &m_dircontent);
			return;
		}
	}
				
	result = mp_ftp->Dir(dirname.latin1(), "");

	if (result) 
	{		
		m_dirlist.clear();
		m_filelist.clear();		
		FormatFilelist(dirname.latin1(), m_pwd, &m_dirlist, &m_filelist); 
		m_dircontent.first = m_dirlist;
		m_dircontent.second = m_filelist;
		m_cache_vector.push_back(m_dircontent);
		m_cache_list.push_back(m_pwd);
		
		if (KbConfig::dirCachingIsEnabled())
		{	
			if (cacheindex == -1)
			{
				m_cache_vector.push_back(m_dircontent);
				m_cache_list.push_back(m_pwd);
			}
			else m_cache_vector.at(cacheindex) = m_dircontent;
		}
		Event(EventHandler::dir_success, &m_dircontent);
	}
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::dir_failure, &m_dircontent);
	}
	
	QFile::remove(dirname);
}

void FtpThread::Scandir_thread()
{
	bool result = true;

	list<KbDirInfo*>::iterator end_dir = mp_scandir->Dirlist()->end();
	for(list<KbDirInfo*>::iterator dirIterator = mp_scandir->Dirlist()->begin(); dirIterator != end_dir; dirIterator++)
	{
		result = Scandir_recurse(*dirIterator, (*dirIterator)->dirPath() + (*dirIterator)->fileName());
	}
	
	if (result) Event(EventHandler::scandir_success, mp_scandir);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::scandir_failure);
	}
}

void FtpThread::Rmdir_thread()
{
	bool result;
	
	QString name = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = Delete_recurse(name.latin1());

	if (result) Event(EventHandler::rmdir_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::rmdir_failure);
	}
}

void FtpThread::Raw_thread()
{
	bool result;
	
	QString cmd = m_stringlist.front();
	m_stringlist.pop_front();
	
	result = mp_ftp->Raw(cmd.latin1());

	if (result) Event(EventHandler::raw_success);
	else 
	{
		if (ConnectionLost()) Event(EventHandler::connectionlost);
		else Event(EventHandler::raw_failure);
	}
}

/* the thread's main loop */

void FtpThread::run()
{
	while (!m_tasklist.empty())
	{
		task t = m_tasklist.front();
		m_tasklist.pop_front();
		
		switch (t)
		{
			case FtpThread::connect:
				Connect_thread();
				break;
			case FtpThread::login:
				Login_thread();
				break;
			case FtpThread::quit:
				Quit_thread();
				break;
			case FtpThread::pwd:
				Pwd_thread();
				break;
			case FtpThread::chdir:
				Chdir_thread();
				break;
			case FtpThread::cdup:
				Cdup_thread();
				break;
			case FtpThread::dir:
				Dir_thread();
				break;
			case FtpThread::scandir:
				Scandir_thread();
				break;
			case FtpThread::rm:
				Rm_thread();
				break;
			case FtpThread::rmdir:
				Rmdir_thread();
				break;
			case FtpThread::authtls:
				Authtls_thread();
				break;
			case FtpThread::dataencryption:
				Dataencryption_thread();
				break;
			case FtpThread::transfer_get:
				Transfer_Get_thread();
				break;
			case transfer_put:
				Transfer_Put_thread();
				break;
			case FtpThread::mkdir:
				Mkdir_thread();
				break;
			case FtpThread::rename:
				Rename_thread();
				break;
			case FtpThread::raw:
				Raw_thread();
				break;
			case FtpThread::transfer_changedir:
				Transfer_Changedir_thread();
				break;
			case FtpThread::transfer_mkdir:
				Transfer_Mkdir_thread();
				break;
			case FtpThread::transfer_fxp:
				Transfer_Fxp_thread();
			default:
				Event(EventHandler::error);
				break;
		}
	}
	Event(EventHandler::finished);
}

/* event is posted to the eventreceiver */

void FtpThread::Event(EventHandler::EventType type, void *data)
{
	if (mp_eventreceiver == NULL) qWarning("WARNING: mp_eventreceiver is NULL");
	else 
	{
		QCustomEvent* e = new QCustomEvent(type);
		if (data != NULL) e->setData(data);	
		qApp->postEvent(mp_eventreceiver, e);	
	}
}

/* parses the dir file */

bool FtpThread::FormatFilelist(const char *filename, QString current, filist *dirtable, filist *filetable)
{
	int i, blocks, space, month_int = 1;
	unsigned int loc, fileloc, datebegin, sizebegin = 0;
	string buffer, filestring;

	FILE* dirfile;

	char file[1024];

	char month[][5] = {
		"... ", "Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ", "Jul ", "Aug ",
		"Sep ", "Oct ", "Nov ", "Dec "
	};

	dirfile = fopen(filename, "r");
	if (dirfile == NULL)
	{
		qWarning("ERROR: failed open dirfile");
		return false;
	}

	while (fgets(file, 1024, dirfile) != NULL)
	{
		*(strchr(file,'\n')) = '\0';
		buffer = file;

		for (i = 1; i < 13; i++)
		{
			loc = buffer.find(month[i], 0);
			if (loc != string::npos)
			{
				month_int = i;
				break;
			}
		}
		if( loc == string::npos )
		{
			qWarning("INFO: no month entry found");
			loc = buffer.length();
		}

		datebegin = loc;

		space = 1;
		blocks = 0;
		fileloc = 0;
		while (loc < buffer.length())
		{
				if (space)
				{
					// look for a space
					if (buffer[loc] == ' ')
					{
						blocks++;
						space = !space;
					}
				}
				else
				{
					// look for a non-space
					if (buffer[loc] != ' ')
					{
						if (blocks > 2)
						{
							fileloc = loc;
							break;
						}
						space = !space;
					}
				}
				loc += 1;
		}
		if (blocks != 3)
		{
			qWarning("INFO: ignoring invalid line in dirlist");
		}
		else
		{
			QString date(buffer.substr( datebegin, loc - datebegin - 1).c_str());

			int day_int = date.section( ' ', 1, 1 ).toInt();
			int year_int = date.section( ' ', -1, -1 ).toInt();
			int hour_int = 0;
			int minute_int = 0;
			
			if (day_int == 0) day_int = date.section( ' ', 1, 2 ).toInt();

			if (year_int == 0)
			{
				year_int = QDate::currentDate().year();
				if (month_int > QDate::currentDate().month()) year_int--;
				QTime mytime = QTime::fromString(date.section( ' ', -1, -1 ));
				hour_int = mytime.hour();
				minute_int = mytime.minute();
			}

			unsigned int date_int = 
				(minute_int) |
				(hour_int << 6 ) |
				(day_int << 11) |
				(month_int << 16) |
				(year_int << 20);
			
			// 6 bits for minutes, 5 bits for hours, 5 bits for days, 4 bits for months, 11 bits for years -> 31 bits (should be sufficient)
			
			space = 0;
			blocks = 0;
			loc = datebegin;
			while (loc > 0)
			{
				if (space)
				{
					if (buffer[loc] == ' ')
					{
						blocks++;
						space = !space;
					}
				}
				else
				{
					if (buffer[loc] != ' ')
					{
						if (blocks > 1)
						{
							sizebegin = loc + 1;
							break;
						}
						space = !space;
					}
				}
				loc--;
			}

			off64_t size = atoll(buffer.substr( sizebegin, datebegin - sizebegin).c_str());
			
			filestring.erase();
			filestring.append(buffer, fileloc, buffer.length() - fileloc);
			if ((filestring.compare(".") != 0) && (filestring.compare("..") != 0))
			{
				if ((*file == 'd') || (*file == 'D'))
				{
					KbFileInfo* di = new KbFileInfo(current, filestring.c_str(), size, date, date_int);
					dirtable->push_back(di);
				}
				else if ((*file == 'l') || (*file == 'L'))
				{
					qWarning("INFO: links to files not supported yet");
				}
				else
				{
					KbFileInfo* fi = new KbFileInfo(current, filestring.c_str(), size, date, date_int);
					filetable->push_back(fi);
				}
			}
		}
	}
	fclose(dirfile);
	return true;
}

/* recursive method to check content of a directory */

bool FtpThread::Scandir_recurse(KbDirInfo *dir, QString path)
{	
	char currentpath[1024];
	int result;
	filist dirlist;
	filist filelist;
	QString dirname;
		
	result = mp_ftp->Pwd(currentpath, 1024);
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	result = mp_ftp->Chdir(dir->fileName().latin1());
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return true;
	}
	else Event(EventHandler::misc_success);
	
	dirname = locateLocal("appdata", QString::number(rand()) + ".dir");
	
	result = mp_ftp->Dir(dirname.latin1(), "");
			
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	if(!FormatFilelist(dirname.latin1(), path, &dirlist, &filelist)) return false; 
	
	QFile::remove(dirname);
	
	filist::iterator end_file = filelist.end();	
	for(filist::iterator fiIterator = filelist.begin(); fiIterator != end_file; fiIterator++)
	{
		dir->AddFile(*fiIterator);
	}
	
	filist::iterator end_dir = dirlist.end();
	for(filist::iterator fiIterator = dirlist.begin(); fiIterator != end_dir; fiIterator++)
	{
		KbDirInfo* newdir = dir->AddDirectory(**fiIterator);
		if (!Scandir_recurse(newdir, path + '/' + newdir->fileName())) return false;
	}
		
	result = mp_ftp->Chdir(currentpath);
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	return true;
}

/* recursive method to delete directories */

bool FtpThread::Delete_recurse(QString name)
{
	char currentdir[1024];
	int result;
	filist dirlist;
	filist filelist;
	QString dirname;
	
	result = mp_ftp->Pwd(currentdir, 1024);
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
		
	result = mp_ftp->Chdir(name.latin1());
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return true;
	}
	else Event(EventHandler::misc_success);
	
	//dirname = QDir::homeDirPath() + 
	//"/.kasablanca/" +
	//QString::number((int) time(NULL) & 0xffff) + 
	//".dir";
	dirname = locateLocal("appdata", QString::number(rand()) + ".dir");
	
	result = mp_ftp->Dir(dirname.latin1(), "");
			
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	if(!FormatFilelist(dirname.latin1(), "", &dirlist, &filelist)) return false; 
	
	QFile::remove(dirname);
	
	filist::iterator end_file = filelist.end();
	for(filist::iterator fiIterator = filelist.begin(); fiIterator != end_file; fiIterator++)
	{
		result = mp_ftp->Delete((*fiIterator)->fileName().latin1());
		if (!result) 
		{
			Event(EventHandler::misc_failure);
			return false;
		}
		else Event(EventHandler::misc_success);
	}
	
	filist::iterator end_dir = dirlist.end();
	for(filist::iterator fiIterator = dirlist.begin(); fiIterator != end_dir; fiIterator++)
	{
		if (!Delete_recurse((*fiIterator)->fileName())) return false;
	}
		
	result = mp_ftp->Chdir(currentdir);
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	result = mp_ftp->Rmdir(name.latin1());
	
	if (!result) 
	{
		Event(EventHandler::misc_failure);
		return false;
	}
	else Event(EventHandler::misc_success);
	
	return true;
}

/* check if we lost the connection */

bool FtpThread::ConnectionLost()
{
	QString response;
	
	response = mp_ftp->LastResponse();

	if (response.startsWith("421"))
	{
		ClearQueue();
		return true;
	}
	else return false;
}
	
/* empty queue */

void FtpThread::ClearQueue()
{
	m_tasklist.clear();
	m_stringlist.clear();
	m_intlist.clear();
	m_ulonglist.clear();
	m_ftplist.clear();
	m_cache_vector.clear();
	m_cache_list.clear();
}
