//
// C++ Interface: kbsiteinfo
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KBSITEINFO_H
#define KBSITEINFO_H

#include <qstring.h>

#include <list>

/**
@author Magnus Kulke
*/

using namespace std;

class KbSiteInfo
{
public:
	KbSiteInfo();
	~KbSiteInfo();
	QString GetInfo() { return m_info; };
	QString GetUser() { return m_user; };
	QString GetPass() { return m_pass; };
	QString GetName() { return m_name; };
	QString GetDefaultDirectory() { return m_defaultdir; };
	int GetAlternativeFxp() { return m_altfxp; };
	int GetCorrectPasv() { return m_correctpasv; };
	int GetPasv() { return m_pasv; };
	int GetTls() { return m_tls; };
	void SetPasv(int pasv) { m_pasv = pasv; };
	void SetTls(int tls) { m_tls = tls; };
	void SetDefaultDirectory(QString path) { m_defaultdir = path; };
	void SetAlternativeFxp(int i) { m_altfxp = i; };
	void SetCorrectPasv(int i) { m_correctpasv = i; };
	void SetUser(QString user) { m_user = user; };
	void SetInfo(QString info) { m_info = info; };
	void SetPass(QString pass) { m_pass = pass; };
	void SetName(QString name) { m_name = name; };
	bool IsLegit();
	static list<KbSiteInfo> ParseBookmarks();
	static bool WriteBookmarks(const list<KbSiteInfo> bookmarks);
	void Clear();
private:
	list<KbSiteInfo> goo;
	QString m_user;
	QString m_info;
	QString m_name;
	QString m_pass;
	QString m_defaultdir;
	int m_altfxp;
	int m_correctpasv;
	int m_pasv;
	int m_tls; 
};

#endif
