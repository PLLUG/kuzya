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

