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
#include <QMessageBox>
#include "QDebug"

#include "compiler.h"

Compiler::Compiler(QObject *parent) : QProcess(parent)
{
    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(afterExit(int, QProcess::ExitStatus)));
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdErr()));
    connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilerProcessError(QProcess::ProcessError)));

    setProcessChannelMode(MergedChannels);
    compilerProfile = NULL;
    refreshSupported();

    QString pathToKuzyagraph = QDir::toNativeSeparators(QApplication::applicationDirPath());
    qputenv("PATH", qgetenv("PATH") + pathToKuzyagraph.toUtf8());
}

Compiler::~Compiler()
{
    if (compilerProfile!=NULL) free(compilerProfile);
}

void Compiler::refreshSupported()
{
    supportedLanguages.clear();
    supportedExtensions.clear();
    profileLocations.clear();
    supportedCompilers.clear();
    profilesPathList.clear();

#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/profiles";
#else
    QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../kuzya/profiles");
    if (false == QDir(path).exists())
    {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/../profiles");
        if (false == QDir(path).exists())
        {
            QMessageBox msgBox;
            msgBox.setText("There is some problem with loading of profiles");
            msgBox.exec();
        }
    }
#endif

    QDirIterator it(path, QDir::NoDotAndDotDot|QDir::AllDirs, QDirIterator::NoIteratorFlags);

    QString name;
    while (it.hasNext())
    {
        it.next();
        if (it.fileInfo().isDir())
        {
            name = it.fileInfo().filePath()+"/"+it.fileInfo().fileName()+".ini";
            QSettings info(name, QSettings::IniFormat);
            if (QSettings::NoError == info.status())
            {
                info.beginGroup("info");
                QString lang = info.value("language", "").toString();
                QString ext = info.value("filter","").toString();
                info.endGroup();

                if (lang.isEmpty()||ext.isEmpty()) continue;

                supportedLanguages << lang;
                supportedExtensions << ext;

                QString location =  it.fileInfo().filePath();
                profileLocations << location;

                QDirIterator fileIt(location, QStringList()<< "*.prof", QDir::NoDotAndDotDot|QDir::Files, QDirIterator::NoIteratorFlags);
                QString prof;
                QString profiles("");
                QString compilers("");
                while (fileIt.hasNext())
                {
                    fileIt.next();
                    if (fileIt.fileInfo().isReadable())
                    {
                        prof = fileIt.fileInfo().filePath();
                        QSettings profile(prof, QSettings::IniFormat);
                        if (QSettings::NoError == info.status())
                        {
                            profile.beginGroup("info");
                            QString str = profile.value("compiler", "").toString();
                            profile.endGroup();

                            if (!str.isEmpty())
                            {
                                compilers = compilers + str + " ";
                                profiles = profiles + fileIt.fileInfo().filePath() + " ";
                            }
                            else continue;
                        }
                    }
                }
                if (!compilers.isEmpty())
                {
                supportedCompilers << compilers;
                profilesPathList << profiles;
                }
                else continue;
            }
        }
    }
}

QString Compiler::getProfilePath(QString lang, QString profile)
{
    QStringList profiles;
    QStringList locations;

    int index = supportedLanguages.indexOf(lang);
    if (-1 == index) return QString::Null();

    QString str = supportedCompilers.at(index);
    profiles = str.split(" ");
    profiles.removeAll("");

    str = profilesPathList.at(index);
    locations = str.split(" ");
    locations.removeAll("");

    index = profiles.indexOf(profile);
    if (-1 == index) return QString::Null();

    return locations.at(index);
}

QStringList Compiler::getSupportedLanguages()
{
    refreshSupported();
    return supportedLanguages;
}

QString Compiler::getSupportedExtensions(QString lang)
{
    if (!supportedLanguages.contains(lang)) return QString::null;

    int index = supportedLanguages.indexOf(lang);

    if (-1 == index) return QString("--");
    else return supportedExtensions.at(index);
}

QStringList Compiler::getSupportedCompilers(QString lang)
{
    QStringList supported;

    if (!supportedLanguages.contains(lang)) return QStringList();
    int index = supportedLanguages.indexOf(lang);

    if (-1 == index) return QStringList();
    QString str = supportedCompilers.at(index);

    supported = str.split(" ");
    supported.removeAll("");

    return supported;
}

QString Compiler::getCompilerInfo(QString lang, QString profile)
{
    if (lang.isEmpty() || profile.isEmpty()) return "";

    QString profPath = getProfilePath(lang, profile);
    if (QString::Null() == profPath) return "";

    QSettings prof(profPath, QSettings::IniFormat);
    prof.beginGroup("info");
    QString info = prof.value("comment", "").toString();
    prof.endGroup();

    return info;
}

void Compiler::loadProfile(QString lang, QString profile)
{
    if (NULL != compilerProfile)
    {
        free(compilerProfile);
    }

    if (lang.isEmpty() || profile.isEmpty()) return;

    refreshSupported();
    QString profPath = getProfilePath(lang, profile);
    if (QString::Null() == profPath) return;

    compilerProfile = new QSettings(profPath, QSettings::IniFormat);
    compileMode = DEFAULT;
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
    if (NULL == compilerProfile) return false;

    if (compilerProfile->status() == QSettings::NoError) return true;
    else return false;
}

