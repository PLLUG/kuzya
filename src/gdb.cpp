#include "gdb.h"

#include <QDebug>
#include <iostream>
#include <QRegExp>
#include <QFileInfo>

Gdb::Gdb()
{
}

Gdb::Gdb(QString gdbPath):
    mInfoCaptured{false},
    mWhatisCaptured{false},
    mPrintCaptured{false}
{
    mGdbFile.setFileName(gdbPath);
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadStdOutput()), Qt::UniqueConnection);
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(slotReadErrOutput()), Qt::UniqueConnection);
}

void Gdb::start(const QStringList &arguments, QIODevice::OpenMode mode)
{   //starts QPRocess, opens $mGdbFile.fileName()$ and passes $arguments$ as arguments
    QFileInfo checkFile(mGdbFile.fileName());
    if(!(checkFile.exists() && checkFile.isFile()))
    {
        QString message = tr("Gdb not found at %1").arg(mGdbFile.fileName());
        if(mGdbFile.fileName().isEmpty())
        {
            message = "Gdb is not configurated";
        }
        throw std::domain_error(message.toStdString());
    }
    QProcess::start(mGdbFile.fileName(), arguments, mode);
}

void Gdb::write(QByteArray command)
{   //wrtie command to GDB. You shouldn't pass command with '\n' It will appended here.
    QByteArray enter("\n");
    command.append(enter);
    int writeResult = QProcess::write(command);
    if(writeResult == -1)
    {
        emit signalErrorOccured(tr("Error while writing to GDB. Command didn't write"));
    }
}

void Gdb::readStdOutput()
{   //Reads all standart output from GDB
    mBuffer = QProcess::readAll();

    checkBreakpoint();

    checkPrintCommand();

    checkWhatIsCommand();

    checkInfoCommand();

    checkError();

    emit signalReadyReadGdb();
}

void Gdb::readType(const QString &varName)
{
    QRegExp findType("type\\s\\=\\s[\\w:\\*\\s\\<\\>\\,]+"); // find string after 'type = ' included only characters,
                                                 // digits, uderscores, '*' and whitespaces
    QRegExp findName("whatis\\s[\\w.)(*]+");
    int pos = -1;
    do
    {
        findName.indexIn(varName, pos+1);
        pos = findType.indexIn(varName, pos+1);
        if(pos != -1)
        {
            QString type = findType.cap();
            QString bareType = type.split('=')[1].trimmed(); // type is string after 'type = '
            QString nameStr = findName.cap();
            QString bareName = nameStr.split(' ')[1].trimmed();
            auto var = find_if(mVariableTypeQueue.begin(), mVariableTypeQueue.end(), [&](Variable var){return var.getName() == bareName;});
            if(var == mVariableTypeQueue.end())
            {
                return;
            }
            var->setType(bareType);
            emit signalTypeUpdated(*var);
            mVariableTypeQueue.erase(var);
        }
    }
    while(pos != -1);
}

void Gdb::readContent(const QString &context)
{
    QRegExp findName("print\\s[\\w.*)(]+");
    QRegExp content("=.*\\^done"); // match string beginning with '= ' and ending with '^done'
    QRegExp clean("[\\\\\"|~]"); // find all garbage characters '\', '"', '~'

    findName.indexIn(context);
    QString badName = findName.cap();
    QString bareName = badName.split(' ')[1].trimmed();

    content.indexIn(context);
    QString bareRes = content.cap();
    QString firstMatches = bareRes.split(QString("^done")).first();
    QString res = firstMatches.replace(clean, "").replace("^done", "").trimmed();
    res.resize(res.size()-1); // last character is garbate too

    /* Removed all line breaks */
    auto lst = res.split('\n');
    for(QString& i : lst)
    {
      i = i.trimmed();
    }
    QString withoutLines = lst.join(""); // complete one QString again
    withoutLines.remove(0, 2); // first two charactes are garbage too '= '
    emit signalContentUpdated(Variable(bareName, "", withoutLines));
}

QString Gdb::getVarContentFromContext(const QString &context)
{   // Produces variable value by GDB output
    QRegExp content("=\\s.*\\^done"); // match string beginning with '= ' and ending with '^done'
    QRegExp clean("[\\\\\"|~]"); // find all garbage characters '\', '"', '~'
    QRegExp pointerMatch("\\(.*\\s*\\)\\s0x[\\d+abcdef]+"); // try to regonize pointer content
                                                            // (SOME_TYPE *) 0x6743hf2
    if(pointerMatch.indexIn(context) != -1)
    {
        QString addres = pointerMatch.cap().split(' ').last();  // get only hex addres
        return addres;
    }
    content.indexIn(context);
    QString res = content.cap().replace(clean, "").replace("^done", "").trimmed();
    /* Removed all line breaks */
    auto lst = res.split('\n');
    for(QString& i : lst)
    {
        i = i.trimmed();
    }
    QString withoutLines = lst.join(""); // complete one QString again
    withoutLines.remove(0, 2); // first two charactes are garbage too '= '
    return withoutLines;
}

