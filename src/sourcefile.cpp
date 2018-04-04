#include "sourcefile.h"
#include <QString>
#include <QTemporaryFile>
#include <QFile>
#include <QDebug>
#include <QUuid>
#include <QApplication>

//Source file
SourceFile::SourceFile()
{
    mFile = new QTemporaryFile();

    QString temporaryFileName = QUuid::createUuid().toString();

    temporaryFileName.remove("{");
    temporaryFileName.remove("}");
    temporaryFileName.remove("-");

    temporaryFileName = qApp->applicationDirPath()+ "/" + temporaryFileName + ".txt";

    mFile->setFileName(temporaryFileName);
}

SourceFile::SourceFile(QString fileName)
{
    mFile = new QFile(fileName);
}

SourceFile::~SourceFile()
{
    delete mFile;
}

bool SourceFile::writeToFile(QString text)
{
    if (!mFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream stream(mFile);
    stream << text;
    mFile->close();
    return true;
}

QString SourceFile::readFromFile() const
{
    if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }
    QString text;
    QTextStream stream(mFile);
    text = stream.readAll();
    mFile->close();
    return text;
}

QString SourceFile::getFileName() const
{
    return mFile->fileName();
}

bool SourceFile::isSaved() const
{
    QString typeFile = mFile->metaObject()->className();
    if(typeFile == "QFile")
    {
        return true;
    }
    return false;
}

void SourceFile::save(QString fileName)
{
    if(!isSaved())
    {
        delete mFile;
        mFile = new QFile(fileName);
    }
    if(mFile->fileName() == fileName)
    {
        return;
    }
    mFile->setFileName(fileName);
}
