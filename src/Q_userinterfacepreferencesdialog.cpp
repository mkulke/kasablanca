#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_userinterfacepreferencesdialog.ui'
**
** Created: Mo Jan 31 15:02:09 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_userinterfacepreferencesdialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <kfontrequester.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaUserInterfacePreferencesDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
KasablancaUserInterfacePreferencesDialog::KasablancaUserInterfacePreferencesDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "KasablancaUserInterfacePreferencesDialog" );
    KasablancaUserInterfacePreferencesDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "KasablancaUserInterfacePreferencesDialogLayout"); 
    spacer4 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    KasablancaUserInterfacePreferencesDialogLayout->addItem( spacer4, 4, 3 );

    textLabel1_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2" );

    KasablancaUserInterfacePreferencesDialogLayout->addMultiCellWidget( textLabel1_2_2_2_2, 0, 0, 0, 1 );

    textLabel1_2_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2_2" );

    KasablancaUserInterfacePreferencesDialogLayout->addMultiCellWidget( textLabel1_2_2_2_2_2, 1, 1, 0, 1 );

    textLabel1_2_2_2_2_2_3 = new QLabel( this, "textLabel1_2_2_2_2_2_3" );

    KasablancaUserInterfacePreferencesDialogLayout->addMultiCellWidget( textLabel1_2_2_2_2_2_3, 2, 2, 0, 2 );

    kcfg_SystrayIsEnabled = new QCheckBox( this, "kcfg_SystrayIsEnabled" );

    KasablancaUserInterfacePreferencesDialogLayout->addWidget( kcfg_SystrayIsEnabled, 0, 4 );

    kcfg_CommandLineIsEnabled = new QCheckBox( this, "kcfg_CommandLineIsEnabled" );

    KasablancaUserInterfacePreferencesDialogLayout->addWidget( kcfg_CommandLineIsEnabled, 1, 4 );

    kcfg_HideHiddenFilesIsEnabled = new QCheckBox( this, "kcfg_HideHiddenFilesIsEnabled" );

    KasablancaUserInterfacePreferencesDialogLayout->addWidget( kcfg_HideHiddenFilesIsEnabled, 2, 4 );

    kcfg_LogwindowFont = new KFontRequester( this, "kcfg_LogwindowFont" );

    KasablancaUserInterfacePreferencesDialogLayout->addMultiCellWidget( kcfg_LogwindowFont, 3, 3, 2, 4 );

    textLabel1_2_2_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2_2_2" );

    KasablancaUserInterfacePreferencesDialogLayout->addWidget( textLabel1_2_2_2_2_2_2, 3, 0 );
    spacer3 = new QSpacerItem( 140, 58, QSizePolicy::Expanding, QSizePolicy::Minimum );
    KasablancaUserInterfacePreferencesDialogLayout->addMultiCell( spacer3, 0, 1, 2, 3 );
    spacer3_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    KasablancaUserInterfacePreferencesDialogLayout->addItem( spacer3_2, 3, 1 );
    languageChange();
    resize( QSize(448, 165).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaUserInterfacePreferencesDialog::~KasablancaUserInterfacePreferencesDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaUserInterfacePreferencesDialog::languageChange()
{
    setCaption( tr2i18n( "User Interface" ) );
    textLabel1_2_2_2_2->setText( tr2i18n( "Show system tray icon:" ) );
    textLabel1_2_2_2_2_2->setText( tr2i18n( "Show command lines:" ) );
    textLabel1_2_2_2_2_2_3->setText( tr2i18n( "Hide hidden files from view:" ) );
    kcfg_SystrayIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_CommandLineIsEnabled->setText( tr2i18n( "Enabled" ) );
    kcfg_HideHiddenFilesIsEnabled->setText( tr2i18n( "Enabled" ) );
    textLabel1_2_2_2_2_2_2->setText( tr2i18n( "Logwindow Font:" ) );
}

#include "Q_userinterfacepreferencesdialog.moc"
