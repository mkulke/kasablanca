// This file is generated by kconfig_compiler from kbconfig.kcfg.
// All changes you do to this file will be lost.

#include "kbconfig.h"

#include <kstaticdeleter.h>

KbConfig *KbConfig::mSelf = 0;
static KStaticDeleter<KbConfig> staticKbConfigDeleter;

KbConfig *KbConfig::self()
{
  if ( !mSelf ) {
    staticKbConfigDeleter.setObject( mSelf, new KbConfig() );
    mSelf->readConfig();
  }

  return mSelf;
}

KbConfig::KbConfig(  )
  : KConfigSkeleton( QString::fromLatin1( "kasablancarc" ) )
{
  mSelf = this;
  setCurrentGroup( QString::fromLatin1( "general" ) );

  KConfigSkeleton::ItemString  *itemSkiplist;
  itemSkiplist = new KConfigSkeleton::ItemString( currentGroup(), QString::fromLatin1( "Skiplist" ), mSkiplist );
  addItem( itemSkiplist, QString::fromLatin1( "Skiplist" ) );
  KConfigSkeleton::ItemString  *itemPrioritylist;
  itemPrioritylist = new KConfigSkeleton::ItemString( currentGroup(), QString::fromLatin1( "Prioritylist" ), mPrioritylist );
  addItem( itemPrioritylist, QString::fromLatin1( "Prioritylist" ) );
  KConfigSkeleton::ItemBool  *itemDirCachingIsEnabled;
  itemDirCachingIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "DirCachingIsEnabled" ), mDirCachingIsEnabled, false );
  addItem( itemDirCachingIsEnabled, QString::fromLatin1( "DirCachingIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemDeleteMovesIntoTrashIsEnabled;
  itemDeleteMovesIntoTrashIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "DeleteMovesIntoTrashIsEnabled" ), mDeleteMovesIntoTrashIsEnabled, false );
  addItem( itemDeleteMovesIntoTrashIsEnabled, QString::fromLatin1( "DeleteMovesIntoTrashIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemSkiplistIsEnabled;
  itemSkiplistIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "SkiplistIsEnabled" ), mSkiplistIsEnabled, false );
  addItem( itemSkiplistIsEnabled, QString::fromLatin1( "SkiplistIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemPrioritylistIsEnabled;
  itemPrioritylistIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "PrioritylistIsEnabled" ), mPrioritylistIsEnabled, false );
  addItem( itemPrioritylistIsEnabled, QString::fromLatin1( "PrioritylistIsEnabled" ) );
  KConfigSkeleton::ItemString  *itemOnQueueFinished;
  itemOnQueueFinished = new KConfigSkeleton::ItemString( currentGroup(), QString::fromLatin1( "OnQueueFinished" ), mOnQueueFinished );
  addItem( itemOnQueueFinished, QString::fromLatin1( "OnQueueFinished" ) );
  KConfigSkeleton::ItemBool  *itemOnQueueFinishedIsEnabled;
  itemOnQueueFinishedIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "OnQueueFinishedIsEnabled" ), mOnQueueFinishedIsEnabled, false );
  addItem( itemOnQueueFinishedIsEnabled, QString::fromLatin1( "OnQueueFinishedIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemOnFileExistsOverwrite;
  itemOnFileExistsOverwrite = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "OnFileExistsOverwrite" ), mOnFileExistsOverwrite, false );
  addItem( itemOnFileExistsOverwrite, QString::fromLatin1( "OnFileExistsOverwrite" ) );
  KConfigSkeleton::ItemBool  *itemOnFileExistsResume;
  itemOnFileExistsResume = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "OnFileExistsResume" ), mOnFileExistsResume, true );
  addItem( itemOnFileExistsResume, QString::fromLatin1( "OnFileExistsResume" ) );
  KConfigSkeleton::ItemBool  *itemOnFileExistsSkip;
  itemOnFileExistsSkip = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "OnFileExistsSkip" ), mOnFileExistsSkip, false );
  addItem( itemOnFileExistsSkip, QString::fromLatin1( "OnFileExistsSkip" ) );
  KConfigSkeleton::ItemBool  *itemOnFileExistsIsEnabled;
  itemOnFileExistsIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "OnFileExistsIsEnabled" ), mOnFileExistsIsEnabled, false );
  addItem( itemOnFileExistsIsEnabled, QString::fromLatin1( "OnFileExistsIsEnabled" ) );

  setCurrentGroup( QString::fromLatin1( "ui" ) );

  KConfigSkeleton::ItemBool  *itemSystrayIsEnabled;
  itemSystrayIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "SystrayIsEnabled" ), mSystrayIsEnabled, false );
  addItem( itemSystrayIsEnabled, QString::fromLatin1( "SystrayIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemCommandLineIsEnabled;
  itemCommandLineIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "CommandLineIsEnabled" ), mCommandLineIsEnabled, true );
  addItem( itemCommandLineIsEnabled, QString::fromLatin1( "CommandLineIsEnabled" ) );
  KConfigSkeleton::ItemBool  *itemHideHiddenFilesIsEnabled;
  itemHideHiddenFilesIsEnabled = new KConfigSkeleton::ItemBool( currentGroup(), QString::fromLatin1( "HideHiddenFilesIsEnabled" ), mHideHiddenFilesIsEnabled, false );
  addItem( itemHideHiddenFilesIsEnabled, QString::fromLatin1( "HideHiddenFilesIsEnabled" ) );
  KConfigSkeleton::ItemFont  *itemLogwindowFont;
  itemLogwindowFont = new KConfigSkeleton::ItemFont( currentGroup(), QString::fromLatin1( "LogwindowFont" ), mLogwindowFont, QFont( "DEC Terminal" ) );
  addItem( itemLogwindowFont, QString::fromLatin1( "LogwindowFont" ) );

  setCurrentGroup( QString::fromLatin1( "colors" ) );

  KConfigSkeleton::ItemColor  *itemLocalColor;
  itemLocalColor = new KConfigSkeleton::ItemColor( currentGroup(), QString::fromLatin1( "LocalColor" ), mLocalColor, QColor( 244,247,66 ) );
  addItem( itemLocalColor, QString::fromLatin1( "LocalColor" ) );
  KConfigSkeleton::ItemColor  *itemSuccessColor;
  itemSuccessColor = new KConfigSkeleton::ItemColor( currentGroup(), QString::fromLatin1( "SuccessColor" ), mSuccessColor, QColor( 0,188,53 ) );
  addItem( itemSuccessColor, QString::fromLatin1( "SuccessColor" ) );
  KConfigSkeleton::ItemColor  *itemBackgroundColor;
  itemBackgroundColor = new KConfigSkeleton::ItemColor( currentGroup(), QString::fromLatin1( "BackgroundColor" ), mBackgroundColor, QColor( 0,90,128 ) );
  addItem( itemBackgroundColor, QString::fromLatin1( "BackgroundColor" ) );
  KConfigSkeleton::ItemColor  *itemFailureColor;
  itemFailureColor = new KConfigSkeleton::ItemColor( currentGroup(), QString::fromLatin1( "FailureColor" ), mFailureColor, QColor( 222,9,48 ) );
  addItem( itemFailureColor, QString::fromLatin1( "FailureColor" ) );
}

KbConfig::~KbConfig()
{
  if ( mSelf == this )
    staticKbConfigDeleter.setObject( mSelf, 0, false );
}

