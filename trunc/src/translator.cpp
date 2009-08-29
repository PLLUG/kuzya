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

#include "translator.h"

#include <QDirIterator>
#include <QApplication>

#include <QDebug>

Translator::Translator(QObject *parent) : QObject(parent)
{
}

Translator::~Translator()
{
}

QStringList Translator::getSupportedTranslations(QString lang)
{
#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/profiles/"+lang;
#else
    QString path = "/usr/share/kuzya/profiles/"+lang;
#endif

    QDirIterator fileIt(path, QStringList()<< "*.tr", QDir::NoDotAndDotDot|QDir::Files, QDirIterator::NoIteratorFlags);
    QString translation;
    while (fileIt.hasNext())
    {
        fileIt.next();
        if (fileIt.fileInfo().isReadable())
        {
            translation = fileIt.fileInfo().fileName();
            translation.truncate(translation.lastIndexOf("."));
            supportedTranslations << translation;
        }
    }
    supportedTranslations.removeDuplicates();
    return supportedTranslations;
}

QString Translator::detectCodeLanguage(QString filePath, QString lang)
{
    QFile code(filePath);
    if(!code.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "en";
    }
    QTextStream codeStream(&code);
    QString text = codeStream.readAll();

    QStringList supportedTransl = getSupportedTranslations(lang);

    QFile fileTrans;
    QTextStream trStream;
    QString trLine;
    QString translation;
    foreach (QString transl, supportedTransl)
    {
        fileTrans.setFileName(translationsPath+transl+".tr");
        if(!fileTrans.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return "en";
        }
        trStream.setDevice(&fileTrans);
        trStream.setCodec("UTF-8");
        while (!trStream.atEnd())
        {
            trLine = trStream.readLine();
            translation = trLine.section('=', 1);
            if (text.contains(translation))
            {
                fileTrans.close();
                return transl;
            }
        }
        fileTrans.close();
    }
    return "en";
}

void Translator::openFile(QString file, QString lang)
{
    language = lang;
    fileName = file;
    translatedFileName = file;

#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    translationsPath = path+"/profiles/"+lang+"/";
#else
    translationsPath = "/usr/share/kuzya/profiles/"+lang+"/";
#endif

    codeTranslation = detectCodeLanguage(file, language);
    if ("en" != codeTranslation)
    {
        fileName = file;
        fileName.truncate(file.lastIndexOf("."));
        QString ex = file;
        ex.remove(0, ex.lastIndexOf("."));
        fileName = fileName+"_src"+ex;
    }
}

void Translator::setTranslation(QString transl)
{
    if (transl == codeTranslation) return;

    retranslate();

    codeTranslation = transl;

    translatedFileName = fileName;
    if ("en" != transl)
    {
        translatedFileName = fileName;
        translatedFileName.remove("_src");
        translatedFileName.remove("_"+codeTranslation);
        translatedFileName.truncate(translatedFileName.lastIndexOf("."));
        QString ex = fileName;
        ex.remove(0, ex.lastIndexOf("."));
        translatedFileName = translatedFileName+"_"+codeTranslation+ex;
    }
    translateCode(fileName, translatedFileName, fromCode);
}

void Translator::retranslate()
{
    if ("en" != codeTranslation)
    {
        translateCode(translatedFileName, fileName, toCode);
    }
}

QString Translator::translation()
{
    return codeTranslation;
}

QString Translator::codeFile()
{
    return fileName;
}

QString Translator::translatedCodeFile()
{
    return translatedFileName;
}

void Translator::translateCode(QString srcFile, QString destFile, DirectionEnum direction)
{
    QFile fileTrans(translationsPath+codeTranslation+".tr");
    if(!fileTrans.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream trStream(&fileTrans);
    trStream.setCodec("UTF-8");

    QFile inFile(srcFile);
    if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream inStream(&inFile);
    QString text = inStream.readAll();

    QString trLine;
    QString key;
    QString translation;
    while (!trStream.atEnd())
    {
        trLine = trStream.readLine();
        key = trLine;
        key.truncate(key.lastIndexOf('='));
        translation = trLine.section('=', 1);
        if (fromCode == direction)
        {
            text.replace(key, translation);
        }
        else
        {
            text.replace(translation, key);
        }
    }
    fileTrans.close();

    QFile outFile(destFile);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         return ;
    }
    QTextStream outStream(&outFile);
    outStream << text;
    outFile.close();

}


