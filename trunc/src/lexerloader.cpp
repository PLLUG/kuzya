#include "lexerloader.h"
#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>
#include <QFile>
#include <QTextStream>
#include <cctype>
// The ctor.
LexerLoader::LexerLoader(QObject *parent, bool caseInsensitiveKeywords,QString languageName)
    : QsciLexer(parent),
      fold_atelse(false), fold_comments(false), fold_compact(true),
      fold_preproc(true), style_preproc(false), dollars(true),
      nocase(caseInsensitiveKeywords)
{
    languageNameStr = languageName;

/*
    QFile file("../profiles/langs1.xml");      //file with lexer's preferences
    if (!file.open(QIODevice::ReadOnly))
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is mising.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         return;
    }
    if (!domDoc.setContent(&file))  //sets file as the content of the document
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     file.close();

     domElement = domDoc.documentElement();
     domNode = domElement.firstChild();
     domNode=domNode.nextSibling();
     domNode=domNode.firstChild();
     //qDebug()<<domNode.toElement().tagName();
     //qDebug()<<domNode.toElement().attribute("name","");
     //qDebug()<<"**********"<<languageNameStr;
     while((languageNameStr!=domNode.toElement().attribute("name",""))&&(!domNode.isNull()))
     {
         //qDebug()<<domNode.toElement().attribute("name","");
         domNode=domNode.nextSibling();

     }
     if(languageNameStr!=domNode.toElement().attribute("name",""))
     {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     */
}


// The dtor.
LexerLoader::~LexerLoader()
{
}


// Returns the language name.
const char *LexerLoader::language() const
{
    return languageNameStr.toAscii();
}


// Returns the lexer name.
const char *LexerLoader::lexer() const
{
    //return (nocase ? "cppnocase" : "cpp");
    //const char * ch=languageNameStr.toStdString().c_str();

    return languageNameStr.toAscii();
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList LexerLoader::autoCompletionWordSeparators() const
{
    QStringList wl;
    QString str = domNode.namedItem("AutoCompletionWordSeparators").toElement().text().toLocal8Bit();
    if (str!="") wl = str.split(" ");
    ///wl << "::" << "->" << ".";

    return wl;
}


// Return the list of keywords that can start a block.
const char *LexerLoader::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    /*return "case catch class default do else finally for if private "
           "protected public struct try union while";*/

    return domNode.namedItem("BlockStartKeyword").toElement().text().toLocal8Bit();

}


// Return the list of characters that can start a block.
const char *LexerLoader::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    //return "{";
     return domNode.namedItem("BlockStart").toElement().text().toLocal8Bit();
}


// Return the list of characters that can end a block.
const char *LexerLoader::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    //return "}";
    return domNode.namedItem("BlockEnd").toElement().text().toLocal8Bit();
}


// Return the style used for braces.
int LexerLoader::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *LexerLoader::wordCharacters() const
{
    //return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#ячсмитьбюфівапролджйцукенгшщз";
    return domNode.namedItem("WordCharacters").toElement().text().toLocal8Bit();
}


// Returns the foreground colour of the text for a style.
QColor LexerLoader::defaultColor(int style) const
{
    QDomNode styleNode = domNode.namedItem("LexerStyle");
    QDomElement wordStyleDomElement = styleNode.firstChildElement();

    while((!wordStyleDomElement.isNull()&&(style!=wordStyleDomElement.attribute("styleID","-1").toInt())))
    {
        wordStyleDomElement=wordStyleDomElement.nextSiblingElement();
        //qDebug()<<wordStyleDomElement.attribute("name","");
    }
    if(style==wordStyleDomElement.attribute("styleID","-1").toInt())
    {
        return QColor("#"+wordStyleDomElement.attribute("fgColor","000000"));
    }
    else
    {
        return QsciLexer::defaultColor(style);
    }
 /*   switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case CommentDoc:
    case CommentLineDoc:
        return QColor(0x3f,0x70,0x3f);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0xff,0x00);

    case DoubleQuotedString:
    case SingleQuotedString:
        return QColor(0x7f,0x00,0x7f);

    case PreProcessor:
        return QColor(0x7f,0x7f,0x00);

    case Operator:
    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case Identifier:
        break;

    case Regex:
        return QColor(0x3f,0x7f,0x3f);

    case CommentDocKeyword:
        return QColor(0x30,0x60,0xa0);

    case CommentDocKeywordError:
        return QColor(0x80,0x40,0x20);
    }return QsciLexer::defaultColor(style);
*/

}


