#include "codefile.h"
#include <QString>
#include <QTemporaryFile>
#include <QFile>
#include <QDebug>

CodeFile::CodeFile()
{
    mFile = new QTemporaryFile();
}

CodeFile::CodeFile(QString fileName)
{
    mFile = new QFile(fileName);
}

CodeFile::~CodeFile()
{
    delete mFile;
}

bool CodeFile::writeToFile(QString text)
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

QString CodeFile::readFromFile() const
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

QString CodeFile::getFileName() const
{
    return mFile->fileName();
}

bool CodeFile::isSaved() const
{
    QString typeFile = mFile->metaObject()->className();
    if(typeFile == "QFile")
    {
        return true;
    }
    return false;
}

void CodeFile::Save(QString fileName)
{
    if(mFile->fileName() == fileName)
    {
        return;
    }
    mFile->setFileName(fileName);
}
