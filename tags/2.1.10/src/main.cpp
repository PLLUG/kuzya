/******************************************************************************
 *   Copyright (C) 2008 by                                                    *
 *                     Volodymyr Shevchyk (volder@users.sourceforge.net),     *
 *                     Victor Sklyar (bouyantgrambler@users.sourceforge.net), *
 *                     Alex Chmykhalo (alexchmykhalo@users.sourceforge.net)   *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>     *
 ******************************************************************************/


#include <QSplashScreen>
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QString>

#include <QFileInfo>

#include <QDebug>

#include "kuzya.h"

int main(int argc, char ** argv)
{

    //qDebug()<<argc;
/*    for(int i = 1;i<argc;i++)
    {
        QString paramStr = argv[i];
        if( true == paramStr.startsWith('-') )
        {
            for (int j = 1;j < paramStr.count();j++)
            {
                switch ( paramStr.at(j).toAscii() )
                {
                    case 'h':
                    {
                        //std::cout<<"fgfgfgfg0";
                                printf("\tThe Kuzya IDE - Free Development Environment.\n\n");
                                printf("Arguments:\n");
                                printf("   -a or --about\tPrints information about Kuzya IDE and exit\n");
                                printf("   -h or --help\t\tPrints this help message and exit\n");
                                printf("   -o or --open [FILE]\tOpen file \"FILE\"\n");
                                printf("   -v or --version\tPrint version information and exit\n");
                                return 0;                               
                    }
                }

            }
        }
        
    }*/
    Q_INIT_RESOURCE(images);

//    qDebug() << QVariant(QLocale::system().name).toString();
//    qDebug()<<QLocale::languageToString(QLocale::system().language());

    QApplication a(argc, argv);


    QString splashDir;
    QSplashScreen *splash = new QSplashScreen();
#ifdef WIN32
    splashDir=QApplication::applicationDirPath()+"/../resources/SplashCukr.png";
#else
    splashDir="/usr/share/kuzya/resources/SplashCukr.png";
#endif
    splash->setPixmap(QPixmap(splashDir));
    splash->show();

    Kuzya * mw = new Kuzya();

    splash->finish(mw);
    delete splash;

    mw->show();



    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    return a.exec();
}

