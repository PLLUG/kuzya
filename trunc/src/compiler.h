/***************************************************************************
 *   Copyright (C) 2009 by Alex Chmykhalo                                  *
 *   alexchmykhalo@users.sourceforge.net                                   *
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

#ifndef COMPILER_H
#define COMPILER_H

#include <QProcess>

#include "replacedialog.h"

class QSettings;

class Compiler : public QProcess
{
	Q_OBJECT
public:
	enum endStatusEnum{NOERROR, ERROR};
	enum runStatusEnum{STOP ,COMP, RUN};
	enum compileModeEnum{DEFAULT,EXECUTIVE,OBJECT,LIB,ALTERNATIVE};

	struct compilerError
	{
		int line;
		QString description;
	};

	Compiler(QObject *parent = 0);
        ~Compiler();

	void loadProfile(QString profile);
	void setOptions(QString str);
	void setCompilerDir(QString dir);
	void setLibDir(QString dir);
	void setIncludeDir(QString dir);
	bool isReady();
	bool isModeAvailable(int compileMode);
	void compile(QString sourceFile,int compileMode = DEFAULT);
	void run(void);
	QList<compilerError>* getLastErrors(void);

signals:
        void compileEnded(int status);
        void runEnded(int status);

private slots:
	void afterExit(int exitCode, QProcess::ExitStatus exitStatus);
	void readStdErr(void);

private:
	QString programPath;
	int runStatus;
	QList<compilerError> errorList;
        QString compiler;
	QString compilerDir;
	QString includeDir;
	QString libDir;
	QString options;
        QSettings* compilerProfile;
	QStringList parseParamList;
};

#endif

