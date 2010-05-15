#ifndef LEXERLOADER_H
#define LEXERLOADER_H
#include <qobject.h>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>
#include <QtXml>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QDomNamedNodeMap>
#include <QFontDatabase>
class LexerLoader : public QsciLexer
{
    Q_OBJECT
public:
    //! This enum defines the meanings of the different styles used by the
    //! lexer.
    enum {
        //! The default.
        Default = 0,

        //! A C comment.
        Comment = 1,

        //! A C++ comment line.
        CommentLine = 2,

        //! A JavaDoc/Doxygen style C comment.
        CommentDoc = 3,

        //! A number.
        Number = 4,

        //! A keyword.
        Keyword = 5,

        //! A double-quoted string.
        DoubleQuotedString = 6,

        //! A single-quoted string.
        SingleQuotedString = 7,

        //! An IDL UUID.
        UUID = 8,

        //! A pre-processor block.
        PreProcessor = 9,

        //! An operator.
        Operator = 10,

        //! An identifier
        Identifier = 11,

        //! The end of a line where a string is not closed.
        UnclosedString = 12,

        //! A C# verbatim string.
        VerbatimString = 13,

        //! A JavaScript regular expression.
        Regex = 14,

        //! A JavaDoc/Doxygen style C++ comment line.
        CommentLineDoc = 15,

        //! A keyword defined in keyword set number 2.  The class must
        //! be sub-classed and re-implement keywords() to make use of
        //! this style.
        KeywordSet2 = 16,

        //! A JavaDoc/Doxygen keyword.
        CommentDocKeyword = 17,

        //! A JavaDoc/Doxygen keyword error.
        CommentDocKeywordError = 18,

        //! A global class or typedef defined in keyword set number 4.
        //! The class must be sub-classed and re-implement keywords()
        //! to make use of this style.
        GlobalClass = 19
    };
    //LexerLoader();
    //! Construct a LexerLoader with parent \a parent.  \a parent is typically
       //! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
       //! lexer ignores the case of keywords.
       LexerLoader(QObject *parent = 0, bool caseInsensitiveKeywords = false,QString languageName="normal");

       //! Destroys the LexerLoader instance.
       virtual ~LexerLoader();

       //! Returns the name of the language.
       const char *language() const;

       //! Returns the name of the lexer.  Some lexers support a number of
       //! languages.
       const char *lexer() const;

       //! \internal Returns the character sequences that can separate
       //! auto-completion words.
       QStringList autoCompletionWordSeparators() const;

       //! \internal Returns a space separated list of words or characters in
       //! a particular style that define the end of a block for
       //! auto-indentation.  The styles is returned via \a style.
       const char *blockEnd(int *style = 0) const;

       //! \internal Returns a space separated list of words or characters in
       //! a particular style that define the start of a block for
       //! auto-indentation.  The styles is returned via \a style.
       const char *blockStart(int *style = 0) const;

       //! \internal Returns a space separated list of keywords in a
       //! particular style that define the start of a block for
       //! auto-indentation.  The styles is returned via \a style.
       const char *blockStartKeyword(int *style = 0) const;

       //! \internal Returns the style used for braces for brace matching.
       int braceStyle() const;

       //! \internal Returns the string of characters that comprise a word.
       const char *wordCharacters() const;

       //! Returns the foreground colour of the text for style number \a style.
       //!
       //! \sa defaultPaper()
       QColor defaultColor(int style) const;

       //! Returns the end-of-line fill for style number \a style.
       bool defaultEolFill(int style) const;

       //! Returns the font for style number \a style.
       QFont defaultFont(int style) const;

       //! Returns the background colour of the text for style number \a style.
       //!
       //! \sa defaultColor()
       QColor defaultPaper(int style) const;

       //! Returns the set of keywords for the keyword set \a set recognised
       //! by the lexer as a space separated string.
       const char *keywords(int set) const;

       //! Returns the descriptive name for style number \a style.  If the
       //! style is invalid for this language then an empty QString is returned.
       //! This is intended to be used in user preference dialogs.
       QString description(int style) const;

       //! Causes all properties to be refreshed by emitting the
       //! propertyChanged() signal as required.
       void refreshProperties();

       //! Returns true if "} else {" lines can be folded.
       //!
       //! \sa setFoldAtElse()
       bool foldAtElse() const;

       //! Returns true if multi-line comment blocks can be folded.
       //!
       //! \sa setFoldComments()
       bool foldComments() const;

       //! Returns true if trailing blank lines are included in a fold block.
       //!
       //! \sa setFoldCompact()
       bool foldCompact() const;

       //! Returns true if preprocessor blocks can be folded.
       //!
       //! \sa setFoldPreprocessor()
       bool foldPreprocessor() const;

       //! Returns true if preprocessor lines (after the preprocessor
       //! directive) are styled.
       //!
       //! \sa setStylePreprocessor()
       bool stylePreprocessor() const;

       //! If \a allowed is true then '$' characters are allowed in identifier
       //! names.  The default is true.
       //!
       //! \sa dollarsAllowed()
       void setDollarsAllowed(bool allowed);

       //! Returns true if '$' characters are allowed in identifier names.
       //!
       //! \sa setDollarsAllowed()
       bool dollarsAllowed() const;

       // Returns true if stylingFileName was setted .
       bool setStylingFileName(QString fileName);
        
   public slots:
       //! If \a fold is true then "} else {" lines can be folded.  The
       //! default is false.
       //!
       //! \sa foldAtElse()
       virtual void setFoldAtElse(bool fold);

       //! If \a fold is true then multi-line comment blocks can be folded.
       //! The default is false.
       //!
       //! \sa foldComments()
       virtual void setFoldComments(bool fold);

       //! If \a fold is true then trailing blank lines are included in a fold
       //! block. The default is true.
       //!
       //! \sa foldCompact()
       virtual void setFoldCompact(bool fold);

       //! If \a fold is true then preprocessor blocks can be folded.  The
       //! default is true.
       //!
       //! \sa foldPreprocessor()
       virtual void setFoldPreprocessor(bool fold);

       //! If \a style is true then preprocessor lines (after the preprocessor
       //! directive) are styled.  The default is false.
       //!
       //! \sa stylePreprocessor()
       virtual void setStylePreprocessor(bool style);

   protected:
       //! The lexer's properties are read from the settings \a qs.  \a prefix
       //! (which has a trailing '/') should be used as a prefix to the key of
       //! each setting.  true is returned if there is no error.
       //!
       //! \sa writeProperties()
       bool readProperties(QSettings &qs,const QString &prefix);

       //! The lexer's properties are written to the settings \a qs.
       //! \a prefix (which has a trailing '/') should be used as a prefix to
       //! the key of each setting.  true is returned if there is no error.
       //!
       //! \sa readProperties()
       bool writeProperties(QSettings &qs,const QString &prefix) const;

   private:
       void setAtElseProp();
       void setCommentProp();
       void setCompactProp();
       void setPreprocProp();
       void setStylePreprocProp();
       void setDollarsProp();

       bool fold_atelse;
       bool fold_comments;
       bool fold_compact;
       bool fold_preproc;
       bool style_preproc;
       bool dollars;

       bool nocase;

       QString stylingFileName;
       QString languageNameStr;
       QDomDocument domDoc;
       QDomNode domNode;
       QDomElement domElement;
       QFile xmlFile;
       LexerLoader(const LexerLoader &);
       LexerLoader &operator=(const LexerLoader &);

};

#endif // LEXERLOADER_H
