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

#include <qevent.h>

#include "ftpthread.h"
#include "eventhandler.h"

/* 
class description:

the class transforms the events posted by an ftp thread into signals. the signals
are composed from the type of the arrived event and in certain cases an out_variable.
*/

/* constructor */

EventHandler::EventHandler(QObject *parent, const char *name)
 : QObject(parent, name)
{
	installEventFilter(this);

	mp_thread = NULL;
}

/* destructor */

EventHandler::~EventHandler()
{
}

/* set ftp thread */

void EventHandler::SetFtpThread(FtpThread* thread)
{
	mp_thread = thread;
}

/* the eventfilter method. when a certain event arrives a signal is emitted, composed
from the events type and a possible out_ variable from the ftp thread object. */

bool EventHandler::eventFilter(QObject*, QEvent *e )
{
	EventType type = (EventType)(e->type());

	if (mp_thread == NULL)
	{
		emit ftp_error("mp_thread is NULL");
		return TRUE;
	}
	else if (type == outlog) 
	{   
		QString *line = static_cast<QString*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_log(*line, true);
		delete line;
      return TRUE; 
   }
	else if (type == inlog) 
	{   
		QString *line = static_cast<QString*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_log(*line, false);
		delete line;
      return TRUE; 
   }
	else if (type == xfered) 
	{   
		xferpair* xp = static_cast<xferpair*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_xfered(xp->first, xp->second);
		delete xp;
      return TRUE; 
   }
	else if (type == finished) 
	{   
		emit ftp_finished();
      return TRUE; 
   }	
	else if (type == connectionlost) 
	{   
		emit ftp_connectionlost();
      return TRUE; 
   }	
	else if (type == connect_success) 
	{   
		emit ftp_connect(true);
      return TRUE; 
   } 
	else if (type == connect_failure) 
	{   
		emit ftp_connect(false);
      return TRUE; 
   }
	else if (type == misc_success) 
	{   
		emit ftp_misc(true);
      return TRUE; 
   } 
	else if (type == misc_failure) 
	{   
		emit ftp_misc(false);
      return TRUE; 
   }
	else if (type == login_success) 
	{   
		emit ftp_login(true);
      return TRUE; 
   } 
	else if (type == login_failure) 
	{   
		emit ftp_login(false);
      return TRUE; 
   }
	else if (type == quit_success) 
	{   
		emit ftp_quit(true);
      return TRUE; 
   } 
	else if (type == quit_failure) 
	{   
		emit ftp_quit(false);
      return TRUE; 
   }
	else if (type == authtls_success) 
	{   
		emit ftp_authtls(true);
      return TRUE; 
   } 
	else if (type == authtls_failure) 
	{   
		emit ftp_authtls(false);
      return TRUE; 
   }
	else if (type == pwd_success) 
	{   
		QString *path = static_cast<QString*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_pwd(true, *path);
		delete path;
      return TRUE; 
   } 
	else if (type == pwd_failure) 
	{   
		emit ftp_pwd(false, "");
      return TRUE; 
   }
	else if (type == chdir_success) 
	{   
		emit ftp_chdir(true);
      return TRUE; 
   }
	else if (type == chdir_failure) 
	{   
		emit ftp_chdir(false);
      return TRUE; 
   }
	else if (type == transfer_success) 
	{   
		emit ftp_transfer(true);
      return TRUE; 
   }
	else if (type == transfer_failure) 
	{   
		emit ftp_transfer(false);
      return TRUE; 
   }
	else if (type == dir_success) 
	{   
		contentpair *content = static_cast<contentpair*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_dir(true, content->first, content->second);
      return TRUE; 
   }
	else if (type == dir_failure) 
	{   
		contentpair *content = static_cast<contentpair*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_dir(false, content->first, content->second);
      return TRUE; 
   }
	else if (type == scandir_success) 
	{   
		KbDirInfo *dir = static_cast<KbDirInfo*>(static_cast<QCustomEvent*>(e)->data());
		emit ftp_scandir(true, dir);
      return TRUE; 
   }
	else if (type == scandir_failure) 
	{   
		emit ftp_scandir(false, NULL);
      return TRUE; 
   }
	else if (type == rm_success) 
	{   
		emit ftp_rm(true);
      return TRUE; 
   }
	else if (type == rm_failure) 
	{   
		emit ftp_rm(false);
      return TRUE; 
   }
	else if (type == rmdir_success) 
	{   
		emit ftp_rmdir(true);
      return TRUE; 
   }
	else if (type == rmdir_failure) 
	{   
		emit ftp_rmdir(false);
      return TRUE; 
   }
	else if (type == encryptdata_success) 
	{   
		emit ftp_encryptdata(true);
      return TRUE; 
   }
	else if (type == encryptdata_failure) 
	{   
		emit ftp_encryptdata(false);
      return TRUE; 
   }
	else if (type == get_success) 
	{   
		emit ftp_get(true);
      return TRUE; 
   }
	else if (type == get_failure) 
	{   
		emit ftp_get(false);
      return TRUE; 
   }
	else if (type == mkdir_success) 
	{   
		emit ftp_mkdir(true);
      return TRUE; 
   }
	else if (type == mkdir_failure) 
	{   
		emit ftp_mkdir(false);
      return TRUE; 
   }
	else if (type == rename_success) 
	{   
		emit ftp_rename(true);
      return TRUE; 
   }
	else if (type == rename_failure) 
	{   
		emit ftp_rename(false);
      return TRUE; 
   }
	else if (type == raw_success) 
	{   
		emit ftp_raw(true);
      return TRUE; 
   }
	else if (type == raw_failure) 
	{   
		emit ftp_raw(false);
      return TRUE; 
   }
	else if (type == put_success) 
	{   
		emit ftp_put(true);
      return TRUE; 
   }
	else if (type == put_failure) 
	{   
		emit ftp_put(false);
      return TRUE; 
   }
	else if (type == fxp_success) 
	{   
		emit ftp_fxp(true);
      return TRUE; 
   }
	else if (type == fxp_failure) 
	{   
		emit ftp_fxp(false);
      return TRUE; 
   }
	else 
	{
      return FALSE;
  	}
}


#include "eventhandler.moc"
