#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_importdialog.ui'
**
** Created: Mo Jan 31 15:02:10 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_importdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaImportDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KasablancaImportDialog::KasablancaImportDialog( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KasablancaImportDialog" );
    KasablancaImportDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "KasablancaImportDialogLayout"); 

    ImportGroupBox = new QButtonGroup( this, "ImportGroupBox" );
    ImportGroupBox->setColumnLayout(0, Qt::Vertical );
    ImportGroupBox->layout()->setSpacing( 6 );
    ImportGroupBox->layout()->setMargin( 11 );
    ImportGroupBoxLayout = new QGridLayout( ImportGroupBox->layout() );
    ImportGroupBoxLayout->setAlignment( Qt::AlignTop );

    GftpRadioButton = new QRadioButton( ImportGroupBox, "GftpRadioButton" );
    GftpRadioButton->setChecked( TRUE );

    ImportGroupBoxLayout->addWidget( GftpRadioButton, 0, 0 );

    KasablancaImportDialogLayout->addWidget( ImportGroupBox, 0, 0 );
    languageChange();
    resize( QSize(231, 80).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaImportDialog::~KasablancaImportDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaImportDialog::languageChange()
{
    setCaption( tr2i18n( "Import Bookmarks" ) );
    ImportGroupBox->setTitle( tr2i18n( "Import filter" ) );
    GftpRadioButton->setText( tr2i18n( "gFtp" ) );
}

#include "Q_importdialog.moc"
