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
#include <QPointer>

class QSettings;

class CompilerSettings : public QObject
{
Q_OBJECT
public:
    explicit CompilerSettings(QObject *parent = 0);
    ~CompilerSettings();

    void load(QString filePath);
    QString getName();
    QString getLanguage();
    QString getComment();
    QString getLinkerName();
    QString getLinkerComment();
    bool isPlatformSupport();
    bool redirectMsgEnabled();

    static QString location();
    static void setLocation(QString path);

signals:

public slots:

private:
    bool settingsAreValid();

    static QString settingsLocation;
    static QStringList settingsFilesList;
    static QStringList filtersList;
    static QStringList languagesList;
    static QStringList compilersList;
    static QStringList commentsList;


    QPointer<QSettings> settingsFile;



};

#endif // COMPILERSETTINGS_H
