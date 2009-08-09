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

        QStringList getSupportedLanguages();
        QString getSupportedExtensions(QString lang);
        QStringList getSupportedCompilers(QString lang);
        QString getCompilerInfo(QString lang, QString profile);
        void loadProfile(QString lang, QString profile);
	void setOptions(QString str);
	void setCompilerDir(QString dir);
        bool isReady(void);
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
        void refreshSupported();

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
        QStringList supportedLanguages;
        QStringList supportedExtensions;
        QStringList profileLocations;
        QStringList supportedCompilers;
        QStringList profilesPathList;
};

#endif

