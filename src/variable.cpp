#include "variable.h"

#include <QRegExp>
#include <QDebug>

Variable::Variable()
{
}

Variable::Variable(QString name, QString type, QString content):
    mName(name),
    mType(type),
    mContent(content)
{
}

QStringList Variable::getSubVariables() const
{  // returns list of nested variables in first level
    QRegExp isPointerMatch("\\*");
    if(isPointerMatch.indexIn(mType) != -1) // if it is pointer
    {
        QString newName = mName;
        newName.prepend('*');
        return QStringList() << newName; // returns only addres of pointed object
    }
    int brackets = 0;
    QString currentName;
    QStringList nestedVareables;
    bool capture = true;
    for(int i=0;i<mContent.size();++i)
    {
        QChar ch = mContent[i];
        /* Control of nesting level */
        if(ch == '{')
        {
            ++brackets;
            continue;
        }
        if(ch == '}')
        {
            --brackets;
            continue;
        }
        /* if captured one name should to write it int nestedVariables */
        if(ch == ' ' && !currentName.isEmpty())
        {
            capture = false;
            nestedVareables << currentName;
            currentName.clear();
            continue;
        }
        /* start captured from the next nested type (after coma) */
        if(ch == ',')
        {
            capture = true;
            continue;
        }
        /* capture only characters if nesting level = 1 and capturing is allowed */
        if(ch != ' ' && ch != '=' && ch!= ',' && brackets == 1 && capture)
        {
            currentName.append(ch);
        }
    }
    return nestedVareables;
}

QStringList Variable::getNestedStructures() const
{
    QString nestedType = this->getContent();
    QStringList res;
    return res;

}

QString Variable::getName() const
{
    return mName;
}

QString Variable::getType() const
{
    return mType;
}

QString Variable::getContent() const
{
    return mContent;
}

QStringList Variable::readNestedStruct(const QString &vec) const
{
    int level = 0;
    bool capture = true;
    QString key = "";
    QString value = "";
    QStringList res;
    for (int i = vec.indexOf('{'); i>=0 && i < vec.size(); i++)
    {
        if (vec[i] == '{') { ++level;}
        if (vec[i] == '}'){ --level; }
        QString behind = "";
        if (i + 2 < vec.size()) behind = vec.mid(i, 2);
        if (behind == " =" && capture)
        {
            capture = false;
        }
        else if (vec[i] == ',' && level == 1 && !capture || i+1 == vec.size())
        {
            capture = true;
            if(key.isEmpty() && value.isEmpty())
            {
                break;
            }
            else if(!key.isEmpty() && !value.isEmpty())
            {
                value[0] = ' ';
                value[1] = ' ';
                value = value.trimmed();
                res << QString("%1|%2").arg(key).arg(value);
            }
            if(i + 2 < vec.size())
            {
                ++i;
                ++i;
            }
            else
            {
                break;
            }
            value = "";
            key = "";
        }
        else if(!capture)
        {
            value += vec[i];
        }
        if (capture)
        {
            if(vec[i] == '{' && key.size() == 0) continue;
            key += vec[i];
        }
    }
    return res;
}

std::vector<Variable> Variable::getNestedTypes() const
{
    QStringList nestedTypeString = readNestedStruct(mContent);
    std::vector<Variable> nestedTypes;
    for(auto i : nestedTypeString)
    {
        QStringList varInfo = i.split('|');
        if(varInfo.size() == 2)
        {
            nestedTypes.emplace_back(QString("%1.%2").arg(mName).arg(varInfo[0]), "<No info>", varInfo[1]);
        }
    }
    return nestedTypes;
}

void Variable::setType(const QString &type)
{
    mType = type;
}

bool Variable::isPointer() const
{
    QRegExp pointersMathc("\\*");
    return pointersMathc.indexIn(mType) != -1;
}

void Variable::setContent(const QString &content)
{
    mContent = content;
}
