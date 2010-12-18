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


#include <QSettings>
#include <QDirIterator>
#include <QtCore/QCoreApplication>

#include "QDebug"

#include "compiler.h"
#include "compilersettings.h"

Compiler::Compiler(QObject *parent) : QProcess(parent)
{  
    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(afterExit(int, QProcess::ExitStatus)));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdErr()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilerProcessError(QProcess::ProcessError)));

    setProcessChannelMode(MergedChannels);

    QString lCompilerSettingsPath = QDir::cleanPath(QCoreApplication::applicationDirPath()
        + "../../profiles");
    CompilerSettings::setSettingsPath(lCompilerSettingsPath);

    mComplerSettings = new CompilerSettings(this);
}

Compiler::~Compiler()
{
}

QString Compiler::getProfilePath(QString lang, QString profile)
{
    return CompilerSettings::getCompilerSettingsPath(profile);
}

QStringList Compiler::getSupportedLanguages()
{
    return CompilerSettings::supportedLanguages();
}

QString Compiler::getSupportedExtensions(QString lang)
{
    return CompilerSettings::getFileFilters(lang).join(" ");
}

QStringList Compiler::getSupportedCompilers(QString lang)
{
    return CompilerSettings::supportedCompilers(lang);
}

QString Compiler::getCompilerInfo(QString lang, QString profile)
{
    return CompilerSettings::getCompilerDescription(profile);
}

void Compiler::loadProfile(QString lang, QString profile)
{
    if (lang.isEmpty() || profile.isEmpty()) return;

    compileMode = CompilerSettings::DEFAULT;

    mComplerSettings->loadCompilerSettings(profile);
}

void Compiler::setOptions(QString str)
{
    options = str;
}

void Compiler::setCompilerDir(QString dir)
{
    compilerDir = dir;
}

bool Compiler::isReady()
{
    return true;
}

bool Compiler::isModeAvailable(int compileMode)
{
    return mComplerSettings->isModeAvailable(compileMode);
 }

void Compiler::setCompilerMode(int mode)
{
    compileMode = mode;
}

void Compiler::openFile(QString srcPath)
{
    if (QFile::exists(srcPath)) sourceFile = srcPath;
    else sourceFile = "";
}

QString Compiler::getCompilerName()
{
    return mComplerSettings->compilerName();
}

QString Compiler::getCompilerParams()
{
    sourceFile = sourceFile.replace("/", QDir::separator());
    programPath = sourceFile.left(sourceFile.lastIndexOf('.'));
    sourcePath = sourceFile.left(sourceFile.lastIndexOf(QDir::separator()))+QDir::separator();

    qDebug() << "\nSOURCE FILE:" << sourceFile;
    qDebug() <<   "PROGRAM PATH:" << programPath;
    qDebug() <<   "SOURCE PATH:" << sourcePath;

    QString param = mComplerSettings->getCompilationParams(compileMode);

    if (!param.isEmpty())
    {
        param.replace(QString("$source$"), sourceFile);
        param.replace(QString("$output$"), programPath);
//        param.replace(QString("$options$"), opt+" "+options);
        param.replace(QString("$options$"), "");
        param.replace(QString("$compilerdir$"), compilerDir);
    }
    else
    {
        qDebug() << "FAIL: Could not find compiler parameters.\n";
        return "";
    }

    return param;
}

void Compiler::resetParseErrorList()
{
    parseErrorList = mComplerSettings->errorMsgTemplates();
}

void Compiler::resetParseWarningList()
{
    parseWarningList = mComplerSettings->warningMsgTemplates();
}

void Compiler::compile()
{
    if (sourceFile.isEmpty()) return;

    errorList.clear();
    warningList.clear();
    outFile.clear();

    QString compiler = getCompilerName();
//    QString config = getConfig();
    QString param = getCompilerParams();

    if (compiler.isEmpty() || param.isEmpty()) return;

    resetParseErrorList();
    resetParseWarningList();

//#ifdef WIN32
//    if (config.contains("outfile"))
//    {
//        outFile = sourcePath+"out.txt";
//        param = param+" > "+outFile;
//        qDebug() << "CONFIG:outfile";
//    }
//#endif

#ifdef WIN32
    QString prevDir = QApplication::applicationDirPath();
    if (!compilerDir.isEmpty()) QDir::setCurrent(compilerDir);
#endif

    start(QString(compiler+" "+param), QIODevice::ReadWrite);

    qDebug() << "SET PATH:" << QDir::currentPath();
    qDebug() << "COMPILE:" << compiler << " " << param;

#ifdef WIN32
    QDir::setCurrent(prevDir);
#endif
}


void Compiler::run(void)
{
    if (programPath.isEmpty()) return;

    QString prevPath = QDir::currentPath();
    QDir::setCurrent(sourcePath);

    QString pathToKuzyagraph = QDir::toNativeSeparators(QApplication::applicationDirPath());

#ifdef WIN32
    startDetached("cmd", QStringList() << "/C"<< "title "+programPath+"&&(set path=%path%;"+pathToKuzyagraph+ ")&&("+programPath+")&pause");
#else
    startDetached("xterm", QStringList() << "-e" << "/bin/sh" << "-c"<< programPath);
#endif

    QDir::setCurrent(prevPath);
}

void Compiler::afterExit(int exitCode, QProcess::ExitStatus exitStatus)
{
    int endSt;

    if ((0 == exitCode)&&(0 == exitStatus)) endSt = NOERROR;
    else
    {
        programPath = QString::null;
        endSt = ERROR;
    }

    if (!outFile.isEmpty()) readStdErr();

    emit compileEnded(endSt);
}

void Compiler::compilerProcessError(QProcess::ProcessError error)
{
    int endSt;

    if (QProcess::FailedToStart == error) endSt = FAILED_TO_START;
    else endSt = CRASHED;

    programPath = QString::null;

    emit compileEnded(endSt);
}

QList<Compiler::compilerError>* Compiler::getLastErrors(void)
{
    return &errorList;
}

QList<Compiler::compilerWarning>* Compiler::getLastWarnings(void)
{
    return &warningList;
}

void Compiler::readStdErr(void)
{
    Compiler::compilerError ce;
    Compiler::compilerWarning cw;

    QByteArray result;
    if (outFile.isEmpty()) result = readAllStandardOutput();
    else
    {
        QFile out(outFile);
        if(!out.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        result = out.readAll();
        out.close();
        QFile::remove(outFile);
    }

    QTextStream procStream(&result);
    QString line,pattern;
    QString parseParam;
    int ln,desc;
    QRegExp re;
    while (!procStream.atEnd())
    {
        line = procStream.readLine();
        qDebug() << "OUT:" << line;

        foreach(parseParam, parseErrorList)
        {
            pattern = parseParam.section(":",2);
            re.setPattern(pattern);
            if (re.indexIn(line) > -1)
            {
                ln = parseParam.section(":",0,0).toInt();
                if (0 != ln) ce.line = re.cap(ln).toInt();
                else ce.line = 0;
                desc = parseParam.section(":",1,1).toInt();
                ce.description = re.cap(desc);
                errorList.append(ce);
                break;
            }
        }

        foreach(parseParam, parseWarningList)
        {
            pattern = parseParam.section(":",2);
            re.setPattern(pattern);
            if (re.indexIn(line) > -1)
            {
                ln = parseParam.section(":",0,0).toInt();
                if (0 != ln) cw.line = re.cap(ln).toInt();
                else cw.line = 0;
                desc = parseParam.section(":",1,1).toInt();
                cw.description = re.cap(desc);
                warningList.append(cw);
                break;
            }
        }
    }
}
