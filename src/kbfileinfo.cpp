//
// C++ Implementation: kbfileinfo
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

// enable > 2gb support (LFS)

#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE 

#include <qregexp.h>
#include <qdir.h>

#include "kbconfig.h"
#include "kbitem.h"

#include "kbfileinfo.h"

/*KbFileInfo::KbFileInfo() : QFileInfo()
{

}*/

KbFileInfo::KbFileInfo(QString workingdir) : QFileInfo(QDir(workingdir), ".")
{

}

bool KbFileInfo::PrioritySort(KbFileInfo *f1, KbFileInfo *f2)
//bool KbFileInfo::PrioritySort(const KbFileInfo &f1, const KbFileInfo &f2)
{
	QRegExp m_prioritylist(KbConfig::prioritylist());

	if ((m_prioritylist.search(f1->fileName()) >= 0) > (m_prioritylist.search(f2->fileName()) >= 0)) return true;
	else return false;
};

KbFileInfo::KbFileInfo(QFileInfo qfi) : QFileInfo(qfi)
{
	m_date_int = 
		qfi.lastModified().date().year() * 10000 +
		qfi.lastModified().date().month() * 100 +
		qfi.lastModified().date().day();
	m_date = qfi.lastModified().toString("MMM dd yyyy");
	m_size = qfi.size();
}

KbFileInfo::KbFileInfo(const KbFileInfo& kfi) : QFileInfo(kfi)
{
	m_date_int = kfi.m_date_int;
	m_date = kfi.m_date;
	m_size = kfi.m_size;
}

KbFileInfo::KbFileInfo(KbItem* item, QString workingdir) : QFileInfo(QDir(workingdir), item->File())
{
	m_date_int = item->DateInt();
	m_date = item->Date();
	m_size = item->Size();
}

KbFileInfo::KbFileInfo(const QString & d, const QString & fileName, off64_t size, QString date, unsigned int date_int)
 : QFileInfo(QDir(d), fileName)
{
	m_date_int = date_int;
	m_date = date;
	m_size = size;
}

KbFileInfo::~KbFileInfo()
{
}

void KbFileInfo::SetDirPath(QString path)
{
	setFile(QDir(path), fileName());
} 