// Returns the end-of-line fill for a style.
bool LexerLoader::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont LexerLoader::defaultFont(int style) const
{
    QFont f;

    QDomNode styleNode = domNode.namedItem("LexerStyle");
    QDomElement wordStyleDomElement = styleNode.firstChildElement();

    while((!wordStyleDomElement.isNull()&&(style!=wordStyleDomElement.attribute("styleID","-1").toInt())))
    {
        wordStyleDomElement=wordStyleDomElement.nextSiblingElement();
        //qDebug()<<wordStyleDomElement.attribute("name","");
    }
    if(style==wordStyleDomElement.attribute("styleID","-1").toInt())
    {
        if(""!=wordStyleDomElement.attribute("fontName",""))
        {
            f=QFont(wordStyleDomElement.attribute("fontName",""));
            if (""!=wordStyleDomElement.attribute("fontSize",""))
            {
                f.setPointSize(wordStyleDomElement.attribute("fontSize","9").toInt());
            }
            if(1==wordStyleDomElement.attribute("fontStyle","").toInt()) f.setBold(true);
            if(2==wordStyleDomElement.attribute("fontStyle","").toInt()) f.setItalic(true);
        }

    }
    else
    {
        f=QsciLexer::defaultFont(style);
    }

/*
    switch (style)
    {

    case Comment:
    case CommentLine:
    case CommentDoc:
    case CommentLineDoc:
    case CommentDocKeyword:
    case CommentDocKeywordError:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DoubleQuotedString:
    case SingleQuotedString:
    case UnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }
*/
    return f;
}


// Returns the set of keywords.
const char *LexerLoader::keywords(int set) const
{

    {
        if (set == 1)
        {
            /*
             QFile file("/home/buoyantgrumbler/lexic.txt");
             if (file.open(QIODevice::ReadOnly | QIODevice::Text))
             QTextStream in(&file);
             QString str = in.readAll();
             char const *a=str.toLocal8Bit();
             return a;
             */
           /*
         QByteArray ba(str.toLocal8Bit());
         char const *a = ba.data();
        */

            //qDebug()<<domNode.namedItem("Keywords").toElement().text().toLocal8Bit();
            //qDebug()<<domNode.toElement().text().toLocal8Bit();
            return domNode.namedItem("Keywords").toElement().text().toLocal8Bit();
         }


    if (set == 3)
        return
            "a addindex addtogroup anchor arg attention author b "
            "brief bug c class code date def defgroup deprecated "
            "dontinclude e em endcode endhtmlonly endif "
            "endlatexonly endlink endverbatim enum example "
            "exception f$ f[ f] file fn hideinitializer "
            "htmlinclude htmlonly if image include ingroup "
            "internal invariant interface latexonly li line link "
            "mainpage name namespace nosubgrouping note overload "
            "p page par param post pre ref relates remarks return "
            "retval sa section see showinitializer since skip "
            "skipline struct subsection test throw todo typedef "
            "union until var verbatim verbinclude version warning "
            "weakgroup $ @ \\ & < > # { }";
    }
    return 0;
}


