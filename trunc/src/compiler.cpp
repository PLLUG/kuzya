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


#include <QSettings>
#include <QDirIterator>
#include "QDebug"

#include "compiler.h"

Compiler::Compiler(QObject *parent) : QProcess(parent)
{  
	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(afterExit(int, QProcess::ExitStatus)));
        connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdErr()));
        connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilerProcessError(QProcess::ProcessError)));

        setProcessChannelMode(MergedChannels);
        refreshSupported();
	compilerProfile = NULL;
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
    QString path = "/usr/share/kuzya/profiles";
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
                supportedLanguages << info.value("language", "").toString();
                supportedExtensions << info.value("filter","").toString();
                info.endGroup();

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
                            QString str = profile.value("compiler", "").toString()+" ";
                            profile.endGroup();

                            if (str.isEmpty()) continue;
                            compilers = compilers + str + " ";
                            profiles = profiles + fileIt.fileInfo().filePath() + " ";
                        }
                    }
                }
                supportedCompilers << compilers;
                profilesPathList << profiles;
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

    if (-1 == index) return QString("");
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
        if (NULL!=compilerProfile)
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

void Compiler::compile(QString sourceFile)
{
	if (sourceFile.isEmpty()) return;
        errorList.clear();
        warningList.clear();
        outFile.clear();

        sourceFile = sourceFile.replace("/", QDir::separator());
        programPath = sourceFile.left(sourceFile.lastIndexOf('.'));
        QString sourcePath = sourceFile.left(sourceFile.lastIndexOf(QDir::separator()));
        qDebug() << sourceFile;
        qDebug() << programPath;
        qDebug() << sourcePath;

	compilerProfile->beginGroup("info");
        QString compiler = compilerProfile->value("compiler", "").toString();
        config = compilerProfile->value("config", "").toString();
	compilerProfile->endGroup();

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

        if (param.isEmpty() || compiler.isEmpty()) return;
	else
	{
                param.replace(QString("$source$"), sourceFile);
                param.replace(QString("$output$"), programPath);
                param.replace(QString("$options$"), options+" "+opt);
                param.replace(QString("$compilerdir$"), compilerDir);
	}

        QStringList keyList;

        parseErrorList.clear();
        compilerProfile->beginGroup("errors");
        keyList = compilerProfile->childKeys();
        foreach(QString key, keyList)
                parseErrorList << compilerProfile->value(key,"").toString();
        compilerProfile->endGroup();

        parseWarningList.clear();
        compilerProfile->beginGroup("warnings");
        keyList = compilerProfile->childKeys();
        foreach(QString key, keyList)
                parseWarningList << compilerProfile->value(key,"").toString();
        compilerProfile->endGroup();

        qDebug() << QString(compiler+" "+param);

#ifdef WIN32
        if (config.contains("nostd"))
        {
            outFile = QApplication::applicationDirPath()+QDir::separator()+"out.txt";
            outFile.replace("/", QDir::separator());
            param = param+" > "+outFile;
        }
#endif


        QString prevDir = QDir::currentPath();
        if (!compilerDir.isEmpty()) QDir::setCurrent(compilerDir);
        start(QString(compiler+" "+param), QIODevice::ReadWrite);
        qDebug() << QDir::currentPath();
        qDebug() << QString(compiler+" "+param);
        QDir::setCurrent(prevDir);
}


void Compiler::run(void)
{
    if (programPath.isEmpty()) return;
#ifdef WIN32
        startDetached("cmd", QStringList() << "/C"<< "title "+programPath+"&&("+programPath+")&pause");
#else
        startDetached("xterm", QStringList() << "-e" << "/bin/sh" << "-c"<< programPath);
#endif
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
        }
        QTextStream procStream(&result);
	QString line,pattern;
        QString parseParam;
	int ln,desc;
        QRegExp re;
	while (!procStream.atEnd())
	{
		line = procStream.readLine();
                qDebug() << line;
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


