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

#ifndef COMPILER_H
#define COMPILER_H

#include <QProcess>

#include "replacedialog.h"

class QSettings;
class CompilerSettings;

class Compiler : public QProcess
{
	Q_OBJECT
public:
        enum endStatusEnum{NOERROR, ERROR, FAILED_TO_START, CRASHED};
        enum compileModeEnum{DEFAULT, ALTERNATIVE, OBJECT, STATIC_LIB, DYNAMIC_LIB};

	struct compilerError
	{
		int line;
		QString description;
	};

        typedef compilerError compilerWarning;

        Compiler(QObject *parent = 0);
        ~Compiler();

        QStringList getSupportedLanguages();
        QString getSupportedExtensions(QString lang);
        QStringList getSupportedCompilers(QString lang);
        QString getCompilerInfo(QString lang, QString profile);
        void loadProfile(QString lang, QString profile);
        void setOptions(QString str);
        void setCompilerDir(QString dir);
        void setCompilerMode(int mode);
        bool isReady(void);
        bool isModeAvailable(int compileMode);
        void openFile(QString srcPath);
        void compile();
        void run(void);
        QList<compilerError>* getLastErrors(void);
        QList<compilerWarning>* getLastWarnings(void);

signals:
        void compileEnded(int status);

private slots:
        void afterExit(int exitCode, QProcess::ExitStatus exitStatus);
        void compilerProcessError(QProcess::ProcessError error);
        void readStdErr(void);

private:
        void refreshSupported();
        QString getProfilePath(QString lang, QString profile);
        void resetParseErrorList();
        void resetParseWarningList();
        QString getCompilerParams();

        QString sourceFile;
        QString sourcePath;
        QString programPath;
        int runStatus;
        int compileMode;
        QList<compilerError> errorList;
        QList<compilerWarning> warningList;
        QString compilerDir;
        QString options;
        QSettings* compilerProfile;
        QStringList parseErrorList;
        QStringList parseWarningList;
        QStringList supportedLanguages;
        QStringList supportedExtensions;
        QStringList profileLocations;
        QStringList supportedCompilers;
        QStringList profilesPathList;
        QString outFile;

        CompilerSettings *settings;
};

#endif

