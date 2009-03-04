/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk   *
 *   i'mnotageycom.ua   *
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


#ifndef KUZYA_H
#define KUZYA_H

#include <QMainWindow>
#include <QProcess>

#include "ui_kuzya.h"
#include "compiler.h"

class QsciScintilla;
class GoToLineDialog;
class QLabel;


class Kuzya: public QMainWindow, private Ui::kuzyaForm
{
    Q_OBJECT

public:
    Kuzya(QWidget *parent = 0);
    ~Kuzya();

protected:

private slots:
	void slotUpdateStatusLabel(int line, int pos);
	void slotUpdateWindowName(bool m);
	void slotNew(void);
	void slotOpen(void);
	void slotSave(void);
	void slotSave_as(void);
	void slotRun(void);
	void slotCompile(void);
	void slotAbout(void);
	void slotAfterCompile(int status);
	void slotMarginClicked(int margin, int line, Qt::KeyboardModifiers state);
	void slotVoid(void);
	void slotChar(void);
	void slotWchar_t(void);
	void slotShort(void);
	void slotInt(void);
	void slotDouble(void);
	void slotFloat(void);	
	void slotLong(void);
	void slotBool(void);
	void slotSigned(void);
	void slotUnsigned(void);
	void slotConst(void);
	void slotTypedef(void);
	void slotClass(void);
	void slotStruct(void);
	void slotEnum(void);	
	void slotUnion(void);
	void slotNewMemory(void);	
	void slotDelete(void);
	void slotConst_cast(void);
	void slotReinterpret_cast(void);	
	void slotStatic_cast(void);	
	void slotAnd(void);
	void slotAnd_eq(void);
	void slotBitand(void);
	void slotBitor(void);
	void slotCompl(void);	
	void slotNot(void);
	void slotNot_eq(void);	
	void slotOr(void);
	void slotOr_eq(void);
	void slotXor(void);	
	void slotXor_eq(void);	
	void slotTrue(void);	
	void slotFalse(void);	
	void slotPrivate(void);
	void slotProtected(void);	
	void slotPublic(void);
	void slotFriend(void);
	void slotVirtual(void);	
	void slotExtern(void);	
	void slotInline(void);	
	void slotStatic(void);	
	void slotCase(void);	
	void slotBreak(void);	
	void slotContinue(void);	
	void slotDo_while(void);
	void slotElse(void);	
	void slotFor(void);
	void slotGoto(void);
	void slotIf(void);	
	void slotReturn(void);	
	void slotSwitch(void);	
	void slotWhile(void);	
	void slotTry(void);	
	void slotCatch(void);	
	void slotThrow(void);	


private:
	void paintErrorMarkers(QList<Compiler::compilerError>* errorList);

private:
	QsciScintilla* textEditor;
	QString fileName;
	GoToLineDialog* goToLine;
	Compiler *compiler;
	QLabel* statusLabel;
	int errorMarker;
	QMap<int, QString> errorMap;
};

#endif

