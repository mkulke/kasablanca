//
// C++ Implementation: importdialog
//
// Description:
//
//
// Author: Christoph Thielecke <u15119@hs-harz.de>, (C) 2004
//
// Contains pieces of code from kbear, Björn Sahlström <kbjorn@users.sourceforge.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qradiobutton.h>
#include <qdir.h>
#include <qfile.h>
#include <qprogressdialog.h>

#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kdebug.h>

#include "kbsiteinfo.h"
#include "importdialog.h"

using namespace std;

ImportDialog::ImportDialog(QWidget *parent, const char *name ) : KDialogBase(parent, name, true, i18n( "Import Bookmarks" ), KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, true) 
{
	mp_dialog = new KasablancaImportDialog(this);
	setMainWidget(mp_dialog);	
}

ImportDialog::~ImportDialog() 
{
}


void ImportDialog::slotOk()
{
	if (mp_dialog->GftpRadioButton->isChecked())
		importGftpBookmarks ( QDir::homeDirPath() + "/.gftp/bookmarks" );
	accept();
}

void ImportDialog::importGftpBookmarks( QString fileName ) {

//	kdDebug() << "import start" << endl;
	if ( fileName.isEmpty() || fileName.isNull() ) {
		KMessageBox::sorry( 0, i18n( "You have to select a file to import." ), i18n( "No file" ) );

		//		emit progress( 100 );
		return ;
	}

	QFile f(fileName);
	if ( !f.exists() ) {
		KMessageBox::information( 0, i18n( "No gftp bookmarks to import found." ), i18n( "No file" ) );

		//		emit progress( 100 );
		return ;
	}


	// First we fetch some global settings
	KConfig tmpConfig( fileName, true, false, "HOME" );
	QString email = tmpConfig.readEntry( "email", "anonymous@" );

	KConfig config( fileName, true, false, "HOME" );
	QStringList groupList = config.groupList();
	float size = ( float ) groupList.count();
	if ( size == 0 ) {

		//	m_hasError = true;
		//	emit progress( 100 );
		return ;
	}
	int counter = 0;

	m_lines = groupList.count();

	QProgressDialog progress( 0, "progressdialog", true );
                                progress.setLabelText( i18n( "Importing bookmarks..." ) );
                                progress.setCancelButton( 0 );
                                progress.setTotalSteps( m_lines );


	for ( QStringList::Iterator it = groupList.begin(); it != groupList.end(); ++it ) {
		QString groupName = *it; // found groupname from gFTP, can have subgroups ( group1/group2/group3 )
		QStringList groupNames = QStringList::split( "/", groupName ); //split group name into subgroups if any

		config.setGroup( groupName );
		QString tmp = config.readEntry( "hostname" );  // we ignore the site if it hasn't any host

		// first  check so the site has a host, if not ignore it
		if ( ! tmp.isNull() ) {  // had host

			KbSiteInfo newsite;

	//		kdDebug() << "-- start -- " << endl;

	//		kdDebug() << "bookmark count (read): " << counter << endl;

			tmp = groupName;
	//		kdDebug() << "label: " << tmp << endl;
			newsite.SetName( tmp );

			// set host
			tmp = config.readEntry( "hostname" );
	//		kdDebug() << "hostname: " << tmp << endl;

			// set port
			int p = config.readNumEntry( "port", 21 );
	//		kdDebug() << "port: " << p << endl;


	//		QString HostPort = tmp; /* host:port */
	//		HostPort += ":";
	//		HostPort += QString().setNum( p );

			newsite.SetInfo(tmp + ":" + QString::number(p));
	
			//newsite.SetPort( p );
			//newsite.SetHostname(tmp);

			// set protocol
			//tmp = config.readEntry( "protocol", "ftp" ).lower();
			//kdDebug() << "protocol: " << tmp << endl;

			// set remote directory
			tmp = config.readEntry( "remote directory", "/" );
	//		kdDebug() << "remote directory: " << tmp << endl;
			newsite.SetDefaultDirectory( tmp );

			// set local directory
			//tmp = config.readEntry( "local directory", QDir::homeDirPath() );
			//kdDebug() << "local directory: " << tmp << endl;

			// set username
			tmp = config.readEntry( "username", "anonymous" );
			newsite.SetUser( tmp );

			// set password
			tmp = config.readEntry( "password" );

			if ( tmp == "@EMAIL@" || tmp.isNull() || tmp.isEmpty() )
				tmp = email;

//			kdDebug() << "password: " << tmp << endl;
			newsite.SetPass( tmp );

//			kdDebug() << "-- end -- " << endl;

			newsite.SetPasv( 1 );
			newsite.SetTls( 0 );
			newsite.SetAlternativeFxp( 0 );
			newsite.SetCorrectPasv( 0 );

			m_bookmarks.push_back( newsite );
		}
		//	emit progress( int( float( counter ) / size * 100 ) );
		counter++;
		progress.setProgress( counter );
	} // for
	KMessageBox::information( 0, i18n( "%1 bookmarks from gftp successful imported." ).arg(QString().setNum(counter-1)), i18n( "Successful import" ) );
	/*
	int cnt = 0;
	list<KbSiteInfo>::iterator end_session = bookmarks.end();
	for (list<KbSiteInfo>::iterator i = bookmarks.begin(); i != end_session; i++){

		cnt++;
		kdDebug() << "bookmark count(list): " << cnt << endl;
	}*/
}

