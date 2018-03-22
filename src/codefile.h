#ifndef CODEFILE_H
#define CODEFILE_H

class QTemporaryFile;
class QFile;
class QString;

class CodeFile
{
public:
    CodeFile();
    CodeFile(QString fileName);
    ~CodeFile();

    bool writeToFile(QString text);
    QString readFromFile() const;
    QString getFileName() const;
    bool isSaved() const;
    void Save(QString fileName);

private:
    QFile *mFile;
};

#endif // CODEFILE_H
