#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_colorspreferencesdialog.ui'
**
** Created: Mo Jan 31 15:02:07 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_colorspreferencesdialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <kcolorbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaColorsPreferencesDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
KasablancaColorsPreferencesDialog::KasablancaColorsPreferencesDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "KasablancaColorsPreferencesDialog" );
    KasablancaColorsPreferencesDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "KasablancaColorsPreferencesDialogLayout"); 

    textLabel1_2 = new QLabel( this, "textLabel1_2" );

    KasablancaColorsPreferencesDialogLayout->addWidget( textLabel1_2, 0, 0 );

    textLabel1_3_3 = new QLabel( this, "textLabel1_3_3" );

    KasablancaColorsPreferencesDialogLayout->addWidget( textLabel1_3_3, 1, 0 );

    textLabel1_3 = new QLabel( this, "textLabel1_3" );

    KasablancaColorsPreferencesDialogLayout->addWidget( textLabel1_3, 2, 0 );

    textLabel1_3_2 = new QLabel( this, "textLabel1_3_2" );

    KasablancaColorsPreferencesDialogLayout->addWidget( textLabel1_3_2, 3, 0 );
    spacer3 = new QSpacerItem( 30, 60, QSizePolicy::Expanding, QSizePolicy::Minimum );
    KasablancaColorsPreferencesDialogLayout->addMultiCell( spacer3, 0, 2, 1, 1 );

    kcfg_FailureColor = new KColorButton( this, "kcfg_FailureColor" );

    KasablancaColorsPreferencesDialogLayout->addWidget( kcfg_FailureColor, 2, 2 );

    kcfg_SuccessColor = new KColorButton( this, "kcfg_SuccessColor" );

    KasablancaColorsPreferencesDialogLayout->addWidget( kcfg_SuccessColor, 1, 2 );

    kcfg_LocalColor = new KColorButton( this, "kcfg_LocalColor" );

    KasablancaColorsPreferencesDialogLayout->addWidget( kcfg_LocalColor, 3, 2 );

    kcfg_BackgroundColor = new KColorButton( this, "kcfg_BackgroundColor" );

    KasablancaColorsPreferencesDialogLayout->addWidget( kcfg_BackgroundColor, 0, 2 );
    spacer5 = new QSpacerItem( 31, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    KasablancaColorsPreferencesDialogLayout->addItem( spacer5, 4, 1 );
    languageChange();
    resize( QSize(241, 183).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaColorsPreferencesDialog::~KasablancaColorsPreferencesDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaColorsPreferencesDialog::languageChange()
{
    setCaption( tr2i18n( "Colors" ) );
    textLabel1_2->setText( tr2i18n( "Background:" ) );
    textLabel1_3_3->setText( tr2i18n( "Success Text:" ) );
    textLabel1_3->setText( tr2i18n( "Failure Text:" ) );
    textLabel1_3_2->setText( tr2i18n( "Local Text:" ) );
    kcfg_FailureColor->setText( QString::null );
    kcfg_SuccessColor->setText( QString::null );
    kcfg_LocalColor->setText( QString::null );
    kcfg_BackgroundColor->setText( QString::null );
}

#include "Q_colorspreferencesdialog.moc"
