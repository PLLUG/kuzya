/******************************************************************************
 *   Copyright (C) 2010 by                                                    *
 *                     Alex Chmykhalo (alexchmykhalo@users.sourceforge.net)   *
 *                                                                            *
 *                                                                            *
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
#include "compilersettings.h"
#include <QDebug>
#include <QtGui>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
CompilerSettings::CompilerSettings(QObject *parent) :
    QObject(parent)
{
}

void CompilerSettings::setSettingsPath(QString pSettingsPath)
{
       mSettingsPath = pSettingsPath;
       mProfilesList << "*.xml";
       QDirIterator dirIterator(mSettingsPath, mProfilesList,QDir::Files, QDirIterator::Subdirectories);
       QStringList results;

       while (dirIterator.hasNext())
       {
           QString qstr = dirIterator.next();
           qDebug() << qstr;
       }

    refreshProfileList();
}

void CompilerSettings::refreshProfileList()
{
    path.refresh();
}
