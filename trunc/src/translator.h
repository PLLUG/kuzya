/******************************************************************************
 *   Copyright (C) 2008 by                                                    *
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
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QString>
#include <QStringList>

class Translator : public QObject
{
    Q_OBJECT

public:
    Translator(QObject *parent = 0);
    ~Translator();

    QStringList getSupportedTranslations(QString lang);
    void openFile(QString file, QString lang);
    QString translation();
    QString codeFile();
    QString translatedCodeFile();
    void retranslate();
    void setTranslation(QString transl);

private:
    enum DirectionEnum { fromCode, toCode };

    QString detectCodeLanguage(QString filePath, QString lang);
    void translateCode(QString srcFile, QString destFile, DirectionEnum direction);

private:
    QStringList supportedTranslations;
    QString fileName;
    QString translatedFileName;
    QString translationsPath;
    QString language;
    QString codeTranslation;
};

#endif // TRANSLATOR_H
