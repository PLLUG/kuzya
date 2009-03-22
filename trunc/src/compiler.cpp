/***************************************************************************
 *   Copyright (C) 2009 by LLUG_DEV Programmers Group                      *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "QDebug"

#include "compiler.h"
#include <QSettings>

Compiler::Compiler(QObject *parent) : QProcess(parent)
{
	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(afterExit(int, QProcess::ExitStatus)));
	connect(this, SIGNAL(readyReadStandardError()), this, SLOT(readStdErr()));

	runStatus = STOP;
	compilerProfile = NULL;
}

Compiler::~Compiler()
{
	if (compilerProfile!=NULL) free(compilerProfile);
}

void Compiler::loadProfile(QString profile)
{
	if (compilerProfile!=NULL) 
	{
		free(compilerProfile);
	}
	
	compilerProfile = new QSettings(profile, QSettings::IniFormat);
}

void Compiler::setOptions(QString str)
{
	options = str;
}

void Compiler::setCompilerDir(QString dir)
{
	compilerDir = dir;
}

void Compiler::setLibDir(QString dir)
{
	libDir = dir;
}

void Compiler::setIncludeDir(QString dir)
{
	includeDir = dir;
}

bool Compiler::isReady()
{
	if (compilerProfile->status() == QSettings::NoError && runStatus == STOP) return true;
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
//		case EXECUTIVE:
//			param = compilerProfile->value("executive","").toString();
//			break;
//		case OBJECT:
//			param = compilerProfile->value("object","").toString();
//			break;
//		case LIB:
//			param = compilerProfile->value("lib","").toString();
//			break;
		case ALTERNATIVE:
			param = compilerProfile->value("alternative").toString();
			break;
		default:
			param = "";
	}
	compilerProfile->endGroup();
	return !param.isEmpty();
}

void Compiler::compile(QString sourceFile,int compileMode)
{
	if (sourceFile.isEmpty()) return;
	runStatus = COMP;
	errorList.clear();

	programPath = sourceFile.left(sourceFile.lastIndexOf('.'));

	compilerProfile->beginGroup("info");
	QString compiler = compilerProfile->value("compiler","").toString();	
	compilerProfile->endGroup();

	compilerProfile->beginGroup("compile");
	QString param;
	switch (compileMode) 
	{
		case DEFAULT:
			param = compilerProfile->value("default","").toString();
			break;
//		case EXECUTIVE:
//			param = compilerProfile->value("executive","").toString();
//			break;
//		case OBJECT:
//			param = compilerProfile->value("object","").toString();
//			break;
//		case LIB:
//			param = compilerProfile->value("lib","").toString();
//			break;
                case ALTERNATIVE:
			param = compilerProfile->value("alternative","").toString();
			break;
		default:
			param = "";
	}
	compilerProfile->endGroup();

	if (param.isEmpty() || compiler.isEmpty())
	{
		runStatus = STOP;
		return;
	}
	else
	{
		param.replace(QString("$source$"),sourceFile);
		param.replace(QString("$output$"),programPath);
		param.replace(QString("$options$"),options);	
		param.replace(QString("$libdir$"),libDir);	
		param.replace(QString("$compilerdir$"),compilerDir);	
		param.replace(QString("$includedir$"),includeDir);	
	}

	compilerProfile->beginGroup("parse");
	QStringList keyList = compilerProfile->childKeys();	
	foreach(QString key, keyList)
		parseParamList << compilerProfile->value(key,"").toString();
	compilerProfile->endGroup();

    qDebug() << QString(compilerDir+compiler+" "+param);

	start(QString(compilerDir+compiler+" "+param), QIODevice::ReadWrite);
}


void Compiler::run(void)
{
	if (programPath.isEmpty()) return;
    //runStatus = RUN;
#ifdef WIN32
    startDetached("cmd.exe", QStringList() << "/c " << programPath);
#else
    startDetached("xterm", QStringList() << "-e" << "/bin/sh" << "-c" << programPath);
    qDebug() << programPath;
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

        if (COMP == runStatus) emit compileEnded(endSt);
        else if (RUN == runStatus) emit runEnded(endSt);
	runStatus = STOP;
}


QList<Compiler::compilerError>* Compiler::getLastErrors(void)
{
	return &errorList;
}


void Compiler::readStdErr(void)
{
	
	if (COMP != runStatus) return;
	Compiler::compilerError ce;

	QByteArray result = readAllStandardError();
	QTextStream procStream(&result);
	QString line,pattern;
	int ln,desc;
	QRegExp errorRe;
	while (!procStream.atEnd())
	{
		line = procStream.readLine();
                qDebug() << line;
		foreach(QString parseParam, parseParamList)
		{
			pattern = parseParam.section(":",2);
			errorRe.setPattern(pattern);
			if (errorRe.indexIn(line) > -1)
			{
				ln = parseParam.section(":",0,0).toInt();
				ce.line = errorRe.cap(ln).toInt();
				desc = parseParam.section(":",1,1).toInt();
				ce.description = errorRe.cap(desc);
				errorList.append(ce);
				break;
			}
		}
	}
}


