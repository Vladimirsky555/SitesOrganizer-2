#ifndef QREGEXPHIGHLIGHTER_H
#define QREGEXPHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

class QRegexpHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    QString pattern;

public:
    QRegexpHighlighter(QObject *parent = 0);
    virtual ~QRegexpHighlighter(){}

    void highlightBlock(const  QString &text);
    void setPattern(QString str){pattern = str;}
};

#endif // QREGEXPHIGHLIGHTER_H
