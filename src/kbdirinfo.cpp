//
// C++ Implementation: kbdirinfo
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qregexp.h>

#include <list>

#include "kbconfig.h"
#include "kbdirinfo.h"

using namespace std;

KbDirInfo::KbDirInfo(KbFileInfo info) : KbFileInfo(info)
{
}

KbDirInfo::KbDirInfo(QString path) : KbFileInfo(path)
{
}

KbDirInfo::~KbDirInfo()
{
}

bool KbDirInfo::PrioritySort(const KbDirInfo *f1, const KbDirInfo *f2)
{
	QRegExp m_prioritylist(KbConfig::prioritylist());

	if ((m_prioritylist.search(f1->fileName()) >= 0) > (m_prioritylist.search(f2->fileName()) >= 0)) return true;
	else return false;
}

KbDirInfo* KbDirInfo::AddDirectory(KbFileInfo info)
{
	if ((info.fileName() == ".") || (info.fileName() == "..")) return NULL;
	KbDirInfo* dir = new KbDirInfo(info);
	m_dirlist.push_back(dir);
	return dir;
}

void KbDirInfo::AddFile(KbFileInfo* file)
{
	m_filelist.push_back(file);
}
