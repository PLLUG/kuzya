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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDirIterator>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamReader>

#include <QDebug>
#include <QtCore/QCoreApplication>

#define TAG_ROOT "compilersettings"
#define ATTRIBUTE_ROOT_COMPILER_NAME "name"
#define ATTRIBUTE_ROOT_COMPILER_INFO "info"
#define ATTRIBUTE_ROOT_PROGRAM_LANGUAGE "lang"
#define ATTRIBUTE_ROOT_FILE_FILTER "filter"

#define TAG_COMPILER_OPTIONS "options"
#define TAG_OPTION "option"
#define ATTRIBUTE_OPTION_TYPE "type"
#define ATTRIBUTE_OPTION_PLATFORM "platform"
#define ATTRIBUTE_OPTION_VALUE "value"

#define TYPE_PARAM "param"

#define PLATFORM_WINDOWS "windows"
#define PLATFORM_UNIX "unix"

#define TAG_COMPILING_MODES "modes"
#define TAG_COMPILING_MODE_DEFAULT "default"
#define TAG_COMPILING_MODE_OBJECT_FILE "object"
#define TAG_COMPILING_MODE_STATIC_LIB "staticlib"
#define TAG_COMPILING_MODE_DYNAMIC_LIB "dylib"
#define TAG_COMPILING_MODE_ALTERNATIVE "alternative"
#define ATTRIBUTE_COMPILING_MODE_PARAMETERS "params"

#define TAG_MSG_ERRORS "errors"
#define TAG_MSG_WARNINGS "warnings"
#define TAG_MSG "msg"
#define ATTRIBUTE_MSG_LINE_CAP_POS "ln"
#define ATTRIBUTE_MSG_TEXT_CAP_POS "txt"
#define ATTRIBUTE_MSG_VISIBLE "visible"
#define ATTRIBUTE_MSG_REGEXP "regexp"

CompilerSettings::CompilerSettings(QObject *parent) :
    QObject(parent)
{
}

QMap<QString, QString> CompilerSettings::mProgLanguges;
QMap<QString, QStringList> CompilerSettings::mFileFilters;
QMap<QString, QStringList> CompilerSettings::mCompilerFilters;
QMap<QString, QString> CompilerSettings::mSettingsPathes;
QMap<QString, QString> CompilerSettings::mCompilersDescriptions;

void CompilerSettings::parseSettingsInfo(QString pSettingsPath)
{
    if (pSettingsPath.isEmpty())
    {
        qDebug() << "ERROR: Settings path is empty!";
        return;
    }

    if (!QFile::exists(pSettingsPath))
    {
        qDebug() << "ERROR: Settings file not exists!";
        return;
    }

    QFile lSettingsFile(pSettingsPath);
    if (!lSettingsFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Cannot open settings file!";
        return;
    }

    QXmlStreamReader lXmlReader;
    lXmlReader.setDevice(&lSettingsFile);

    lXmlReader.readNextStartElement();

    if (lXmlReader.name() != TAG_ROOT)
    {
        qDebug() << "ERROR: In settings file" << pSettingsPath << " : cannot find root tag!";
        return;
    }

    QXmlStreamAttributes lAttributes = lXmlReader.attributes();

    if (!lAttributes.hasAttribute(ATTRIBUTE_ROOT_COMPILER_NAME)
        || !lAttributes.hasAttribute(ATTRIBUTE_ROOT_PROGRAM_LANGUAGE)
        || !lAttributes.hasAttribute(ATTRIBUTE_ROOT_FILE_FILTER)
        || !lAttributes.hasAttribute(ATTRIBUTE_ROOT_COMPILER_INFO))
    {
        qDebug() << "ERROR: In settings file" << pSettingsPath << ": cannot find all info!";
        return;
    }

    QString lCompilerName = lAttributes.value(ATTRIBUTE_ROOT_COMPILER_NAME).toString();
    QString lCompilerInfo = lAttributes.value(ATTRIBUTE_ROOT_COMPILER_INFO).toString();
    QString lProgLanguage = lAttributes.value(ATTRIBUTE_ROOT_PROGRAM_LANGUAGE).toString();
    QString lFileFilters = lAttributes.value(ATTRIBUTE_ROOT_FILE_FILTER).toString();

    mSettingsPathes.insert(lCompilerName, pSettingsPath);
    mProgLanguges.insertMulti(lProgLanguage, lCompilerName);
    mCompilersDescriptions.insert(lCompilerName, lCompilerInfo);
    QStringList lFilters = mFileFilters.value(lProgLanguage);
    lFilters.append(lFileFilters.split(','));
    lFilters.removeDuplicates();
    mFileFilters.insert(lProgLanguage, lFilters);
    mCompilerFilters.insert(lCompilerName, lFileFilters.split(','));

    lSettingsFile.close();
}

