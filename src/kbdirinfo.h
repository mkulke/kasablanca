//
// C++ Interface: kbdirinfo
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KBDIRINFO_H
#define KBDIRINFO_H

#include <list>

#include "kbfileinfo.h"

using namespace std;

/**
@author Magnus Kulke
*/
class KbDirInfo : public KbFileInfo
{
public:
	KbDirInfo(QString path);
	KbDirInfo(KbFileInfo info);
   ~KbDirInfo();
	KbDirInfo* AddDirectory(KbFileInfo info);
	void AddFile(KbFileInfo* info);
	static bool PrioritySort(const KbDirInfo *f1, const KbDirInfo *f2);
private:
	list<KbFileInfo*> m_filelist;
	list<KbDirInfo*> m_dirlist;
public:
	list<KbDirInfo*>* Dirlist() { return &m_dirlist; }
	list<KbFileInfo*>* Filelist() { return &m_filelist; }
};

#endif
