/*
 * Copyright (C) 2004 Magnus Kulke <mkulke@magnusmachine>
 */

#include "kasablanca.h"
#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A KDE Ftp Client");

static const char version[] = "0.4.0.2";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
	 KLocale::setMainCatalogue("kasablanca");	

    KAboutData about("kasablanca", I18N_NOOP("kasablanca"), version, description,
                     KAboutData::License_GPL, "(C) 2004 Magnus Kulke", 0, 0,
							"sikor_sxe@radicalapproach.de");
    about.addAuthor( "Magnus Kulke", 0, "sikor_sxe@radicalapproach.de" );
    about.addAuthor( "Big Biff", 0, "bigbiff@chunkyfilms.org" );
	 about.addCredit( "Stefan Bogner", 0, "bochi@online.ms" );
	 about.addCredit( "Christoph Thielecke", 0, "u15119@hs-harz.de" );
	about.addCredit( "Richard Stellingwerf", 0, "justremenic@hotmail.com" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    // register ourselves as a dcop client
    app.dcopClient()->registerAs(app.name(), false);

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(Kasablanca);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            Kasablanca *widget = new Kasablanca;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                Kasablanca *widget = new Kasablanca;
                widget->show();
// TODO: Load the ftp url passed on the command line.
//                widget->load(args->url(i));
            }
        }
        args->clear();
    }

    return app.exec();
}

