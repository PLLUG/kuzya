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
#ifndef COMPILERSETTINGS_H
#define COMPILERSETTINGS_H

#include <QObject>
#include <QStringList>
#include <QDir>
class CompilerSettings : public QObject
{
    Q_OBJECT
public:
     CompilerSettings(QObject *parent = 0);
     void setSettingsPath(QString pSettingsPath);

signals:

public slots:

private:
     void refreshProfileList();

private:
     QStringList mProfilesList;
     QString mSettingsPath;
     QDir path;

};

#endif // COMPILERSETTINGS_H
