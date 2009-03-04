/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk                              *
 *   i'mnotageycom.ua                                                      *
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

Compiler::Compiler(QObject *parent)
 : QProcess(parent)
{
	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(afterExit(int, QProcess::ExitStatus)));
	connect(this, SIGNAL(readyReadStandardError()), this, SLOT(readStdErr()));
	runStatus = STOP;
}

/**
*************************************************************************************************
**/
Compiler::~Compiler()
{
}

/**
*************************************************************************************************
**/
void Compiler::compile(QString path)
{
	if (path.isEmpty()) return;
	runStatus = COMP;
	errorList.clear();
	programPath = path.left(path.lastIndexOf('.'));
	start("g++", QStringList() << "-O2" << "-o" << programPath << path);
}

/**
*************************************************************************************************
**/
void Compiler::run(void)
{
	if (programPath.isEmpty()) return;
	runStatus = RUN;
	start("konsole", QStringList() << "-e" << "/bin/sh" << "-c" << programPath + ";read dummy");
}

/**
*************************************************************************************************
**/
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
	qDebug("Compiler:: process exited with exit code: %d and exit status: %d", exitCode, exitStatus);
}

/**
*************************************************************************************************
**/
QList<Compiler::compilerError>* Compiler::gerLastErrors(void)
{
	return &errorList;
}

/**
*************************************************************************************************
**/
void Compiler::readStdErr(void)
{
	
	if (COMP != runStatus) return;
	Compiler::compilerError ce;

	QByteArray result = readAllStandardError();
	QTextStream procStream(&result);
	QString line;
	while (!procStream.atEnd())
	{
		line = procStream.readLine();
		QRegExp errorRE("(\\d+):\\s(.*)error:\\s(.*)");
		QRegExp errorRE1("(\\d+):(\\d+):\\s(.*)error:\\s(.*)");
qDebug() << line;
		if (errorRE.indexIn(line) > -1)
		{
//			qDebug() << errorRE.cap(1) << errorRE.cap(3);
			ce.line = errorRE.cap(1).toInt();
			ce.description = errorRE.cap(3);
			errorList.append(ce);
		}
		else if(errorRE1.indexIn(line) > -1)
		{
			ce.line = errorRE1.cap(1).toInt();
			ce.description = errorRE1.cap(4);
			errorList.append(ce);
		}
	}
}


