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


#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

#include "gotolinedialog.h"
#include "compiler.h"
#include "kuzya.h"


Kuzya::Kuzya(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	setObjectName("Kuzya");
	setWindowTitle("Kuzya");

	toolBar->addAction(actionNew);
	toolBar->addAction(actionOpen);
	toolBar->addAction(actionSave);
	toolBar->addSeparator();
	toolBar->addAction(actionUndo);
	toolBar->addAction(actionRedo);
	toolBar->addAction(actionCut);
	toolBar->addAction(actionCopy);
	toolBar->addAction(actionPaste);
	toolBar->addSeparator();
	toolBar->addAction(actionRun);
	toolBar->addAction(actionCompile);

	statusLabel = new QLabel(this);
	statusBar()->addPermanentWidget(statusLabel);

	QGridLayout *gridLayout;
	gridLayout = new QGridLayout(centralwidget);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	textEditor = new QsciScintilla(this);
	gridLayout->addWidget(textEditor, 0, 0, 1, 1);

	textEditor->setCaretLineBackgroundColor(QColor(200, 200, 200));
	textEditor->setCaretLineVisible(true);

	QsciLexerCPP *cppLexer = new QsciLexerCPP(this);
	textEditor->setLexer(cppLexer);

	textEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
	textEditor->setMatchedBraceBackgroundColor(QColor(200, 100, 100));

	textEditor->setSelectionBackgroundColor(QColor(100, 100, 200));

	textEditor->setMarginSensitivity(1, true);
	textEditor->setMarginLineNumbers(1, true);
	textEditor->setMarginWidth(1, 30);

	goToLine = new GoToLineDialog(textEditor);

	compiler = new Compiler(this);

	errorMarker = textEditor->markerDefine(QsciScintilla::CircledPlus);

	connect(actionNew,		SIGNAL(triggered()),this,		SLOT(slotNew()));
	connect(actionOpen, 	SIGNAL(triggered()),this,		SLOT(slotOpen()));
	connect(actionSave,		SIGNAL(triggered()),this,		SLOT(slotSave()));
	connect(actionSave_as,	SIGNAL(triggered()),this,		SLOT(slotSave_as()));
	connect(actionExit, 	SIGNAL(triggered()), this,		SLOT(close()));
	connect(actionUndo, 	SIGNAL(triggered()), textEditor,SLOT(undo()));
	connect(actionRedo, 	SIGNAL(triggered()), textEditor,SLOT(redo()));
	connect(actionCut, 		SIGNAL(triggered()), textEditor,SLOT(cut()));
	connect(actionCopy, 	SIGNAL(triggered()), textEditor,SLOT(copy()));
	connect(actionPaste,	SIGNAL(triggered()), textEditor,SLOT(paste()));
	connect(actionGoToLine, SIGNAL(triggered()), goToLine, 	SLOT(slotGoToLine()));
	connect(actionGoToMatchingBracket, SIGNAL(triggered()),textEditor ,SLOT(moveToMatchingBrace()));
	connect(actionCompile, 	SIGNAL(triggered()), this, 		SLOT(slotCompile()));
	connect(actionRun, 		SIGNAL(triggered()),this,		SLOT(slotRun()));
	connect(actionAbout, 	SIGNAL(triggered()),this,		SLOT(slotAbout()));

	connect(actionVoid, 	SIGNAL(triggered()),this,		SLOT(slotVoid()));
	connect(actionChar, 	SIGNAL(triggered()),this,		SLOT(slotChar()));
	connect(actionWchar_t, 	SIGNAL(triggered()),this,		SLOT(slotWchar_t()));
	connect(actionShort, 	SIGNAL(triggered()),this,		SLOT(slotShort()));
	connect(actionInt, 		SIGNAL(triggered()),this,		SLOT(slotInt()));
	connect(actionDouble, 	SIGNAL(triggered()),this,		SLOT(slotDouble()));
	connect(actionFloat, 	SIGNAL(triggered()),this,		SLOT(slotFloat()));
	connect(actionLong, 	SIGNAL(triggered()),this,		SLOT(slotLong()));
	connect(actionBool, 	SIGNAL(triggered()),this,		SLOT(slotBool()));
	connect(actionSigned, 	SIGNAL(triggered()),this,		SLOT(slotSigned()));
	connect(actionUnsigned, SIGNAL(triggered()),this,		SLOT(slotUnsigned()));
	connect(actionConst, 	SIGNAL(triggered()),this,		SLOT(slotConst()));
	connect(actionTypedef, 	SIGNAL(triggered()),this,		SLOT(slotTypedef()));
	connect(actionClass, 	SIGNAL(triggered()),this,		SLOT(slotClass()));
	connect(actionStruct, 	SIGNAL(triggered()),this,		SLOT(slotStruct()));
	connect(actionEnum,		SIGNAL(triggered()),this,		SLOT(slotEnum()));
	connect(actionUnion, 	SIGNAL(triggered()),this,		SLOT(slotUnion()));
	connect(actionNewMemory,SIGNAL(triggered()),this,		SLOT(slotNewMemory()));
	connect(actionDelete, 	SIGNAL(triggered()),this,		SLOT(slotDelete()));
	connect(actionConst_cast,SIGNAL(triggered()),this,		SLOT(slotConst_cast()));
	connect(actionReinterpret_cast,SIGNAL(triggered()),this,SLOT(slotReinterpret_cast()));
	connect(actionStatic_cast,SIGNAL(triggered()),this,		SLOT(slotStatic_cast()));
	connect(actionAnd, 		SIGNAL(triggered()),this,		SLOT(slotAnd()));
	connect(actionAnd_eq, 	SIGNAL(triggered()),this,		SLOT(slotAnd_eq()));
	connect(actionBitand, 	SIGNAL(triggered()),this,		SLOT(slotBitand()));
	connect(actionBitor, 	SIGNAL(triggered()),this,		SLOT(slotBitor()));
	connect(actionCompl,	SIGNAL(triggered()),this,		SLOT(slotCompl()));
	connect(actionNot, 		SIGNAL(triggered()),this,		SLOT(slotNot()));
	connect(actionNot_eq,	SIGNAL(triggered()),this,		SLOT(slotNot_eq()));
	connect(actionOr, 		SIGNAL(triggered()),this,		SLOT(slotOr()));
	connect(actionOr_eq,	SIGNAL(triggered()),this,		SLOT(slotOr_eq()));
	connect(actionXor,		SIGNAL(triggered()),this,		SLOT(slotXor()));
	connect(actionXor_eq,	SIGNAL(triggered()),this,		SLOT(slotXor_eq()));
	connect(actionTrue,		SIGNAL(triggered()),this,		SLOT(slotTrue()));
	connect(actionFalse,	SIGNAL(triggered()),this,		SLOT(slotFalse()));
	connect(actionPrivate,	SIGNAL(triggered()),this,		SLOT(slotPrivate()));
	connect(actionProtected,SIGNAL(triggered()),this,		SLOT(slotProtected()));
	connect(actionPublic, 	SIGNAL(triggered()),this,		SLOT(slotPublic()));
	connect(actionFriend,	SIGNAL(triggered()),this,		SLOT(slotFriend()));
	connect(actionVirtual,	SIGNAL(triggered()),this,		SLOT(slotVirtual()));
	connect(actionExtern,	SIGNAL(triggered()),this,		SLOT(slotExtern()));
	connect(actionInline,	SIGNAL(triggered()),this,		SLOT(slotInline()));
	connect(actionStatic,	SIGNAL(triggered()),this,		SLOT(slotStatic()));
	connect(actionCase,		SIGNAL(triggered()),this,		SLOT(slotCase()));
	connect(actionBreak,	SIGNAL(triggered()),this,		SLOT(slotBreak()));
	connect(actionContinue,	SIGNAL(triggered()),this,		SLOT(slotContinue()));
	connect(actionDo_while,	SIGNAL(triggered()),this,		SLOT(slotDo_while()));
	connect(actionElse,		SIGNAL(triggered()),this,		SLOT(slotElse()));
	connect(actionFor, 		SIGNAL(triggered()),this,		SLOT(slotFor()));
	connect(actionGoto,		SIGNAL(triggered()),this,		SLOT(slotGoto()));
	connect(actionIf,		SIGNAL(triggered()),this,		SLOT(slotIf()));
	connect(actionReturn,	SIGNAL(triggered()),this,		SLOT(slotReturn()));
	connect(actionSwitch,	SIGNAL(triggered()),this,		SLOT(slotSwitch()));
	connect(actionWhile,	SIGNAL(triggered()),this,		SLOT(slotWhile()));
	connect(actionTry,		SIGNAL(triggered()),this,		SLOT(slotTry()));
	connect(actionCatch,	SIGNAL(triggered()),this,		SLOT(slotCatch()));
	connect(actionThrow,	SIGNAL(triggered()),this,		SLOT(slotThrow()));


	connect(textEditor, SIGNAL(cursorPositionChanged (int, int)), this,SLOT(slotUpdateStatusLabel(int, int)));
	connect(textEditor, SIGNAL(modificationChanged(bool)), this, SLOT(slotUpdateWindowName(bool)));
	connect(compiler, SIGNAL(compileEnded(int)), this, SLOT(slotAfterCompile(int)));
	connect(textEditor, SIGNAL(marginClicked (int, int, Qt::KeyboardModifiers)), this, SLOT(slotMarginClicked(int, int, Qt::KeyboardModifiers)));

	statusBar()->showMessage(tr("Ready"));
}

