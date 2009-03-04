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
#ifndef COMPILER_H
#define COMPILER_H

#include <QProcess>

/**
	@author Volodymyr Shevchyk
*/
class Compiler : public QProcess
{
	Q_OBJECT
public:
    Compiler(QObject *parent = 0);
    ~Compiler();
	enum endStatusEnum{NOERROR, ERROR};
	enum runStatusEnum{STOP ,COMP, RUN};
	struct compilerError
	{
		int line;
		QString description;
	};
	void compile(QString path);
	void run(void);
	QList<compilerError>* gerLastErrors(void);

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
//	QString pathToSource;
};

#endif

