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

#define MAIN_GROUP "main"
#define COMPILER_NAME_KEY "compiler/name"
#define COMPILER_COMMENT_KEY "compiler/comment"
#define LINKER_NAME_KEY "linker/name"
#define LINKER_COMMENT_KEY "linker/comment"
#define CONFIG_WIN32_KEY "config/win32"
#define CONFIG_UNIX_KEY "config/unix"
#define CONFIG_MSGFILE_KEY "config/msgfile"

#define COMPILATION_GROUP "compile"
#define MODE_NAME_KEY "name"
#define MODE_PARAMS_KEY "params"
#define MODE_LINK_KEY "link"
#define MODE_LINK_PARAMS_KEY "link_params"

#define ERRORS_GROUP "errors"
#define WARNINGS_GROUP "warnings"
#define MESSAGES_GROUP "messages"
#define LINE_CAP_KEY "ln"
#define COLUMN_CAP_KEY "col"
#define MSG_CAP_KEY "msg"
#define REG_EXP_KEY "exp"



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

//==================PRIVATE=========================

bool CompilerSettings::settingsAreValid()
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

//==================PUBLIC===========================
void CompilerSettings::load(QString filePath)
{
    if (!settingsFile.isNull())
    {
        free(settingsFile);
    }

    settingsFile = new QSettings(filePath, QSettings::IniFormat);
}

QString CompilerSettings::getName()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(COMPILER_NAME_KEY, "").toString();
        settingsFile->endGroup(); //MAIN_SETTINGS_GROUP
    }
    else value = "";

    return value;
}

QString CompilerSettings::getComment()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(COMPILER_COMMENT_KEY, "").toString();
        settingsFile->endGroup(); //MAIN_SETTINGS_GROUP
    }
    else value = "";

    return value;
}

QString CompilerSettings::getLinkerName()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(LINKER_NAME_KEY, "").toString();
        settingsFile->endGroup(); //MAIN_SETTINGS_GROUP
    }
    else value = "";

    return value;
}

QString CompilerSettings::getLinkerComment()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(LINKER_COMMENT_KEY, "").toString();
        settingsFile->endGroup(); //MAIN_SETTINGS_GROUP
    }
    else value = "";

    return value;
}