void Gdb::updateVariableFromBuffer()
{   // Read variables from mBuffer with their content
    QRegExp clean("~|\"|\\s|=");// find all garbage character

    QStringList vars = mBuffer.split("\\n");
    for(auto i : vars)
    {
        QStringList splittedBuffer = i.split(" = ");
        QString value;
        for(int i=1;i<splittedBuffer.size();++i)
        {
            value.append(splittedBuffer[i]);
            if(i+1 != splittedBuffer.size())
            {
                value.append(" = ");
            }
        }
        value = value.prepend(" = ");
        value = value.append("^done");
        QString content = getVarContentFromContext(value);
        if(!content.isEmpty())
        {
            mVariablesList.emplace_back(splittedBuffer[0].replace(clean, ""), "", content);
        }
    }
}

void Gdb::readErrOutput()
{
    mBuffer = QProcess::readAllStandardError();
}

void Gdb::checkBreakpoint()
{
    QRegExp exit("exit-code=\"\\d+\"");
    QRegExp stop("\\*stopped,reason="); // match stops
    QRegExp line("line=\"\\d+\""); // match line='$_digits_$'
    if(stop.indexIn(mBuffer) != -1)
    {   // if GDB matches breakpoint
        line.indexIn(mBuffer);
        QString lineStr = line.cap(); // line="123"
        if(lineStr.isEmpty())
        {
            exit.indexIn(mBuffer);
            QString codeStr = exit.cap();
            codeStr = codeStr.replace("[\\w\"=]+", QString());
            emit signalDebugEnded(codeStr.toInt());
            return;
        }
        int firstQuote = lineStr.indexOf(tr("\""));
        int lastQuote = lineStr.indexOf("\"", firstQuote+1);
        QString bareLine = lineStr.mid(firstQuote+1, lastQuote-firstQuote-1);
        emit signalGdbStopped(bareLine.toInt());
    }
}

void Gdb::checkPrintCommand()
{
    QRegExp doneOrError("\\^done|\\^error"); // match '^done' or '^error' literally
    QRegExp printRegex("print\\s"); // match 'print ' literally
    /*print capturing section*/
    if(printRegex.indexIn(mBuffer) != -1 || mPrintCaptured)
    {
        mPrintCaptured = true;
        mPrintBuffer.append(mBuffer);
    }
    if(mPrintCaptured && doneOrError.indexIn(mBuffer) != -1)
    {
        readContent(mPrintBuffer);
        mPrintCaptured = false;
        mPrintBuffer.clear();
    }
}

void Gdb::checkWhatIsCommand()
{
    QRegExp whatis("whatis\\s"); // match 'whatis ' literally
    QRegExp doneOrError("\\^done|\\^error"); // match '^done' or '^error' literally
    /*whatis capturing section*/
    if(whatis.indexIn(mBuffer) != -1 || mWhatisCaptured)
    { // is 'whatis' appears in output
        mWhatisBuffer.append(mBuffer);
        mWhatisCaptured = true;
    }
    int lastDoneorError = mWhatisBuffer.lastIndexOf("^done");
    int lastWhatis = mWhatisBuffer.lastIndexOf("whatis");
    if(doneOrError.indexIn(mBuffer) != -1 && mWhatisCaptured
            && lastWhatis < lastDoneorError) // if 'whatis' does not appear after ^done again
    {
        mWhatisCaptured = false;
        readType(mWhatisBuffer);
        mWhatisBuffer.clear();
    }
}

void Gdb::checkInfoCommand()
{
    QRegExp info("info\\s"); // match '^info ' literally
    QRegExp doneOrError("\\^done|\\^error"); // match '^done' or '^error' literally
    /*info capturing section*/
    if(info.indexIn(mBuffer) != -1 || mInfoCaptured)
    {
        mInfoCaptured = true;
        updateVariableFromBuffer();
    }
    if(doneOrError.indexIn(mBuffer) != -1 && mInfoCaptured)
    {
        mInfoCaptured = false;
        emit signalUpdatedVariables();
    }
}

