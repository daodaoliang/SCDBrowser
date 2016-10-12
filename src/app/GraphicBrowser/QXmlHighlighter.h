#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QMultiHash>
#include <QRegExp>

class QTextDocument;

class QXmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QXmlHighlighter(QTextDocument *parent);
    ~QXmlHighlighter(void);

protected:
    void highlightBlock(const QString &text);

private:
    enum State {Normal=0x01, InComment=0x02};
    enum Type {Tag, Attribute, Entity, Comment};

    void addRegex(Type type, const QString &pattern,bool minimal=true);

    void highlightPatterns(const QString &text);
    void highlightComments(const QString &text);

    QTextCharFormat tagFormat;
    QTextCharFormat attributeNameFormat;
    QTextCharFormat attributeValueFormat;
    QTextCharFormat entityFormat;
    QTextCharFormat commentFormat;
    QMultiHash<Type, QRegExp> regexForType;

};

