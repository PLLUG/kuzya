#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <vector>

class Variable
{
public:
    Variable();
    Variable(QString name, QString type, QString content);
    QStringList getSubVariables()const;
    QStringList getNestedStructures()const;
    QString getName()const;
    QString getType()const;
    QString getContent()const;
    QStringList readNestedStruct(const QString& vec)const;

    std::vector<Variable> getNestedTypes()const;
    void setType(const QString& type);
    bool isPointer()const;
    void setContent(const QString& content);
private:

    QString mName;
    QString mType;
    QString mContent;
};

#endif // VARIABLE_H
