#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_bookmarkdialog.ui'
**
** Created: Mo Jan 31 15:02:05 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_bookmarkdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qheader.h>
#include <klistview.h>
#include <qtabwidget.h>
#include <klineedit.h>
#include <qlabel.h>
#include <kcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaBookmarkDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KasablancaBookmarkDialog::KasablancaBookmarkDialog( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KasablancaBookmarkDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    KasablancaBookmarkDialogLayout = new QGridLayout( this, 1, 1, 2, 6, "KasablancaBookmarkDialogLayout"); 

    splitter1 = new QSplitter( this, "splitter1" );
    splitter1->setOrientation( QSplitter::Horizontal );

    BookmarkListView = new KListView( splitter1, "BookmarkListView" );
    BookmarkListView->addColumn( tr2i18n( "Name" ) );
    BookmarkListView->header()->setClickEnabled( FALSE, BookmarkListView->header()->count() - 1 );
    BookmarkListView->setMinimumSize( QSize( 0, 0 ) );
    BookmarkListView->setAcceptDrops( TRUE );
    BookmarkListView->setDragEnabled( TRUE );

    SettingsTab = new QTabWidget( splitter1, "SettingsTab" );
    SettingsTab->setEnabled( TRUE );
    SettingsTab->setMinimumSize( QSize( 360, 232 ) );

    tab = new QWidget( SettingsTab, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 2, 6, "tabLayout"); 

    PassEdit = new KLineEdit( tab, "PassEdit" );
    PassEdit->setEnabled( FALSE );
    PassEdit->setEchoMode( KLineEdit::Password );

    tabLayout->addMultiCellWidget( PassEdit, 3, 3, 1, 2 );

    UserEdit = new KLineEdit( tab, "UserEdit" );
    UserEdit->setEnabled( FALSE );

    tabLayout->addMultiCellWidget( UserEdit, 2, 2, 1, 2 );

    InfoEdit = new KLineEdit( tab, "InfoEdit" );
    InfoEdit->setEnabled( FALSE );

    tabLayout->addMultiCellWidget( InfoEdit, 1, 1, 1, 2 );

    NameEdit = new KLineEdit( tab, "NameEdit" );
    NameEdit->setEnabled( FALSE );
    NameEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, NameEdit->sizePolicy().hasHeightForWidth() ) );

    tabLayout->addMultiCellWidget( NameEdit, 0, 0, 1, 2 );

    textLabel1_3 = new QLabel( tab, "textLabel1_3" );

    tabLayout->addWidget( textLabel1_3, 2, 0 );

    textLabel1 = new QLabel( tab, "textLabel1" );

    tabLayout->addWidget( textLabel1, 0, 0 );

    textLabel1_2_2_2 = new QLabel( tab, "textLabel1_2_2_2" );

    tabLayout->addWidget( textLabel1_2_2_2, 4, 0 );

    textLabel1_2 = new QLabel( tab, "textLabel1_2" );

    tabLayout->addWidget( textLabel1_2, 1, 0 );

    textLabel1_2_2 = new QLabel( tab, "textLabel1_2_2" );

    tabLayout->addWidget( textLabel1_2_2, 3, 0 );

    textLabel1_2_2_3 = new QLabel( tab, "textLabel1_2_2_3" );

    tabLayout->addWidget( textLabel1_2_2_3, 5, 0 );
    spacer8 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer8, 6, 2 );

    ModeComboBox = new KComboBox( FALSE, tab, "ModeComboBox" );
    ModeComboBox->setEnabled( FALSE );

    tabLayout->addWidget( ModeComboBox, 5, 1 );

    EncryptionComboBox = new KComboBox( FALSE, tab, "EncryptionComboBox" );
    EncryptionComboBox->setEnabled( FALSE );

    tabLayout->addWidget( EncryptionComboBox, 4, 1 );
    spacer10 = new QSpacerItem( 111, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addMultiCell( spacer10, 4, 5, 2, 2 );
    SettingsTab->insertTab( tab, QString("") );

    tab_2 = new QWidget( SettingsTab, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 2, 6, "tabLayout_2"); 

    textLabel3 = new QLabel( tab_2, "textLabel3" );

    tabLayout_2->addWidget( textLabel3, 0, 0 );

    DefaultDirectoryEdit = new QLineEdit( tab_2, "DefaultDirectoryEdit" );
    DefaultDirectoryEdit->setEnabled( FALSE );

    tabLayout_2->addMultiCellWidget( DefaultDirectoryEdit, 0, 0, 1, 3 );

    textLabel2 = new QLabel( tab_2, "textLabel2" );

    tabLayout_2->addWidget( textLabel2, 1, 0 );

    AlternativeFxpCheckBox = new QCheckBox( tab_2, "AlternativeFxpCheckBox" );
    AlternativeFxpCheckBox->setEnabled( FALSE );

    tabLayout_2->addWidget( AlternativeFxpCheckBox, 1, 3 );

    CorrectPasvCheckBox = new QCheckBox( tab_2, "CorrectPasvCheckBox" );
    CorrectPasvCheckBox->setEnabled( FALSE );

    tabLayout_2->addWidget( CorrectPasvCheckBox, 2, 3 );

    textLabel1_2_2_2_2 = new QLabel( tab_2, "textLabel1_2_2_2_2" );

    tabLayout_2->addMultiCellWidget( textLabel1_2_2_2_2, 2, 2, 0, 1 );
    spacer16 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addMultiCell( spacer16, 1, 2, 2, 2 );
    spacer7 = new QSpacerItem( 20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( spacer7, 3, 1 );
    SettingsTab->insertTab( tab_2, QString("") );

    KasablancaBookmarkDialogLayout->addWidget( splitter1, 0, 0 );
    languageChange();
    resize( QSize(541, 240).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( NameEdit, InfoEdit );
    setTabOrder( InfoEdit, UserEdit );
    setTabOrder( UserEdit, PassEdit );
    setTabOrder( PassEdit, EncryptionComboBox );
    setTabOrder( EncryptionComboBox, ModeComboBox );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaBookmarkDialog::~KasablancaBookmarkDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaBookmarkDialog::languageChange()
{
    setCaption( tr2i18n( "Bookmarks" ) );
    BookmarkListView->header()->setLabel( 0, tr2i18n( "Name" ) );
    QWhatsThis::add( PassEdit, tr2i18n( "The user's password. Anonymous ftp servers ussually don't use passwords or accept passwords in the form of \"user@emailaddress.com\"." ) );
    QWhatsThis::add( UserEdit, tr2i18n( "The ftp Login Username. On open ftp servers you usually login using \"anonymous\"." ) );
    InfoEdit->setInputMask( QString::null );
    QWhatsThis::add( InfoEdit, tr2i18n( "This information points to the the ftp server. HOST can either be a standard ip adress like 192.168.2.1 or a domain name. PORT is the port number on which the ftp server listens. In most cases this is 21. Both are seperated by a \":\" character. A legit input would be \"ftp.kde.org:21\"." ) );
    QWhatsThis::add( NameEdit, tr2i18n( "The ftp's name is entered here." ) );
    textLabel1_3->setText( tr2i18n( "Username:" ) );
    textLabel1->setText( tr2i18n( "Name:" ) );
    textLabel1_2_2_2->setText( tr2i18n( "Encryption:" ) );
    QWhatsThis::add( textLabel1_2_2_2, QString::null );
    textLabel1_2->setText( tr2i18n( "Host:Port:" ) );
    textLabel1_2_2->setText( tr2i18n( "Password:" ) );
    textLabel1_2_2_3->setText( tr2i18n( "Mode:" ) );
    ModeComboBox->clear();
    ModeComboBox->insertItem( tr2i18n( "Active Mode" ) );
    ModeComboBox->insertItem( tr2i18n( "Passive Mode" ) );
    QWhatsThis::add( ModeComboBox, tr2i18n( "The mode you are using for data transfer. This is quite important in case you're behind a firewall or a router. In these cases you depend on the passive mode, which makes the client connects to the ftp server for data transfer. Passive mode is the standard method today, while active mode is the old way to do data transfers, the client listens for the server to connect in active mode." ) );
    EncryptionComboBox->clear();
    EncryptionComboBox->insertItem( tr2i18n( "Unencrypted" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 1" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 2" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 3" ) );
    QWhatsThis::add( EncryptionComboBox, tr2i18n( "There are 4 encryption levels available. Unencrypted leaves all traffic clear and without encryption. Most ftp servers allow only this mode. Level 1 encryption means that the traffic on the control connection is encrypted while data transfer remains clear. Level 2 is basically the same with the exception of directory information which is transfered on the data channel, but is encrypted too. Eventually Level 3 encrypts everything all communication and all data traffic." ) );
    SettingsTab->changeTab( tab, tr2i18n( "General" ) );
    textLabel3->setText( tr2i18n( "Default directory:" ) );
    textLabel2->setText( tr2i18n( "Alternative fxp:" ) );
    AlternativeFxpCheckBox->setText( tr2i18n( "Enabled" ) );
    CorrectPasvCheckBox->setText( tr2i18n( "Enabled" ) );
    textLabel1_2_2_2_2->setText( tr2i18n( "Correct pasv responses:" ) );
    SettingsTab->changeTab( tab_2, tr2i18n( "Advanced" ) );
}

#include "Q_bookmarkdialog.moc"
