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
#include <QStringList>
#include <QDirIterator>

#include <QDebug>

#include "compilersettings.h"

#define MAIN_GROUP "main"
#define LANGUAGE_NAME_KEY "language/name"
#define COMPILER_NAME_KEY "compiler/name"
#define COMPILER_FILTER_KEY "compiler/filter"
#define COMPILER_COMMENT_KEY "compiler/comment"
#define LINKER_NAME_KEY "linker/name"
#define LINKER_COMMENT_KEY "linker/comment"
#define CONFIG_WIN32_ONLY_KEY "config/win32_only"
#define CONFIG_REDIRECT_MSG_KEY "config/redirect_msg"

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

QString CompilerSettings::settingsFilePath(QString compiler)
{
    QString filePath;
    int index = compilersList.indexOf(compiler);
    if (-1 == index)
    {
        filePath = settingsFilesList.at(index);
    }
    return filePath;
}

//==================PUBLIC===========================
void CompilerSettings::load(QString compiler)
{
    if (!settingsFile.isNull())
    {
        free(settingsFile);
    }

    QString filePath = settingsFilePath(compiler);
    qDebug() << "LOAD COMPILER SETTINGS:" << filePath;
    settingsFile = new QSettings(filePath, QSettings::IniFormat);
}

QString CompilerSettings::name()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(COMPILER_NAME_KEY, QVariant("")).toString();
        settingsFile->endGroup(); //MAIN_GROUP
    }
    else value = "";

    qDebug() << "COMPILER NAME:" << value;
    return value;
}

QString CompilerSettings::language()
{
      QString value;

      if (settingsAreValid())
      {
          settingsFile->beginGroup(MAIN_GROUP);
          value = settingsFile->value(LANGUAGE_NAME_KEY, QVariant("")).toString();
          settingsFile->endGroup(); //MAIN_GROUP
      }
      else value = "";

      qDebug() << "LANGUAGE NAME:" << value;
      return value;
}

QString CompilerSettings::comment()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(COMPILER_COMMENT_KEY, QVariant("")).toString();
        settingsFile->endGroup(); //MAIN_GROUP
    }
    else value = "";

    qDebug() << "COMPILER COMMENT:" << value;
    return value;
}

QString CompilerSettings::linkerName()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(LINKER_NAME_KEY, QVariant("")).toString();
        settingsFile->endGroup(); //MAIN_GROUP
    }
    else value = "";

    qDebug() << "LINKER NAME:" << value;
    return value;
}

QString CompilerSettings::linkerComment()
{
    QString value;

    if (settingsAreValid())
    {
        settingsFile->beginGroup(MAIN_GROUP);
        value = settingsFile->value(LINKER_COMMENT_KEY, QVariant("")).toString();
        settingsFile->endGroup(); //MAIN_GROUP
    }
    else value = "";

    qDebug() << "LINKER COMMENT:" << value;
    return value;
}

bool CompilerSettings::isPlatformSupport()
{
    bool value = true;

    if (settingsAreValid())
    {
        #ifndef WIN32
            settingsFile->beginGroup(MAIN_GROUP);
            value = !settingsFile->value(CONFIG_WIN32_ONLY_KEY, QVariant(true)).toBool();
            settingsFile->endGroup(); //MAIN_GROUP
        #endif
    }

    return value;
}

bool CompilerSettings::redirectMsgEnabled()
{
    bool value = false;

    if (settingsAreValid())
    {
            settingsFile->beginGroup(MAIN_GROUP);
            value = settingsFile->value(CONFIG_REDIRECT_MSG_KEY, QVariant(false)).toBool();
            settingsFile->endGroup(); //MAIN_GROUP
    }

    return value;
}

//================STATIC MEMBERS=====================

QString CompilerSettings::settingsLocation;
QStringList CompilerSettings::settingsFilesList;
QStringList CompilerSettings::filtersList;
QStringList CompilerSettings::languagesList;
QStringList CompilerSettings::compilersList;
QStringList CompilerSettings::commentsList;

void CompilerSettings::scanSettingsLocation()
{
    settingsFilesList.clear();
    filtersList.clear();
    languagesList.clear();
    compilersList.clear();
    commentsList.clear();

    QDirIterator fileIt(settingsLocation, QStringList()<< "*.ini",
                        QDir::NoDotAndDotDot|QDir::Files,
                        QDirIterator::NoIteratorFlags);

    QString filePath;
    QString filter;
    QString language;
    QString compiler;
    QString comment;

    while(fileIt.hasNext())
    {
        fileIt.next();
        if (fileIt.fileInfo().isReadable())
        {
            filePath = fileIt.fileInfo().filePath();
            qDebug() << "FOUND COMPILER SETTINGS:" << filePath;

            QSettings settings(filePath, QSettings::IniFormat);
            settings.beginGroup(MAIN_GROUP);
#ifndef WIN32
            bool platformSupported = !settingsFile->value(CONFIG_WIN32_ONLY_KEY, QVariant(true)).toBool();
            if (!platformSupported)
            {
                qDebug() << "   SKIPPED: WIN32_ONLY";
                continue;
            }
#endif
            language = settings.value(LANGUAGE_NAME_KEY, QVariant("")).toString();
            compiler = settings.value(COMPILER_NAME_KEY, QVariant("")).toString();
            filter = settings.value(COMPILER_FILTER_KEY, QVariant("")).toString();
            comment = settings.value(COMPILER_COMMENT_KEY, QVariant("")).toString();
            settings.endGroup(); //MAIN_GROUP

            if ((QSettings::NoError == settings.status()) &&
                !language.isEmpty() &&
                !compiler.isEmpty() &&
                !filter.isEmpty())
            {
                settingsFilesList << filePath;
                filtersList << filter;
                languagesList << language;
                compilersList << compiler;
                commentsList << comment;

                qDebug() << "    LANG:" << language << ";COMP:" << compiler << ";FILT:" << filter;
            }
        }
    }

}

void CompilerSettings::setLocation(QString path)
{
    if (!QDir(path).exists()) return;

    settingsLocation = path;
    scanSettingsLocation();
}

QString CompilerSettings::location()
{
    return settingsLocation;
}

QStringList CompilerSettings::supportedLanguages()
{
    QStringList languages = languagesList;
    languages.removeDuplicates();
    return languages;
}

QStringList CompilerSettings::supportedCompilers(QString lang)
{
    QStringList compilers;
    int index = 0;
    for (int i = 0; i < languagesList.count(lang); i++)
    {
        index = languagesList.indexOf(lang, index);
        if (-1 != index)
        {
            compilers << compilersList.at(index);
            index++;
        }
        else break;
    }
    return compilers;
}

QString CompilerSettings::filter(QString lang)
{
    QStringList extensions;
    QString filter;
    int index = 0;
    for (int i = 0; i < languagesList.count(lang); i++)
    {
        index = languagesList.indexOf(lang, index);
        if (-1 != index)
        {
            filter = filtersList.at(index);
            extensions << filter.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            index++;
        }
        else break;
    }
    extensions.removeDuplicates();
    return extensions.join(" ");
}

QString CompilerSettings::comment(QString name)
{
    QString compilerComment;
    int index = 0;
    index = languagesList.indexOf(name);
    if (-1 != index) compilerComment = commentsList.at(index);
    else compilerComment = "";

    return compilerComment;
}
