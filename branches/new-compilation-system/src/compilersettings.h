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

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QMap>

class QXmlStreamReader;

class CompilerSettings : public QObject
{
    Q_OBJECT

public:

    enum compileModeEnum{DEFAULT, ALTERNATIVE, OBJECT, STATIC_LIB, DYNAMIC_LIB};

     CompilerSettings(QObject *parent = 0);

     static QString settingsPath();
     static void setSettingsPath(QString pSettingsPath);
     static QStringList supportedLanguages();
     static QStringList supportedCompilers(QString pProgLanguage);
     static QString getCompilerSettingsPath(QString pCompilerName);
     static QString getCompilerDescription(QString pCompilerName);
     static QStringList getFileFilters(QString pProgLanguage);

     QString compilerName();
     void loadCompilerSettings(QString pCompilerName);
     bool isModeAvailable(int pMode);
     QString getCompilationParams(int pMode);
     QStringList errorMsgTemplates();
     QStringList warningMsgTemplates();

signals:

public slots:

private:
     static void refreshProfileList();
     static void parseSettingsInfo(QString pSettingsPath);

     void parseCompilerSettings(QString pSettingsPath);
     void parseCompilerOptions(QXmlStreamReader *pXmlReader);
     void parseOption(QXmlStreamReader *pXmlReader);
     void addCompilerOption(QString pType, QString pPlatform, QString pValue);
     void parseCompilingModes(QXmlStreamReader *pXmlReader);
     void parseCompilerErrorMessages(QXmlStreamReader *pXmlReader);
     void parseCompilerWarningMessages(QXmlStreamReader *pXmlReader);

private:
     static QMap<QString, QString> mProgLanguges;
     static QMap<QString, QStringList> mFileFilters;
     static QMap<QString, QStringList> mCompilerFilters;
     static QMap<QString, QString> mSettingsPathes;
     static QMap<QString, QString> mCompilersDescriptions;
     static QString mSettingsPath;

     QString mAddParam;
     QString mDefaultParams;
     QString mObjectParams;
     QString mStaticlibParams;
     QString mDylibParams;
     QString mAlternativeParams;
     QString mCompilerName;

     QStringList mErrorMsgs;
     QStringList mWarningMsgs;
};

#endif // COMPILERSETTINGS_H
