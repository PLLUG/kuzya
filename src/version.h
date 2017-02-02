#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QDebug>
#include <QFile>

//class for version of build
class KuziaVersion
{
private:
    int majorver;
    int minorver;
    int revision;
    QString total;
public:
    KuziaVersion() : majorver(0), minorver(0), revision(0), total("\0") {}

    void setVersion(int _majorver, int _minorver, int _revision)
    {
        majorver = _majorver;
        minorver = _minorver;
        revision = _revision;

        total = QString::number(majorver) + "." + QString::number(minorver) + "." + QString::number(revision);
    }

    QString getVersion() const
    {
        return total;
    }
};

//class for time of build
class BuildTime
{
private:
    int mount;
    int day;
    int year;
    QString total;
public:
    BuildTime() : mount(0), day(0), year(0), total("\0") {}

    void setTime(int _mount, int _day, int _year)
    {
        mount = _mount;
        day = _day;
        year = _year;

//Member day and year
        QString temp = QString::number(day);
        temp += ' ';
        temp += QString::number(year);

//Switch month and add membered day and year
        switch (mount)
        {
            case 1:
                total += "January "; total += temp;
                break;
            case 2:
                total += "February "; total += temp;
                break;
            case 3:
                total += "March "; total += temp;
                break;
            case 4:
                total += "April "; total += temp;
                break;
            case 5:
                total += "May "; total += temp;
                break;
            case 6:
                total += "June "; total += temp;
                break;
            case 7:
                total += "July "; total += temp;
                break;
            case 8:
                total += "August "; total += temp;
                break;
            case 9:
                total += "September "; total += temp;
                break;
            case 10:
                total += "October "; total += temp;
                break;
            case 11:
                total += "November "; total += temp;
                break;
            case 12:
                total += "December "; total += temp;
                break;
            default:
                break;
        }
    }

    QString getTime() const
    {
        return total;
    }
};

void setAll(BuildTime &buidltime, KuziaVersion &kuziaversion)
{

    const QString fileName = ":/BUILD.DAT";
    QFile file(fileName);

//Checking file
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
          qCritical() << "File does not open, maybe file doesn't exist!";
          return;
        }

//Member data from file
    const int SIZE = 6;
    QString dataS[SIZE];
    int i = 0;
    while(!file.atEnd())
    {
      dataS[i++] = file.readLine();
    }

//Transform file data from QString to int
    int dataI[SIZE];
    for(int i =0; i < SIZE; i++)
      dataI[i] = dataS[i].toInt();

    buidltime.setTime(dataI[0], dataI[1], dataI[2]);
    kuziaversion.setVersion(dataI[3], dataI[4], dataI[5]);

    file.close();
}

#endif // VERSION_H

//#define MAJORVER 2
//#define MINORVER 1
//#define REVISION 12
//#define MINOR_REVISION 0
//#define BETA 0
//#define BUILD_TIME "15.02.2012"
//#define KUZYA_VERSION "2.1.12"
