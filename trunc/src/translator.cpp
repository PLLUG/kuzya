#include "translator.h"

#include <QDirIterator>
#include <QApplication>

#include <QDebug>

Translator::Translator(QObject *parent) : QObject(parent)
{
//    QStringList list = getSupportedTranslations("c++");

}

Translator::~Translator()
{
}

QStringList Translator::getSupportedTranslations(QString lang)
{
#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/profiles/"+lang;
#else
    QString path = "/usr/share/kuzya/profiles/"+lang;
#endif

    QDirIterator fileIt(path, QStringList()<< "*.tr", QDir::NoDotAndDotDot|QDir::Files, QDirIterator::NoIteratorFlags);
    QString translation;
    while (fileIt.hasNext())
    {
        fileIt.next();
        if (fileIt.fileInfo().isReadable())
        {
            translation = fileIt.fileInfo().fileName();
            translation.truncate(translation.lastIndexOf("."));
            supportedTranslations << translation;
        }
    }
    supportedTranslations.removeDuplicates();
    return supportedTranslations;
}

QString Translator::detectCodeLanguage(QString filePath, QString lang)
{
    QFile code(filePath);
    if(!code.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << tr("Can't open");
        return QString::null;
    }
    QTextStream codeSream(&code);
    QString text = codeSream.readAll();

#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/profiles/"+lang;
#else
    QString path = "/usr/share/kuzya/profiles/"+lang;
#endif

    QStringList supportedTransl = getSupportedTranslations(lang);

    QFile fileTrans;
    QTextStream trStream;
    QString trLine;
    QString translation;
    foreach (QString transl, supportedTransl)
    {
        fileTrans.setFileName(path+"/"+transl+".tr");
        if(!fileTrans.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << tr("Can't find translation for the code");
            return QString::null;
        }
        trStream.setDevice(&fileTrans);
        while (!trStream.atEnd())
        {
            trLine = trStream.readLine();
            translation = trLine.section('=', 1);
            if (text.contains(translation))
            {
                fileTrans.close();
                return transl;
            }
        }
        fileTrans.close();
    }
    return "en";
}
