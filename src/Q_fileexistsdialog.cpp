#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_fileexistsdialog.ui'
**
** Created: Mo Jan 31 15:02:06 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_fileexistsdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a KasablancaFileExistsDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
KasablancaFileExistsDialog::KasablancaFileExistsDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "KasablancaFileExistsDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setModal( TRUE );
    KasablancaFileExistsDialogLayout = new QGridLayout( this, 1, 1, 2, 6, "KasablancaFileExistsDialogLayout"); 
    KasablancaFileExistsDialogLayout->setResizeMode( QLayout::Fixed );

    SkipButton = new QPushButton( this, "SkipButton" );

    KasablancaFileExistsDialogLayout->addWidget( SkipButton, 0, 3 );

    ResumeButton = new QPushButton( this, "ResumeButton" );
    ResumeButton->setEnabled( TRUE );

    KasablancaFileExistsDialogLayout->addWidget( ResumeButton, 0, 1 );

    RenameButton = new QPushButton( this, "RenameButton" );
    RenameButton->setEnabled( TRUE );

    KasablancaFileExistsDialogLayout->addWidget( RenameButton, 0, 2 );

    OverwriteButton = new QPushButton( this, "OverwriteButton" );

    KasablancaFileExistsDialogLayout->addWidget( OverwriteButton, 0, 0 );
    languageChange();
    resize( QSize(514, 57).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( SkipButton, SIGNAL( clicked() ), this, SLOT( Skip() ) );
    connect( OverwriteButton, SIGNAL( clicked() ), this, SLOT( Overwrite() ) );
    connect( ResumeButton, SIGNAL( clicked() ), this, SLOT( Resume() ) );
    connect( RenameButton, SIGNAL( clicked() ), this, SLOT( Rename() ) );

    // tab order
    setTabOrder( OverwriteButton, ResumeButton );
    setTabOrder( ResumeButton, RenameButton );
    setTabOrder( RenameButton, SkipButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaFileExistsDialog::~KasablancaFileExistsDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaFileExistsDialog::languageChange()
{
    setCaption( tr2i18n( "File Exists" ) );
    SkipButton->setText( tr2i18n( "Skip" ) );
    ResumeButton->setText( tr2i18n( "Resume" ) );
    RenameButton->setText( tr2i18n( "Rename" ) );
    OverwriteButton->setText( tr2i18n( "Overwrite" ) );
}

void KasablancaFileExistsDialog::Skip()
{
    qWarning( "KasablancaFileExistsDialog::Skip(): Not implemented yet" );
}

void KasablancaFileExistsDialog::Overwrite()
{
    qWarning( "KasablancaFileExistsDialog::Overwrite(): Not implemented yet" );
}

void KasablancaFileExistsDialog::Resume()
{
    qWarning( "KasablancaFileExistsDialog::Resume(): Not implemented yet" );
}

void KasablancaFileExistsDialog::Rename()
{
    qWarning( "KasablancaFileExistsDialog::Rename(): Not implemented yet" );
}

#include "Q_fileexistsdialog.moc"