QStringList CompilerSettings::supportedLanguages()
{
    return mProgLanguges.keys();
}

QStringList CompilerSettings::supportedCompilers(QString pProgLanguage)
{
    return mProgLanguges.values(pProgLanguage);
}

QString CompilerSettings::getCompilerSettingsPath(QString pCompilerName)
{
    return mSettingsPathes.value(pCompilerName);
}

QString CompilerSettings::getCompilerDescription(QString pCompilerName)
{
    return mCompilersDescriptions.value(pCompilerName);
}

QStringList CompilerSettings::getFileFilters(QString pProgLanguage)
{
    return mFileFilters.value(pProgLanguage);
}

void CompilerSettings::parseCompilerSettings(QString pSettingsPath)
{
    if (pSettingsPath.isEmpty())
    {
        qDebug() << "ERROR: Settings file path is empty!";
        return;
    }

    if (!QFile::exists(pSettingsPath))
    {
        qDebug() << "ERROR: Settings file not exists!";
        return;
    }

    QFile lSettingsFile(pSettingsPath);
    if (!lSettingsFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: In settings file " << pSettingsPath << ": cannnot open settings file!";
        return;
    }

    QXmlStreamReader lXmlReader;
    lXmlReader.setDevice(&lSettingsFile);

    lXmlReader.readNextStartElement();

    if (lXmlReader.name() != TAG_ROOT)
    {
        qDebug() << "ERROR: In settings file " << pSettingsPath << ": cannot find root tag!";
        return;
    }

    while (!lXmlReader.atEnd())
    {
        lXmlReader.readNext();

        if (lXmlReader.isStartElement())
        {
            if (lXmlReader.name() == TAG_COMPILER_OPTIONS)
            {
                parseCompilerOptions(&lXmlReader);
            };

            if (lXmlReader.name() == TAG_COMPILING_MODES)
            {
                parseCompilingModes(&lXmlReader);
            }

            if (lXmlReader.name() == TAG_MSG_ERRORS)
            {
                parseCompilerErrorMessages(&lXmlReader);
            }

            if (lXmlReader.name() == TAG_MSG_WARNINGS)
            {
                parseCompilerWarningMessages(&lXmlReader);
            }
        }
    }
}

void CompilerSettings::parseCompilerOptions(QXmlStreamReader *pXmlReader)
{
    while (!pXmlReader->atEnd())
    {
        pXmlReader->readNext();

        if ((pXmlReader->name() == TAG_COMPILER_OPTIONS) && (pXmlReader->isEndElement()))
        {
            break;
        }

        if (pXmlReader->isStartElement())
        {
            if (pXmlReader->name() == TAG_OPTION)
            {
                parseOption(pXmlReader);
            }
        }
    }
}

void CompilerSettings::parseOption(QXmlStreamReader *pXmlReader)
{
    if (pXmlReader->name() == TAG_OPTION)
    {
        QXmlStreamAttributes lAttributes = pXmlReader->attributes();

        QString lOptionType = lAttributes.value(ATTRIBUTE_OPTION_TYPE).toString();
        QString lOptionPlatform = lAttributes.value(ATTRIBUTE_OPTION_PLATFORM).toString();
        QString lOptionValue = lAttributes.value(ATTRIBUTE_OPTION_VALUE).toString();

        addCompilerOption(lOptionType, lOptionPlatform, lOptionValue);
    }
}

