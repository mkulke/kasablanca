//
// C++ Implementation: kbsiteinfo
//
// Description: 
//
//
// Author: Magnus Kulke <mkulke@magnusmachine>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <kstandarddirs.h>

#include <qdom.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>

#include "kbsiteinfo.h"

KbSiteInfo::KbSiteInfo()
{
	Clear();
}


KbSiteInfo::~KbSiteInfo()
{
}

void KbSiteInfo::Clear()
{
	m_name = "";
	m_info = "";
	m_user = "";
	m_pass = "";
	m_defaultdir = "";
	m_altfxp = 0;
	m_correctpasv = 0;
	m_pasv = 1;
	m_tls = 0;
}

bool KbSiteInfo::IsLegit()
{
	if ((m_name != "") && (m_info != "") && (m_user != "")) return true;
	else return false;
}

list<KbSiteInfo> KbSiteInfo::ParseBookmarks()
{
	list<KbSiteInfo> bookmarks;
	bookmarks.clear();
	
	QDomDocument doc( "KasablancaBookmarks" );

	QFile filein(locateLocal("appdata", "bookmarks.xml"));
	
	if( !filein.open( IO_ReadOnly ) ) return bookmarks;
	
	if( !doc.setContent( &filein ) )
	{
		filein.close();
		return bookmarks;
	}
	
	filein.close();

	QDomElement root = doc.documentElement();

	if( root.tagName() != "kasablanca" ) return bookmarks;

	QDomNode curNode = root.firstChild();
	
	while( !curNode.isNull() )
	{
		QDomElement siteElement = curNode.toElement();

		if( !siteElement.isNull() )
		{
			if( siteElement.tagName() == "site" )
			{
					KbSiteInfo entry;
					entry.SetPasv(1);
					entry.SetTls(0);
					entry.SetAlternativeFxp(0);
					entry.SetCorrectPasv(0);
					entry.SetDefaultDirectory("");
					entry.SetName(siteElement.attribute( "name", "" ));

					QDomNode siteNode = siteElement.firstChild();

					while( !siteNode.isNull() )
					{
						QDomElement curElement = siteNode.toElement();

						if( !curElement.isNull() )
						{
							if(curElement.tagName() == "user") entry.SetUser(curElement.text());
							if(curElement.tagName() == "pass") entry.SetPass(curElement.text());
							if(curElement.tagName() == "info") entry.SetInfo(curElement.text());
							if(curElement.tagName() == "defaultdirectory") entry.SetDefaultDirectory(curElement.text());
							if(curElement.tagName() == "pasv") entry.SetPasv(curElement.text().toInt());
							if(curElement.tagName() == "tls") entry.SetTls(curElement.text().toInt());
							if(curElement.tagName() == "alternativefxp") entry.SetAlternativeFxp(curElement.text().toInt());
							if(curElement.tagName() == "correctpasv") entry.SetCorrectPasv(curElement.text().toInt());
						}

						siteNode = siteNode.nextSibling();
					}

					if (entry.IsLegit()) bookmarks.push_back(entry);
			}
		}

		curNode = curNode.nextSibling();
	}

	return bookmarks;
}

bool KbSiteInfo::WriteBookmarks(list<KbSiteInfo> bookmarks)
{
	QDomDocument doc( "KasablancaBookmarks" );
	QDomElement root = doc.createElement( "kasablanca" );
	doc.appendChild( root );
	 
	list<KbSiteInfo>::iterator end_bookmarks = bookmarks.end();
	for (list<KbSiteInfo>::iterator i = bookmarks.begin(); i != end_bookmarks; i++)
	{
		if ((*i).IsLegit())
		{
			QDomElement siteElement = doc.createElement("site");
			siteElement.setAttribute("name", (*i).GetName());
			root.appendChild( siteElement );
			
			QDomText text;
			QDomElement curElement;
			
			curElement = doc.createElement( "user" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode((*i).GetUser());
			curElement.appendChild(text);
			
			curElement = doc.createElement( "pass" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode((*i).GetPass());
			curElement.appendChild(text);
			
			curElement = doc.createElement( "info" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode((*i).GetInfo());
			curElement.appendChild(text);
			
			curElement = doc.createElement( "pasv" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode(QString::number((*i).GetPasv()));
			curElement.appendChild(text);
			
			curElement = doc.createElement( "tls" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode(QString::number((*i).GetTls()));
			curElement.appendChild(text);
			
			curElement = doc.createElement( "alternativefxp" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode(QString::number((*i).GetAlternativeFxp()));
			curElement.appendChild(text);
			
			curElement = doc.createElement( "correctpasv" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode(QString::number((*i).GetCorrectPasv()));
			curElement.appendChild(text);
			
			curElement = doc.createElement( "defaultdirectory" );
			siteElement.appendChild( curElement );
			text = doc.createTextNode((*i).GetDefaultDirectory());
			curElement.appendChild(text);
        }
    }
		
	 QFile fileout(locateLocal("appdata", "bookmarks.xml"));
    
    if( !fileout.open( IO_WriteOnly ) ) return false;

    QTextStream ts( &fileout );
    ts << doc.toString();

    fileout.close();
	 
	 return true;
}
