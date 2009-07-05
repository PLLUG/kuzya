/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk   *
 *   i'mnotageycom.ua   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QSplashScreen>
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include "kuzya.h"

int main(int argc, char ** argv)
{
        Q_INIT_RESOURCE(images);

        QApplication a(argc, argv);
/*	QTranslator trans_ua;
        trans_ua.load("cukr_ua");
        a.installTranslator(&trans_ua);
*/
        QString splashDir;
        QSplashScreen *splash = new QSplashScreen();
#ifdef WIN32
      splashDir=QApplication::applicationDirPath()+"/../resources/SplashCukr.png";
#else
        splashDir="/usr/share/kuzyaresources/SplashCukr.png";
#endif
        splash->setPixmap(QPixmap(splashDir));
        ///splash->setPixmap(QPixmap("./src/images/SplashCukr.png"));
        splash->show();
        splash->showMessage("Kyzia is ready",Qt::AlignCenter,QColor("black"));
        Kuzya * mw = new Kuzya();
        splash->finish(mw);
        delete splash;
        mw->show();
        a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
        return a.exec();
}

