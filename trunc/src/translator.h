#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QString>
#include <QStringList>

class Translator : public QObject
{
    Q_OBJECT

public:
    Translator(QObject *parent = 0);
    ~Translator();

    QStringList getSupportedTranslations(QString lang);
    void openFile(QString file, QString lang);
    QString translation();
    QString codeFile();
    QString translatedCodeFile();
    void retranslate();
    void setTranslation(QString transl);

private:
    enum DirectionEnum { fromCode, toCode };

    QString detectCodeLanguage(QString filePath, QString lang);
    void translateCode(QString srcFile, QString destFile, DirectionEnum direction);

private:
    QStringList supportedTranslations;
    QString fileName;
    QString translatedFileName;
    QString translationsPath;
    QString language;
    QString codeTranslation;
};

#endif // TRANSLATOR_H
