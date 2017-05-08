#ifndef GDB_H
#define GDB_H

#include <QProcess>
#include <QFile>
#include <QStringList>

#include <vector>
#include <queue>
#include <list>

#include "breakpoint.h"
#include "variable.h"

class Gdb : public QProcess
{
    Q_OBJECT
public:
    Gdb();
    Gdb(QString gdbPath);
    void start(const QStringList &arguments = QStringList() << "--interpreter=mi",
                QProcess::OpenMode mode = QIODevice::ReadWrite);
    void write(QByteArray &command);
    void readStdOutput();
    void readErrOutput();

    const QString& getOutput()const;
    void openProject(const QString& fileName);
    int getCurrentLine();
    void updateBreakpointsList();
    std::vector<Breakpoint> getBreakpoints()const;
    std::vector<Variable> getLocalVariables()const;
    void getVarContent(const QString& var);
    QString getVarType(Variable var);
    void setGdbPath(const QString& path);

    void readType(const QString& varName);
    void readContent(const QString& context);
    void updateVariable64x();
    void updateVariableFromBuffer();
    QString getVarContentFromContext(const QString& context);

public slots:
    void run();
    void stepOver();
    void setBreakPoint(unsigned int line);
    void clearBreakPoint(unsigned int line);
    void stepIn();
    void stepOut();
    void stopExecuting();
    void stepContinue();
    void slotReadStdOutput();
    void slotReadErrOutput();

signals:
    void signalBreakpointHit(int line);
    void signalLocalVarRecieved(const QString&);
    void signalErrorOccured(const QString&);
    void signalUpdatedVariables();
    void signalTypeUpdated(Variable var);
    void signalContentUpdated(Variable var);
    void signalReadyReadGdb();
private:
    QFile mGdbFile;
    QString mErrorMessage;
    QString mBuffer;
    std::vector<Breakpoint> mBreakpointsList;
    std::vector<Variable> mVariablesList;


    QString temp;
    bool mInfoCaptured;
    bool mWhatisCaptured;
    QString mWhatisBuffer;
    QString mPrintBuffer;
    bool collect;
    std::list<Variable> mVariableTypeQueue;
    bool mPrintCaptured;
};

#endif // GDB_H
