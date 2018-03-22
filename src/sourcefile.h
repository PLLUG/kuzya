#ifndef SOURCEFILE_H
#define SOURCEFILE_H

class QTemporaryFile;
class QFile;
class QString;

class SourceFile
{
public:
    SourceFile();
    SourceFile(QString fileName);
    ~SourceFile();

    bool writeToFile(QString text);
    QString readFromFile() const;
    QString getFileName() const;
    bool isSaved() const;
    void save(QString fileName);

private:
    QFile *mFile;
};

#endif // SOURCEFILE_H
