#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_customconnectdialog.ui'
**
** Created: Mo Jan 31 15:02:09 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_customconnectdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaCustomConnectDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KasablancaCustomConnectDialog::KasablancaCustomConnectDialog( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KasablancaCustomConnectDialog" );
    KasablancaCustomConnectDialogLayout = new QGridLayout( this, 1, 1, 2, 6, "KasablancaCustomConnectDialogLayout"); 

    Custom = new QTabWidget( this, "Custom" );

    tab = new QWidget( Custom, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 2, 6, "tabLayout"); 

    textLabel4 = new QLabel( tab, "textLabel4" );
    textLabel4->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    tabLayout->addWidget( textLabel4, 2, 0 );

    UserLineEdit = new QLineEdit( tab, "UserLineEdit" );
    UserLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, UserLineEdit->sizePolicy().hasHeightForWidth() ) );

    tabLayout->addMultiCellWidget( UserLineEdit, 1, 1, 1, 2 );

    PassLineEdit = new QLineEdit( tab, "PassLineEdit" );
    PassLineEdit->setEchoMode( QLineEdit::Password );

    tabLayout->addMultiCellWidget( PassLineEdit, 2, 2, 1, 2 );

    InfoEdit = new QLineEdit( tab, "InfoEdit" );
    InfoEdit->setAlignment( int( QLineEdit::AlignAuto ) );

    tabLayout->addMultiCellWidget( InfoEdit, 0, 0, 1, 2 );

    textLabel3 = new QLabel( tab, "textLabel3" );
    textLabel3->setAlignment( int( QLabel::AlignVCenter ) );

    tabLayout->addWidget( textLabel3, 1, 0 );

    textLabel2 = new QLabel( tab, "textLabel2" );
    textLabel2->setAlignment( int( QLabel::AlignVCenter ) );

    tabLayout->addWidget( textLabel2, 0, 0 );

    textLabel1 = new QLabel( tab, "textLabel1" );
    textLabel1->setAlignment( int( QLabel::AlignVCenter ) );

    tabLayout->addWidget( textLabel1, 3, 0 );

    textLabel1_2 = new QLabel( tab, "textLabel1_2" );
    textLabel1_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

    tabLayout->addWidget( textLabel1_2, 4, 0 );

    AnonymousCheckBox = new QCheckBox( tab, "AnonymousCheckBox" );

    tabLayout->addWidget( AnonymousCheckBox, 1, 3 );
    spacer12 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer12, 5, 1 );

    EncryptionComboBox = new QComboBox( FALSE, tab, "EncryptionComboBox" );

    tabLayout->addWidget( EncryptionComboBox, 3, 1 );

    ModeComboBox = new QComboBox( FALSE, tab, "ModeComboBox" );

    tabLayout->addWidget( ModeComboBox, 4, 1 );
    spacer10 = new QSpacerItem( 120, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout->addMultiCell( spacer10, 3, 4, 2, 3 );
    Custom->insertTab( tab, QString("") );

    tab_2 = new QWidget( Custom, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 2, 6, "tabLayout_2"); 

    textLabel2_2 = new QLabel( tab_2, "textLabel2_2" );

    tabLayout_2->addWidget( textLabel2_2, 1, 0 );

    AlternativeFxpCheckBox = new QCheckBox( tab_2, "AlternativeFxpCheckBox" );

    tabLayout_2->addWidget( AlternativeFxpCheckBox, 1, 3 );

    DefaultDirectoryEdit = new QLineEdit( tab_2, "DefaultDirectoryEdit" );

    tabLayout_2->addMultiCellWidget( DefaultDirectoryEdit, 0, 0, 1, 3 );

    textLabel1_2_2_2_2 = new QLabel( tab_2, "textLabel1_2_2_2_2" );

    tabLayout_2->addMultiCellWidget( textLabel1_2_2_2_2, 2, 2, 0, 1 );

    CorrectPasvCheckBox = new QCheckBox( tab_2, "CorrectPasvCheckBox" );

    tabLayout_2->addWidget( CorrectPasvCheckBox, 2, 3 );

    textLabel3_2 = new QLabel( tab_2, "textLabel3_2" );

    tabLayout_2->addWidget( textLabel3_2, 0, 0 );
    spacer16 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( spacer16, 3, 1 );
    spacer17 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addMultiCell( spacer17, 1, 2, 2, 2 );
    Custom->insertTab( tab_2, QString("") );

    KasablancaCustomConnectDialogLayout->addWidget( Custom, 0, 0 );
    languageChange();
    resize( QSize(370, 214).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaCustomConnectDialog::~KasablancaCustomConnectDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaCustomConnectDialog::languageChange()
{
    setCaption( tr2i18n( "Custom Site" ) );
    textLabel4->setText( tr2i18n( "Password:" ) );
    QWhatsThis::add( UserLineEdit, tr2i18n( "The ftp Login Username. On open ftp servers you usually login using \"anonymous\"." ) );
    QWhatsThis::add( PassLineEdit, tr2i18n( "The user's password. Anonymous ftp servers ussually don't use passwords or accept passwords in the form of \"user@emailaddress.com\"." ) );
    InfoEdit->setText( QString::null );
    QWhatsThis::add( InfoEdit, tr2i18n( "This information points to the the ftp server. HOST can either be a standard ip adress like \"192.168.2.1\" or a domain name. PORT is the port number on which the ftp server listens. In most cases this is 21. Both are seperated by a \":\" character. A legit input would be \"ftp.kde.org:21\"." ) );
    textLabel3->setText( tr2i18n( "Username:" ) );
    textLabel2->setText( tr2i18n( "Host:Port:" ) );
    textLabel1->setText( tr2i18n( "Encryption:" ) );
    textLabel1_2->setText( tr2i18n( "Mode:" ) );
    AnonymousCheckBox->setText( tr2i18n( "Anonymous" ) );
    EncryptionComboBox->clear();
    EncryptionComboBox->insertItem( tr2i18n( "Unencrypted" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 1" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 2" ) );
    EncryptionComboBox->insertItem( tr2i18n( "Level 3" ) );
    QWhatsThis::add( EncryptionComboBox, tr2i18n( "There are 4 encryption levels available. Unencrypted leaves all traffic clear and without encryption. Most ftp servers allow only this mode. Level 1 encryption means that the traffic on the control connection is encrypted while data transfer remains clear. Level 2 is basically the same with the exception of directory information which is transfered on the data channel, but is encrypted too. Eventually Level 3 encrypts everything all communication and all data traffic." ) );
    ModeComboBox->clear();
    ModeComboBox->insertItem( tr2i18n( "Passive Mode" ) );
    ModeComboBox->insertItem( tr2i18n( "Active Mode" ) );
    QWhatsThis::add( ModeComboBox, tr2i18n( "The mode you are using for data transfer. This is quite important in case you're behind a firewall or a router. In these cases you depend on the passive mode, which makes the client connects to the ftp server for data transfer. Passive mode is the standard method today, while active mode is the old way to do data transfers, the client listens for the server to connect in active mode." ) );
    Custom->changeTab( tab, tr2i18n( "General" ) );
    textLabel2_2->setText( tr2i18n( "Alternative fxp:" ) );
    AlternativeFxpCheckBox->setText( tr2i18n( "Enabled" ) );
    textLabel1_2_2_2_2->setText( tr2i18n( "Correct pasv responses:" ) );
    CorrectPasvCheckBox->setText( tr2i18n( "Enabled" ) );
    textLabel3_2->setText( tr2i18n( "Default directory:" ) );
    Custom->changeTab( tab_2, tr2i18n( "Advanced" ) );
}

#include "Q_customconnectdialog.moc"
