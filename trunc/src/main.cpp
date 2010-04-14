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
#include "kuzya.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char ** argv)
{
    Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);

    QString fileName;
    opterr = 0;
    const char* short_options = "havo:";
    const struct option long_options[] = {
          {"help",no_argument,NULL,'h'},
          {"open",required_argument,NULL,'o'},
          {"about",no_argument,NULL,'a'},
          {"version",no_argument,NULL,'v'},
          {NULL,0,NULL,0}
    };
    int rez;
    int option_index;

    while ((rez=getopt_long(argc,argv,short_options,long_options,&option_index))!=-1)
    {
        switch(rez)
        {
            case 'h':
            {
                 printf("\tThe Kuzya IDE - Free Development Environment.\n\n");
                 printf("Arguments:\n");
                 printf("   -a or --about\tPrints information about Kuzya IDE and exit\n");
                 printf("   -h or --help\t\tPrints this help message and exit\n");
                 printf("   -o or --open [FILE]\tOpen file \"FILE\"\n");
                 printf("   -v or --version\tPrint version information and exit\n");
                 return 0;
                 break;
            };

            case 'v' :
            {
                printf("kuzya IDE version 2.1.9 \n");
                return 0;
                break;
            }
            case 'a' :
            {
                printf("\tThe Kuzya  "
                       "\nFree Development Environment\n\n"
                       "build on Jul 6 2009"
                       "\n\t Kuzya is simple crossplatform IDE for people who study  programming."
                       "Main idea of it is to concentrate attention  of the users only on learning the programming"
                       "language  but not on usage of IDE. For more information visit our official web site: "
                       "\n\t<http://kuzya.sourceforge.net>http://kuzya.sourceforge.net \n\n "
                       "Idea: \n \t Grygoriy Zlobin"
                       "\n zlobin@electronics.wups.lviv.ua "
                       "\n\n Team leader: \n \t Andriy Shevchyk "
                       "\n shevchyk@users.sourceforge.net "
                       "\n\n Developers:      \n \t Volodymyr Shevchyk "
                       "\n volder@users.sourceforge.net"
                       "\n              \n \t Victor Sklyar "
                       "\n bouyantgrambler@users.sourceforge.net"
                       "\n              \n \tAlex Chmykhalo "
                       "\n alexchmykhalo@users.sourceforge.net"
                       "\n\n Design:      \n \t Oksana Rondyak "
                       "\n relax777@users.sourceforge.net\n");
                return 0;
                break;
            }
            case 'o' :
            {
                fileName=optarg;
                if(!(QFile(fileName).exists()))
                {
                    printf("could not find file:%s",optarg);
                    return 0;
                }
                break;
            }
            default: /* '?' */
                           fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                                   argv[0]);
                           exit(EXIT_FAILURE);
        };
    };
    if (optind < argc)
    {
     /*   printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);*/
            fileName=argv[argc-1];
            if(!(QFile(fileName).exists()))
            {
                printf("could not find file:%s",optarg);
                return 0;
            }
     }


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

    mw->openFile(fileName);
    mw->refreshProfileSettings();

    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    return a.exec();
}

