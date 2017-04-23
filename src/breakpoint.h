#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <QString>

class Breakpoint
{
public:
    enum Disposition{Keep, Delete};
    void parse(const QString& line);// todo;
    Breakpoint();
    Breakpoint(int line, QString what, bool enabled, Disposition disposition);
    int getLine()const;
    QString getFrame()const;
    bool isEnabled()const;
    Disposition getDisposition()const;
private:
    int mLine;
    QString mWhat;
    bool mEnabled;
    Disposition mDisposition;
};

#endif // BREAKPOINT_H