void CompilerSettings::addCompilerOption(QString pType, QString pPlatform, QString pValue)
{
    if (!pType.isEmpty() && !pValue.isEmpty())
    {
        if (TYPE_PARAM == pType)
        {
#ifdef WIN32
            if (PLATFORM_WINDOWS)
            {
                mAddParam.append(pValue);
            }
#else
            if (PLATFORM_UNIX == pPlatform)
            {
                mAddParam.append(pValue);
            }
#endif
        }
    }
}

void CompilerSettings::parseCompilingModes(QXmlStreamReader *pXmlReader)
{
    while (!pXmlReader->atEnd())
    {
        pXmlReader->readNext();

        if ((pXmlReader->isEndElement()) && (pXmlReader->name() == TAG_COMPILING_MODES))
        {
            break;
        }

        if (pXmlReader->isStartElement())
        {
            QXmlStreamAttributes lAttributes = pXmlReader->attributes();

            if (pXmlReader->name() == TAG_COMPILING_MODE_DEFAULT)
            {
                mDefaultParams = lAttributes.value(ATTRIBUTE_COMPILING_MODE_PARAMETERS).toString();
            }

            if (pXmlReader->name() == TAG_COMPILING_MODE_OBJECT_FILE)
            {
                mObjectParams = lAttributes.value(ATTRIBUTE_COMPILING_MODE_PARAMETERS).toString();
            }

            if (pXmlReader->name() == TAG_COMPILING_MODE_STATIC_LIB)
            {
                mStaticlibParams = lAttributes.value(ATTRIBUTE_COMPILING_MODE_PARAMETERS).toString();
            }

            if (pXmlReader->name() == TAG_COMPILING_MODE_DYNAMIC_LIB)
            {
                mDylibParams = lAttributes.value(ATTRIBUTE_COMPILING_MODE_PARAMETERS).toString();
            }

            if (pXmlReader->name() == TAG_COMPILING_MODE_ALTERNATIVE)
            {
                mAlternativeParams = lAttributes.value(ATTRIBUTE_COMPILING_MODE_PARAMETERS).toString();
            }
        }
    }
}

void CompilerSettings::parseCompilerErrorMessages(QXmlStreamReader *pXmlReader)
{
    while (!pXmlReader->atEnd())
    {
        pXmlReader->readNext();

        if ((pXmlReader->isEndElement()) && (pXmlReader->name() == TAG_MSG_ERRORS))
        {
            break;
        }

        if (pXmlReader->isStartElement())
        {
            if (pXmlReader->name() == TAG_MSG)
            {
                QXmlStreamAttributes lAttributes = pXmlReader->attributes();

                QString lLineCap = lAttributes.value(ATTRIBUTE_MSG_LINE_CAP_POS).toString();
                QString lTextCap = lAttributes.value(ATTRIBUTE_MSG_TEXT_CAP_POS).toString();
                bool lVisible = QVariant(lAttributes.value(ATTRIBUTE_MSG_VISIBLE).toString()).toBool();
                QString lRegExpStr = lAttributes.value(ATTRIBUTE_MSG_REGEXP).toString();

                if (lVisible && !lRegExpStr.isEmpty())
                {
                    QString lErrorMsgTemplate = lLineCap + ":" + lTextCap + ":" + lRegExpStr;
                    mErrorMsgs.append(lErrorMsgTemplate);
                }
            }
        }
    }
}