void Gdb::checkError()
{
    QRegExp errorMatch("\\^error"); // match '^error' literally
    /*error capturing section*/
    if(errorMatch.indexIn(mBuffer) != -1)
    {
        QRegExp errorMsg("msg=.*\\(gdb\\)");
        int pos = -1;
        do
        {
            pos = errorMsg.indexIn(mBuffer, pos+1);
            mErrorMessage = errorMsg.cap();
            mErrorMessage.replace(tr("msg=\""), "");
            mErrorMessage.replace(tr("(gdb)"), "");
            mErrorMessage.remove(mErrorMessage.length()-2, 1);
            emit signalErrorOccured(mErrorMessage);
        }
        while(pos != -1);
    }
}

const QString &Gdb::getOutput() const
{   //Retrun mBuffer output
    return mBuffer;
}

void Gdb::openProject(const QString &fileName)
{   //opens file $fileName$ in gdb to debug it via target exec and file
    write(QByteArray("target exec ").append(fileName));
    write(QByteArray("file ").append(fileName));
    write(QByteArray("set new-console on"));
}

void Gdb::run()
{   //run debugging
    write(QByteArray("run"));
}

void Gdb::stepOver()
{   //goes to the next line of code
    write(QByteArray("next"));
}

void Gdb::setBreakPoint(unsigned int line)
{   //set simple breakpoint at line $line$
    write(QByteArray("b ").append(QString::number(line)));
}

void Gdb::clearBreakPoint(unsigned int line)
{   //clear breakpoint at line $line$
    write(QByteArray("clear ").append(QString::number(line)));
}

void Gdb::stepIn()
{   //step into function under cursor
    write(QByteArray("step"));
}

void Gdb::stepOut()
{   //step out of current function\method
    write(QByteArray("finish"));
}

void Gdb::stopExecuting()
{   //stop executing of target
    write(QByteArray("kill"));
}

void Gdb::stepContinue()
{   //continue normal executing to the next breakpoint or end of the programm
    write(QByteArray("c"));
}

int Gdb::getCurrentLine()
{   //returns current line of code or -1 if any aerror occured
    /*
    ~"#0  main () at main.cpp:46\n"
    ~"46\t\tcout << \"Finished main\";\n"
    ^done
    */
    write(QByteArray("frame"));
    QProcess::waitForReadyRead();
    QRegExp rx(":\\d+"); //finds ':46'
    if(rx.indexIn(mBuffer) == -1)
    {
        return -1; //not found
    }
    QStringList lst = rx.capturedTexts(); //found :46
    QString line = lst[0];// first matches
    return line.split(':').last().toInt();// (int)"46"
}

void Gdb::updateBreakpointsList()
{   //update std::vector<Breakpoint> mBreakpointsList with relevant info
    /*
        &"info b\n"
        ~"Num     Type           Disp Enb Address    What\n"

        ~"1       breakpoint     keep y   0x00401516 in main() at main.cpp:46\n"
        ~"\tbreakpoint already hit 1 time\n"
        ~"2       breakpoint     keep y   0x00401516 in main() at main.cpp:46\n"
        ^done
    */
    write(QByteArray("info b"));
    QProcess::waitForReadyRead(1000);
    QStringList lines = mBuffer.split('~'); // split by CLI output lines
    mBreakpointsList.clear(); // clear old info
    for(int i=2;i<lines.size();++i) //read all lines except of first two (command line and topic line)
    {
        Breakpoint currentBreakpoint;
        QString currentLine = lines[i];
        try
        {
            currentBreakpoint.parse(currentLine); // full breakpoint from $currentLine$
            mBreakpointsList.push_back(currentBreakpoint); // write relevant breakpoint to vector
        }
        catch(std::exception)
        {   //split breakpoint, if there are some error while processing $currentLine$
        }
    }
}

std::vector<Breakpoint> Gdb::getBreakpoints() const
{   //returns list of all breakpoint
    return mBreakpointsList;
}

std::vector<Variable> Gdb::getLocalVariables() const
{   //returns list of all variables
    return mVariablesList;
}

void Gdb::getVarContent(const QString& var)
{   // Asks GDB about variable $var$ and calls signal which will pass relevant info
    write(QByteArray("print ").append(var));
}

QString Gdb::getVarType(Variable var)
{   // Asks GDB about vairable $var$ and calls signal which will pass relevant info
    mVariableTypeQueue.push_back(var);
    write(QByteArray("whatis ").append(var.getName()));
    return QString();
}

void Gdb::updateVariable64x()
{   // Updates all variables
    mVariablesList.clear();
    write(QByteArray("info local"));
    write(QByteArray("info arg"));
}

void Gdb::setGdbPath(const QString &path)
{
    mGdbFile.setFileName(path);
}

void Gdb::slotReadStdOutput()
{
    readStdOutput();
}

void Gdb::slotReadErrOutput()
{
    readErrOutput();
}