bool Compiler::isModeAvailable(int compileMode)
{
    compilerProfile->beginGroup("compile");
    QString param;
    switch (compileMode)
    {
    case DEFAULT:
        param = compilerProfile->value("default","").toString();
        break;
    case ALTERNATIVE:
        param = compilerProfile->value("alternative").toString();
        break;
    case OBJECT:
        param = compilerProfile->value("object","").toString();
        break;
    case STATIC_LIB:
        param = compilerProfile->value("static_lib","").toString();
        break;
    case DYNAMIC_LIB:
        param = compilerProfile->value("dynamic_lib","").toString();
        break;
    default:
        param = "";
    }
    compilerProfile->endGroup();

    return !param.isEmpty();
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
    compilerProfile->beginGroup("info");
    QString compiler = compilerProfile->value("compiler", "").toString();
    compilerProfile->endGroup();

    if (compiler.isEmpty())
    {
        qDebug() << "FAIL: Could not find compiler.\n";
        return "";
    }

    return compiler;
}

QString Compiler::getCompilerParams()
{
    sourceFile = sourceFile.replace("/", QDir::separator());
    programPath = sourceFile.left(sourceFile.lastIndexOf('.'));
    sourcePath = sourceFile.left(sourceFile.lastIndexOf(QDir::separator()))+QDir::separator();

    qDebug() << "\nSOURCE FILE:" << sourceFile;
    qDebug() <<   "PROGRAM PATH:" << programPath;
    qDebug() <<   "SOURCE PATH:" << sourcePath;

    compilerProfile->beginGroup("compile");
    QString param;
    switch (compileMode)
    {
    case DEFAULT:
        param = compilerProfile->value("default","").toString();
        break;
    case ALTERNATIVE:
        param = compilerProfile->value("alternative").toString();
        break;
    case OBJECT:
        param = compilerProfile->value("object","").toString();
        break;
    case STATIC_LIB:
        param = compilerProfile->value("static_lib","").toString();
        break;
    case DYNAMIC_LIB:
        param = compilerProfile->value("dynamic_lib","").toString();
        break;
    default:
        param = "";
    }

    QString opt;

#ifdef WIN32
    opt = compilerProfile->value("win32_opt", "").toString();
#else
    opt = compilerProfile->value("unix_opt", "").toString();
#endif

    compilerProfile->endGroup();

    if (!param.isEmpty())
    {
        param.replace(QString("$source$"), sourceFile);
        param.replace(QString("$output$"), programPath);
        param.replace(QString("$options$"), opt+" "+options);
        param.replace(QString("$compilerdir$"), compilerDir);
    }
    else
    {
        qDebug() << "FAIL: Could not find compiler parameters.\n";
        return "";
    }

    return param;
}

QString Compiler::getConfig()
{
    compilerProfile->beginGroup("info");
    QString config = compilerProfile->value("config", "").toString();
    compilerProfile->endGroup();
    return config;
}

void Compiler::resetParseErrorList()
{
    QStringList keyList;
    parseErrorList.clear();
    compilerProfile->beginGroup("errors");
    keyList = compilerProfile->childKeys();
    foreach(QString key, keyList)
        parseErrorList << compilerProfile->value(key,"").toString();
    compilerProfile->endGroup();
    parseErrorList.removeAll("");
}

void Compiler::resetParseWarningList()
{
    QStringList keyList;
    parseWarningList.clear();
    compilerProfile->beginGroup("warnings");
    keyList = compilerProfile->childKeys();
    foreach(QString key, keyList)
        parseWarningList << compilerProfile->value(key,"").toString();
    compilerProfile->endGroup();
    parseErrorList.removeAll("");

}

void Compiler::compile()
{
    if (sourceFile.isEmpty()) return;

    errorList.clear();
    warningList.clear();
    outFile.clear();

    QString compiler = getCompilerName();
    QString config = getConfig();
    QString param = getCompilerParams();

    if (compiler.isEmpty() || param.isEmpty()) return;

    resetParseErrorList();
    resetParseWarningList();

#ifdef WIN32
    if (config.contains("outfile"))
    {
        outFile = sourcePath+"out.txt";
        param = param+" > "+outFile;
        qDebug() << "CONFIG:outfile";
    }
#endif

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

#ifdef Q_OS_WIN32
    QStringList command;
    command << "/C" << "title "+programPath+ "&&"+programPath;
    startDetached("cmd", command);
#endif /*Q_OS_WIN32*/

#ifdef Q_OS_UNIX
    QStringList params;

    startDetached(terminal, params << "-e" << "/bin/sh -c \'" + programPath + " && read -p \"Press enter to continue... \" REPLY'");
    //startDetached(QString("xterm") + QString(" -e /bin/sh -c \'") + programPath + QString(" && read -p \"Press enter to continue... \" REPLY'"));
#ifdef Q_OS_MAC
    startDetached("/usr/bin/open", QStringList() << "-n" << programPath);
//  startDetached(QString("xterm") + QString(" -e /bin/sh -c \'") + programPath + QString(" && read -p \"Press enter to continue... \" REPLY'"));
#endif /*Q_OS_MAC*/

#endif /*Q_OS_UNIX*/
    QDir::setCurrent(prevPath);
}

void Compiler::afterExit(int exitCode, QProcess::ExitStatus exitStatus)
{
    int endSt;

    if ((0 == exitCode)&&(0 == exitStatus)) endSt = STATUS_NOERROR;
    else
    {
        programPath = QString::null;
        endSt = STATUS_ERROR;
    }

    if (!outFile.isEmpty()) readStdErr();

    emit compileEnded(endSt);
}

void Compiler::compilerProcessError(QProcess::ProcessError error)
{
    int endSt;

    if (QProcess::FailedToStart == error) endSt = STATUS_FAILED_TO_START;
    else endSt = STATUS_CRASHED;

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

void Compiler::setTerminal(const QString &_terminal)
{
    terminal = _terminal;
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
