#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file '/home/mkulke/Development/kasablanca/src/Q_mainwindow.ui'
**
** Created: Mo Jan 31 15:02:06 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.3   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Q_mainwindow.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qheader.h>
#include <qlistview.h>
#include <klineedit.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "kbtaskview.h"

/*
 *  Constructs a KasablancaMainWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KasablancaMainWindow::KasablancaMainWindow( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KasablancaMainWindow" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    KasablancaMainWindowLayout = new QGridLayout( this, 1, 1, 2, 6, "KasablancaMainWindowLayout"); 

    ConnectButtonA = new QToolButton( this, "ConnectButtonA" );
    ConnectButtonA->setEnabled( TRUE );
    ConnectButtonA->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( ConnectButtonA, 0, 0 );

    RefreshButtonA = new QToolButton( this, "RefreshButtonA" );
    RefreshButtonA->setEnabled( TRUE );
    RefreshButtonA->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( RefreshButtonA, 0, 1 );

    TransferButtonA = new QToolButton( this, "TransferButtonA" );
    TransferButtonA->setEnabled( FALSE );
    TransferButtonA->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( TransferButtonA, 0, 2 );

    ConnectButtonB = new QToolButton( this, "ConnectButtonB" );
    ConnectButtonB->setEnabled( TRUE );
    ConnectButtonB->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( ConnectButtonB, 0, 4 );

    RefreshButtonB = new QToolButton( this, "RefreshButtonB" );
    RefreshButtonB->setEnabled( TRUE );
    RefreshButtonB->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( RefreshButtonB, 0, 5 );

    TransferButtonB = new QToolButton( this, "TransferButtonB" );
    TransferButtonB->setEnabled( FALSE );
    TransferButtonB->setAutoRaise( FALSE );

    KasablancaMainWindowLayout->addWidget( TransferButtonB, 0, 6 );

    BrowserA = new QListView( this, "BrowserA" );
    BrowserA->addColumn( tr2i18n( "Name" ) );
    BrowserA->addColumn( tr2i18n( "Size" ) );
    BrowserA->addColumn( tr2i18n( "Date" ) );
    BrowserA->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)2, (QSizePolicy::SizeType)7, 0, 0, BrowserA->sizePolicy().hasHeightForWidth() ) );
    BrowserA->setResizePolicy( QScrollView::Manual );
    BrowserA->setSelectionMode( QListView::Extended );

    KasablancaMainWindowLayout->addMultiCellWidget( BrowserA, 1, 1, 0, 3 );

    BrowserB = new QListView( this, "BrowserB" );
    BrowserB->addColumn( tr2i18n( "Name" ) );
    BrowserB->addColumn( tr2i18n( "Size" ) );
    BrowserB->addColumn( tr2i18n( "Date" ) );
    BrowserB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)2, (QSizePolicy::SizeType)7, 0, 0, BrowserB->sizePolicy().hasHeightForWidth() ) );
    BrowserB->setSelectionMode( QListView::Extended );

    KasablancaMainWindowLayout->addMultiCellWidget( BrowserB, 1, 1, 4, 7 );

    CwdLineA = new KLineEdit( this, "CwdLineA" );

    KasablancaMainWindowLayout->addWidget( CwdLineA, 0, 3 );

    CwdLineB = new KLineEdit( this, "CwdLineB" );

    KasablancaMainWindowLayout->addWidget( CwdLineB, 0, 7 );

    CommandLineA = new KLineEdit( this, "CommandLineA" );

    KasablancaMainWindowLayout->addMultiCellWidget( CommandLineA, 2, 2, 0, 3 );

    CommandLineB = new KLineEdit( this, "CommandLineB" );

    KasablancaMainWindowLayout->addMultiCellWidget( CommandLineB, 2, 2, 4, 7 );

    splitter4 = new QSplitter( this, "splitter4" );
    splitter4->setOrientation( QSplitter::Vertical );

    TaskView = new KbTaskView( splitter4, "TaskView" );

    QWidget* privateLayoutWidget = new QWidget( splitter4, "layout2" );
    layout2 = new QGridLayout( privateLayoutWidget, 1, 1, 0, 6, "layout2"); 

    LogWindowB = new QTextEdit( privateLayoutWidget, "LogWindowB" );
    LogWindowB->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    LogWindowB->setPaletteBackgroundColor( QColor( 82, 194, 255 ) );
    QFont LogWindowB_font(  LogWindowB->font() );
    LogWindowB_font.setFamily( "Dec Terminal" );
    LogWindowB->setFont( LogWindowB_font ); 
    LogWindowB->setHScrollBarMode( QTextEdit::AlwaysOff );
    LogWindowB->setTextFormat( QTextEdit::PlainText );
    LogWindowB->setWrapPolicy( QTextEdit::AtWordOrDocumentBoundary );
    LogWindowB->setReadOnly( TRUE );

    layout2->addWidget( LogWindowB, 0, 1 );

    LogWindowA = new QTextEdit( privateLayoutWidget, "LogWindowA" );
    LogWindowA->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    LogWindowA->setPaletteBackgroundColor( QColor( 82, 194, 255 ) );
    QFont LogWindowA_font(  LogWindowA->font() );
    LogWindowA_font.setFamily( "Dec Terminal" );
    LogWindowA->setFont( LogWindowA_font ); 
    LogWindowA->setHScrollBarMode( QTextEdit::AlwaysOff );
    LogWindowA->setTextFormat( QTextEdit::PlainText );
    LogWindowA->setWrapPolicy( QTextEdit::AtWordOrDocumentBoundary );
    LogWindowA->setReadOnly( TRUE );

    layout2->addWidget( LogWindowA, 0, 0 );

    KasablancaMainWindowLayout->addMultiCellWidget( splitter4, 3, 3, 0, 7 );
    languageChange();
    resize( QSize(689, 626).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( BrowserA, BrowserB );
    setTabOrder( BrowserB, LogWindowA );
}

/*
 *  Destroys the object and frees any allocated resources
 */
KasablancaMainWindow::~KasablancaMainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KasablancaMainWindow::languageChange()
{
    setCaption( tr2i18n( "Kasablanca x" ) );
    ConnectButtonA->setText( QString::null );
    RefreshButtonA->setText( QString::null );
    TransferButtonA->setText( QString::null );
    ConnectButtonB->setText( QString::null );
    RefreshButtonB->setText( QString::null );
    TransferButtonB->setText( QString::null );
    BrowserA->header()->setLabel( 0, tr2i18n( "Name" ) );
    BrowserA->header()->setLabel( 1, tr2i18n( "Size" ) );
    BrowserA->header()->setLabel( 2, tr2i18n( "Date" ) );
    BrowserB->header()->setLabel( 0, tr2i18n( "Name" ) );
    BrowserB->header()->setLabel( 1, tr2i18n( "Size" ) );
    BrowserB->header()->setLabel( 2, tr2i18n( "Date" ) );
    LogWindowB->setText( QString::null );
    LogWindowA->setText( QString::null );
}

#include "Q_mainwindow.moc"
