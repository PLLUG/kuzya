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

private:
    QStringList supportedTranslations;
};

#endif // TRANSLATOR_H