// Returns the user name of a style.
QString LexerLoader::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("C comment");

    case CommentLine:
        return tr("C++ comment");

    case CommentDoc:
        return tr("JavaDoc style C comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UUID:
        return tr("IDL UUID");

    case PreProcessor:
        return tr("Pre-processor block");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case CommentLineDoc:
        return tr("JavaDoc style C++ comment");

    case KeywordSet2:
        return tr("Secondary keywords and identifiers");

    case CommentDocKeyword:
        return tr("JavaDoc keyword");

    case CommentDocKeywordError:
        return tr("JavaDoc keyword error");

    case GlobalClass:
        return tr("Global classes and typedefs");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor LexerLoader::defaultPaper(int style) const
{
    QDomNode styleNode = domNode.namedItem("LexerStyle");
    QDomElement wordStyleDomElement = styleNode.firstChildElement();

    while((!wordStyleDomElement.isNull()&&(style!=wordStyleDomElement.attribute("styleID","-1").toInt())))
    {
        wordStyleDomElement=wordStyleDomElement.nextSiblingElement();
        //qDebug()<<wordStyleDomElement.attribute("name","");
    }
    if(style==wordStyleDomElement.attribute("styleID","-1").toInt())
    {
        return QColor("#"+wordStyleDomElement.attribute("bgColor","FFFFFF"));
    }
    else
    {
        return QColor("#FFFFFF") ;
    }
    /*
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);*/
}


// Refresh all properties.
void LexerLoader::refreshProperties()
{
    setAtElseProp();
    setCommentProp();
    setCompactProp();
    setPreprocProp();
    setStylePreprocProp();
    setDollarsProp();
}


// Read properties from the settings.
bool LexerLoader::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();
    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_preproc = qs.value(prefix + "foldpreprocessor", true).toBool();
    style_preproc = qs.value(prefix + "stylepreprocessor", false).toBool();
    dollars = qs.value(prefix + "dollars", true).toBool();

    return rc;
}


// Write properties to the settings.
bool LexerLoader::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpreprocessor", fold_preproc);
    qs.setValue(prefix + "stylepreprocessor", style_preproc);
    qs.setValue(prefix + "dollars", dollars);

    return rc;
}


// Return true if else can be folded.
bool LexerLoader::foldAtElse() const
{
    return fold_atelse;
}


// Set if else can be folded.
void LexerLoader::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void LexerLoader::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Return true if comments can be folded.
bool LexerLoader::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void LexerLoader::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void LexerLoader::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool LexerLoader::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void LexerLoader::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void LexerLoader::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool LexerLoader::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void LexerLoader::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void LexerLoader::setPreprocProp()
{
    emit propertyChanged("fold.preprocessor",(fold_preproc ? "1" : "0"));
}


// Return true if preprocessor lines are styled.
bool LexerLoader::stylePreprocessor() const
{
    return style_preproc;
}


// Set if preprocessor lines are styled.
void LexerLoader::setStylePreprocessor(bool style)
{
    style_preproc = style;

    setStylePreprocProp();
}


// Set the "styling.within.preprocessor" property.
void LexerLoader::setStylePreprocProp()
{
    emit propertyChanged("styling.within.preprocessor",(style_preproc ? "1" : "0"));
}


// Return true if '$' characters are allowed.
bool LexerLoader::dollarsAllowed() const
{
    return dollars;
}


// Set if '$' characters are allowed.
void LexerLoader::setDollarsAllowed(bool allowed)
{
    dollars = allowed;

    setDollarsProp();
}


// Set the "lexer.cpp.allow.dollars" property.
void LexerLoader::setDollarsProp()
{
    emit propertyChanged("lexer.cpp.allow.dollars",(dollars ? "1" : "0"));
}
bool LexerLoader::setStylingFileName(QString fileName)
{
    //QFile file("../profiles/langs1.xml");      //file with lexer's preferences
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is mising.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         return false;
    }
    if (!domDoc.setContent(&file))  //sets file as the content of the document
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return false;
     }
     file.close();

     domElement = domDoc.documentElement();
     domNode = domElement.firstChild();
     domNode=domNode.nextSibling();
     domNode=domNode.firstChild();
     //qDebug()<<domNode.toElement().tagName();
     //qDebug()<<domNode.toElement().attribute("name","");
     //qDebug()<<"**********"<<languageNameStr;
     while((languageNameStr!=domNode.toElement().attribute("name",""))&&(!domNode.isNull()))
     {
         //qDebug()<<domNode.toElement().attribute("name","");
         domNode=domNode.nextSibling();

     }
     if(languageNameStr!=domNode.toElement().attribute("name",""))
     {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File langs.xml is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return false;
     }
     return true;
}
