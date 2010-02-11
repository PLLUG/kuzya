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

#include <QPointer>
#include <QSettings>

#include <QDebug>

#include "compilersettings.h"

CompilerSettings::CompilerSettings(QObject *parent) :
    QObject(parent)
{

}

CompilerSettings::~CompilerSettings()
{
    if (!settingsFile.isNull())
    {
        free(settingsFile);
    }
}

bool CompilerSettings::areValid()
{
    if (!settingsFile.isNull())
    {
        if (QSettings::NoError == settingsFile->status())
        {
            return true;
        }
    }
    return false;
}

void CompilerSettings::load(QString filePath)
{
    if (!settingsFile.isNull())
    {
        free(settingsFile);
    }

    settingsFile = new QSettings(filePath, QSettings::IniFormat);
}