void CompilerSettings::parseCompilerWarningMessages(QXmlStreamReader *pXmlReader)
{
    while (!pXmlReader->atEnd())
    {
        pXmlReader->readNext();

        if ((pXmlReader->isEndElement()) && (pXmlReader->name() == TAG_MSG_WARNINGS))
        {
            break;
        }

        if (pXmlReader->isStartElement())
        {
            if (pXmlReader->name() == TAG_MSG)
            {
                QXmlStreamAttributes lAttributes = pXmlReader->attributes();

                QString lLineCap = lAttributes.value(ATTRIBUTE_MSG_LINE_CAP_POS).toString();
                QString lTextCap = lAttributes.value(ATTRIBUTE_MSG_TEXT_CAP_POS).toString();
                bool lVisible = QVariant(lAttributes.value(ATTRIBUTE_MSG_VISIBLE).toString()).toBool();
                QString lRegExpStr = lAttributes.value(ATTRIBUTE_MSG_REGEXP).toString();

                if (lVisible && !lRegExpStr.isEmpty())
                {
                    QString lWarningMsgTemplate = lLineCap + ":" + lTextCap + ":" + lRegExpStr;
                    mWarningMsgs.append(lWarningMsgTemplate);
                }
            }
        }
    }
}

void CompilerSettings::refreshProfileList()
{
    mProgLanguges.clear();
    mFileFilters.clear();
    mCompilerFilters.clear();
    mSettingsPathes.clear();
    mCompilersDescriptions.clear();

    QStringList lListFilters;
    lListFilters.append("*.xml");

    QDirIterator lDirIterator(mSettingsPath, lListFilters, QDir::Files, QDirIterator::Subdirectories);

    while (lDirIterator.hasNext())
    {
        QString lCompilerSettingsPath = lDirIterator.next();

        parseSettingsInfo(lCompilerSettingsPath);
    }
}

QString CompilerSettings::mSettingsPath;

QString CompilerSettings::settingsPath()
{
    return mSettingsPath;
}

void CompilerSettings::setSettingsPath(QString pSettingsPath)
{
    if (pSettingsPath.isEmpty())
    {
        return;
    }

    if (QDir(pSettingsPath).exists())
    {
        mSettingsPath = pSettingsPath;
        refreshProfileList();
    }
    else
    {
        qDebug() << "ERROR: Settings path does not exists!";
    }
}

void CompilerSettings::loadCompilerSettings(QString pCompilerName)
{
    QString lSettingsPath = getCompilerSettingsPath(pCompilerName);

    if (!lSettingsPath.isEmpty())
    {
        mAddParam.clear();
        mDefaultParams.clear();
        mObjectParams.clear();
        mStaticlibParams.clear();
        mDylibParams.clear();
        mAlternativeParams.clear();

        mErrorMsgs.clear();
        mWarningMsgs.clear();

        mCompilerName = pCompilerName;

        parseCompilerSettings(lSettingsPath);
    }
    else
    {
        qDebug() << "ERROR: Compiler settings not found!";
    }
}

bool CompilerSettings::isModeAvailable(int pMode)
{
    switch (pMode)
    {
    case DEFAULT:
        return !mDefaultParams.isEmpty();
    case ALTERNATIVE:
        return !mAlternativeParams.isEmpty();
    case OBJECT:
        return !mObjectParams.isEmpty();
    case STATIC_LIB:
        return !mStaticlibParams.isEmpty();
    case DYNAMIC_LIB:
        return !mDylibParams.isEmpty();
    }

    return false;
}

QString CompilerSettings::getCompilationParams(int pMode)
{
    QString lModeParams;

    switch (pMode)
    {
    case DEFAULT:
        lModeParams = mDefaultParams;
        break;
    case ALTERNATIVE:
        lModeParams = mAlternativeParams;
        break;
    case OBJECT:
        lModeParams = mObjectParams;
        break;
    case STATIC_LIB:
        lModeParams = mStaticlibParams;
        break;
    case DYNAMIC_LIB:
        lModeParams = mDylibParams;
        break;
    }

    if (lModeParams.isEmpty())
    {
        return "";
    }

    return lModeParams;
}

QStringList CompilerSettings::errorMsgTemplates()
{
    return mErrorMsgs;
}

QStringList CompilerSettings::warningMsgTemplates()
{
    return mWarningMsgs;
}

QString CompilerSettings::compilerName()
{
    return mCompilerName;
}
