#ifndef SLEEP_H
#define SLEEP_H

#include <QtCore/QThread>

class Sleep : public QThread
{
public:
static void msleep(unsigned long msecs)
{
    QThread::msleep(msecs);
}
};

#endif // SLEEP_H