Kuzya::~Kuzya()
{
}

/**
*******************************************************************************************************
**/
void Kuzya::slotNew(void)
{
	textEditor->markerDeleteAll();

	fileName = QString::null;
	textEditor->clear();
	setWindowTitle("Kuzya");
	statusBar()->showMessage(tr("Created new file"), 2000);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotOpen(void)
{
	textEditor->markerDeleteAll();

	fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
	if (fileName.isEmpty()) return;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QTextStream stream(&file);
	textEditor->setText(stream.readAll());
	textEditor->setModified(false);
	file.close();
	statusBar()->showMessage(tr("Opened"), 2000);
	slotUpdateWindowName(false);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotSave(void)
{
	if (fileName.isEmpty())
	{
		fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                           QString(), tr("Source-Files (*.c *.cpp *.cxx *.h);;All Files (*)"));
		slotUpdateWindowName(false);
	}

	if (fileName.isEmpty()) return;

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

	QTextStream stream(&file);
	stream << textEditor->text();
	textEditor->setModified(false);
	file.close();
	statusBar()->showMessage(tr("Saved"), 2000);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotSave_as(void)
{
	fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                           QString(), tr("Source-Files (*.c *.cpp *.cxx *.h);;All Files (*)"));
	if (fileName.isEmpty()) return;
	slotSave();
}

/**
*******************************************************************************************************
**/
void Kuzya::slotRun(void)
{
	if (fileName.isEmpty())
	{
		statusBar()->showMessage(tr("No binary to run"), 2000);
		return;
	}
	compiler->run();
}

/**
*******************************************************************************************************
**/
void Kuzya::slotCompile(void)
{
	textEditor->markerDeleteAll();
	if (textEditor->isModified()) slotSave();
	if (fileName.isEmpty())
	{
		statusBar()->showMessage(tr("No source to compile"), 2000);
		return;
	}

	statusBar()->showMessage(tr("Compilling..."));
	textEditor->setReadOnly(true);
	compiler->compile(fileName);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAfterCompile(int status)
{
	textEditor->setReadOnly(false);
	if (Compiler::NOERROR == status) statusBar()->showMessage(tr("Compiled successfuly!"), 2000);
	else
	{
		statusBar()->showMessage(tr("Compilation failed!!!"), 2000);
		paintErrorMarkers(compiler->gerLastErrors());
	}
}

/**
*******************************************************************************************************
**/
void Kuzya::paintErrorMarkers(QList<Compiler::compilerError>* errorList)
{
	errorMap.clear();
	if (errorList->empty()) return;
	for (int i = 0; i < errorList->size(); ++i)
	{
		textEditor->markerAdd(errorList->at(i).line, errorMarker);
		errorMap[errorList->at(i).line] = errorList->at(i).description;
	}
	textEditor->setCursorPosition(errorList->at(0).line, 0);
	statusBar()->showMessage(tr("***ERROR: ") + errorList->at(0).description);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAbout(void)
{
	QMessageBox::about(this, tr("About"),tr("      Authors\n Volodymyr Shevchuk \n         && \n   Andriy Shevchyk "));
}

/**
*******************************************************************************************************
**/
void Kuzya::slotUpdateStatusLabel(int line, int pos)
{
	statusLabel->setText(QString(tr("Line: %1 Pos: %2")).arg(line + 1).arg(pos));
}

/**
*******************************************************************************************************
**/
void Kuzya::slotUpdateWindowName(bool m)
{
	if (!fileName.isEmpty())
	{
		if (m) setWindowTitle(fileName.section('/', -1) + " - " + fileName + tr(" [modified] - Kuzya"));
		else setWindowTitle(fileName.section('/', -1) + " - " + fileName + tr(" - Kuzya"));
	}
}

/**
*******************************************************************************************************
**/
void Kuzya::slotMarginClicked(int margin, int line, Qt::KeyboardModifiers state)
{
	if (1 == textEditor->markersAtLine(line)) statusBar()->showMessage(tr("***ERROR: ") + errorMap[line]);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotVoid()
{
	textEditor->insert("void ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotChar()
{
	textEditor->insert("char ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotWchar_t()
{
	textEditor->insert("wchar_t ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotShort()
{
	textEditor->insert("short ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotInt()
{
	textEditor->insert("int ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDouble()
{
	textEditor->insert("double ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFloat()
{
	textEditor->insert("float ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotLong()
{
	textEditor->insert("long ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBool()
{
	textEditor->insert("bool ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotSigned()
{
	textEditor->insert("signed ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotUnsigned()
{
	textEditor->insert("unsigned ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotConst()
{
	textEditor->insert("const ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTypedef()
{
	textEditor->insert("typedef ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotClass()
{
	textEditor->insert("class\n{\n};\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStruct()
{
	textEditor->insert("struct\n{\n};\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotEnum()
{
	textEditor->insert("enum\n{\n};\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotUnion()
{
	textEditor->insert("union\n{\n};\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNewMemory()
{
	textEditor->insert("new ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index +5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDelete()
{
	textEditor->insert("delete ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotConst_cast()
{
	textEditor->insert("const_cast ();\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 12);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotReinterpret_cast()
{
	textEditor->insert("reinterpret_cast ();\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 18);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStatic_cast()
{
	textEditor->insert("static_cast ();\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 13);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotAnd()
{
	textEditor->insert("and ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotAnd_eq()
{
	textEditor->insert("and_eq ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBitand()
{
	textEditor->insert("bitand ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBitor()
{
	textEditor->insert("bitor ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCompl()
{
	textEditor->insert("compl ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNot()
{
	textEditor->insert("not ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNot_eq()
{
	textEditor->insert("not_eq ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index +7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotOr()
{
	textEditor->insert("or ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 3);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotOr_eq()
{
	textEditor->insert("or_eq ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotXor()
{
	textEditor->insert("xor ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotXor_eq()
{
	textEditor->insert("xor_eq ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTrue()
{
	textEditor->insert("true ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFalse()
{
	textEditor->insert("false ");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotPrivate()
{
	textEditor->insert("private:\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotProtected()
{
	textEditor->insert("protected:\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 11);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotPublic()
{
	textEditor->insert("public:\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFriend()
{
	textEditor->insert("friend ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotVirtual()
{
	textEditor->insert("virtual ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotExtern()
{
	textEditor->insert("extern ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotInline()
{
	textEditor->insert("inline ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStatic()
{
	textEditor->insert("static ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCase()
{
	textEditor->insert("case : ;\n\tbreak;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBreak()
{
	textEditor->insert("break ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotContinue()
{
	textEditor->insert("continue ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDo_while()
{
	textEditor->insert("do\n{\n}\nwhile();\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 13);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotElse()
{
	textEditor->insert("else\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFor()
{
	textEditor->insert("for (; ; )\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotGoto()
{
	textEditor->insert("goto ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotIf()
{
	textEditor->insert("if ()\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotReturn()
{
	textEditor->insert("return ;\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotSwitch()
{
	textEditor->insert("switch ()\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotWhile()
{
	textEditor->insert("while ()\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTry()
{
	textEditor->insert("try\n{\n\t\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCatch()
{
	textEditor->insert("catch()\n{\n}\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotThrow()
{
	textEditor->insert("throw ();\n");
	int line, index;
	textEditor->getCursorPosition(&line, &index);
	textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
