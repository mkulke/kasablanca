#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_generalpreferencesdialog.ui'
**
** Created: Mo Jan 31 15:02:08 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_generalpreferencesdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaGeneralPreferencesDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
KasablancaGeneralPreferencesDialog::KasablancaGeneralPreferencesDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "KasablancaGeneralPreferencesDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );
    KasablancaGeneralPreferencesDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "KasablancaGeneralPreferencesDialogLayout"); 
    KasablancaGeneralPreferencesDialogLayout->setResizeMode( QLayout::Minimum );
    spacer2_2 = new QSpacerItem( 20, 78, QSizePolicy::Minimum, QSizePolicy::Fixed );
    KasablancaGeneralPreferencesDialogLayout->addItem( spacer2_2, 3, 0 );

    textLabel1_3 = new QLabel( this, "textLabel1_3" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( textLabel1_3, 4, 0 );

    textLabel1 = new QLabel( this, "textLabel1" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( textLabel1, 0, 0 );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( textLabel1_2, 1, 0 );

    textLabel1_2_2 = new QLabel( this, "textLabel1_2_2" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( textLabel1_2_2, 2, 0 );
    spacer2 = new QSpacerItem( 20, 78, QSizePolicy::Minimum, QSizePolicy::Fixed );
    KasablancaGeneralPreferencesDialogLayout->addItem( spacer2, 3, 3 );

    kcfg_Skiplist = new QLineEdit( this, "kcfg_Skiplist" );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( kcfg_Skiplist, 0, 0, 1, 2 );

    kcfg_OnQueueFinished = new QLineEdit( this, "kcfg_OnQueueFinished" );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( kcfg_OnQueueFinished, 1, 1, 1, 2 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, buttonGroup1->sizePolicy().hasHeightForWidth() ) );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    kcfg_OnFileExistsResume = new QRadioButton( buttonGroup1, "kcfg_OnFileExistsResume" );

    buttonGroup1Layout->addWidget( kcfg_OnFileExistsResume, 1, 0 );

    kcfg_OnFileExistsSkip = new QRadioButton( buttonGroup1, "kcfg_OnFileExistsSkip" );

    buttonGroup1Layout->addWidget( kcfg_OnFileExistsSkip, 2, 0 );

    kcfg_OnFileExistsOverwrite = new QRadioButton( buttonGroup1, "kcfg_OnFileExistsOverwrite" );

    buttonGroup1Layout->addWidget( kcfg_OnFileExistsOverwrite, 0, 0 );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( buttonGroup1, 2, 3, 1, 2 );

    kcfg_Prioritylist = new QLineEdit( this, "kcfg_Prioritylist" );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( kcfg_Prioritylist, 4, 4, 1, 2 );

    textLabel1_3_2 = new QLabel( this, "textLabel1_3_2" );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( textLabel1_3_2, 5, 5, 0, 1 );

    textLabel1_3_2_2 = new QLabel( this, "textLabel1_3_2_2" );

    KasablancaGeneralPreferencesDialogLayout->addMultiCellWidget( textLabel1_3_2_2, 6, 6, 0, 1 );
    spacer9 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    KasablancaGeneralPreferencesDialogLayout->addItem( spacer9, 7, 2 );

    kcfg_SkiplistIsEnabled = new QCheckBox( this, "kcfg_SkiplistIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_SkiplistIsEnabled, 0, 3 );

    kcfg_OnQueueFinishedIsEnabled = new QCheckBox( this, "kcfg_OnQueueFinishedIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_OnQueueFinishedIsEnabled, 1, 3 );

    kcfg_OnFileExistsIsEnabled = new QCheckBox( this, "kcfg_OnFileExistsIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_OnFileExistsIsEnabled, 2, 3 );

    kcfg_PrioritylistIsEnabled = new QCheckBox( this, "kcfg_PrioritylistIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_PrioritylistIsEnabled, 4, 3 );

    kcfg_DirCachingIsEnabled = new QCheckBox( this, "kcfg_DirCachingIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_DirCachingIsEnabled, 5, 3 );

    kcfg_DeleteMovesIntoTrashIsEnabled = new QCheckBox( this, "kcfg_DeleteMovesIntoTrashIsEnabled" );

    KasablancaGeneralPreferencesDialogLayout->addWidget( kcfg_DeleteMovesIntoTrashIsEnabled, 6, 3 );
    spacer10 = new QSpacerItem( 81, 51, QSizePolicy::Expanding, QSizePolicy::Minimum );
    KasablancaGeneralPreferencesDialogLayout->addMultiCell( spacer10, 5, 6, 2, 2 );
    languageChange();
    resize( QSize(455, 303).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaGeneralPreferencesDialog::~KasablancaGeneralPreferencesDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaGeneralPreferencesDialog::languageChange()
{
    setCaption( tr2i18n( "General" ) );
    QWhatsThis::add( this, tr2i18n( "Directory caching means that once you browsed a folder on a ftp session the content of that folder is cached, and will be loaded from there in case you enter it again. This can speed up browsing especially with large dirs. If you want a fresh representation of the dir content, you can press the refresh button." ) );
    textLabel1_3->setText( tr2i18n( "Priority list:" ) );
    textLabel1->setText( tr2i18n( "Skiplist:" ) );
    textLabel1_2->setText( tr2i18n( "On queue finished:" ) );
    textLabel1_2_2->setText( tr2i18n( "On file exist:" ) );
    QWhatsThis::add( kcfg_Skiplist, tr2i18n( "Here you can enter a Regular Expression. Transfer items which match this Regular Expression are skipped. For example a value of \"^\\.\" excludes every entry beginning with \".\" from transfer." ) );
    QWhatsThis::add( kcfg_OnQueueFinished, tr2i18n( "This field can hold an executable and its arguments which will be run, when last item of in queue has finished transferring. For Example, on KDE desktops the entry \"dcop ksmserver ksmserver logout 0 0 0\" would shutdown the computer automatically." ) );
    buttonGroup1->setTitle( QString::null );
    kcfg_OnFileExistsResume->setText( tr2i18n( "Resume (if possible)" ) );
    QWhatsThis::add( kcfg_OnFileExistsResume, tr2i18n( "These Radio Buttons indicate how the application should proceed if a file to be transfered already exists on the destination. Usually the user is asked by the application how to proceed if this occurs, in case you aren't present during the transfer you can enable this feature." ) );
    kcfg_OnFileExistsSkip->setText( tr2i18n( "Skip" ) );
    QWhatsThis::add( kcfg_OnFileExistsSkip, tr2i18n( "These Radio Buttons indicate how the application should proceed if a file to be transfered already exists on the destination. Usually the user is asked by the application how to proceed if this occurs, in case you aren't present during the transfer you can enable this feature." ) );
    kcfg_OnFileExistsOverwrite->setText( tr2i18n( "Overwrite" ) );
    QWhatsThis::add( kcfg_OnFileExistsOverwrite, tr2i18n( "These Radio Buttons indicate how the application should proceed if a file to be transfered already exists on the destination. Usually the user is asked by the application how to proceed if this occurs, in case you aren't present during the transfer you can enable this feature." ) );
    QWhatsThis::add( kcfg_Prioritylist, tr2i18n( "This Field contains a regular expression, which queues the items matching this regular expression before the items which don't. For example, \".sfv\" would make the common checksum files with this fileending transfer first." ) );
    textLabel1_3_2->setText( tr2i18n( "Directory caching:" ) );
    textLabel1_3_2_2->setText( tr2i18n( "Delete moves files into trash:" ) );
    kcfg_SkiplistIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_OnQueueFinishedIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_OnFileExistsIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_PrioritylistIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_DirCachingIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_DeleteMovesIntoTrashIsEnabled->setText( tr2i18n( "Enabled" ) );
}

#include "Q_generalpreferencesdialog.moc"
