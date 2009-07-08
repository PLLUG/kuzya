#include "translator.h"

#include <QDirIterator>
#include <QApplication>

#include <QDebug>

Translator::Translator(QObject *parent) : QObject(parent)
{
    QStringList list = getSupportedTranslations("c++");
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
            qDebug() << translation;
        }
    }
    supportedTranslations.removeDuplicates();
    return supportedTranslations;
}
