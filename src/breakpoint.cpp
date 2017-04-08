#include "breakpoint.h"

#include <QStringList>
#include <QDebug>
#include <QRegExp>

void Breakpoint::parse(const QString &line)
{   // fill breakpoint from $line$
    /*
        $"1       breakpoint     keep y   0x0040149e in main() at main.cpp:40\n"$
        $"2       breakpoint     del y   0x004014fc in main() at main.cpp:41\n"$
    */
      QRegExp errorMatch("(keep|del)");
      if(errorMatch.indexIn(line) == -1)
      {
          throw std::exception("Format exception");
      }
      QRegExp disposition(" keep ");
      QRegExp enabled(" y ");
      QRegExp frame("\\s[:\\w]*\\([\\w\\s:,]*\\)\\s"); // find string between whitespaces
                                                       // contains from characters, ':', whitespaces and breackets
      QRegExp lineRegex(":\\d+");
      if(disposition.indexIn(line) == -1)
      {
          mDisposition = Disposition::Delete;
      }
      else
      {
          mDisposition = Disposition::Keep;
      }
      mEnabled = enabled.indexIn(line) == -1 ? false : true;
      lineRegex.indexIn(line);
      QString lineFrame = lineRegex.cap();
      mLine = lineFrame.split(':')[1].toInt();
      frame.indexIn(line);
      mWhat = frame.cap().trimmed();
}

Breakpoint::Breakpoint()
{
}

Breakpoint::Breakpoint(int line, QString what, bool enabled, Breakpoint::Disposition disposition):
    mLine(line),
    mWhat(what),
    mEnabled(enabled),
    mDisposition(disposition)
{
}

int Breakpoint::getLine() const
{
    return mLine;
}

QString Breakpoint::getFrame() const
{
    return mWhat;
}

bool Breakpoint::isEnabled() const
{
    return mEnabled;
}

Breakpoint::Disposition Breakpoint::getDisposition() const
{
    return mDisposition;
}
