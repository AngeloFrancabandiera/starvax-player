#ifndef SEQUENCR_HIGHLIGHTER_H
#define SEQUENCR_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
class QRegularExpression;
class QTextCharFormat;
class ApplicationSettings;


class SequencerHighlighter : public QSyntaxHighlighter
{
   Q_OBJECT
public:
   SequencerHighlighter( const ApplicationSettings & settings,
                         QTextDocument *parent);

   // QSyntaxHighlighter interface
protected:
   void highlightBlock(const QString & text) override;

private slots:
   void onFontSizeChnaged( int pointSize);

private:
   const ApplicationSettings & m_settings;

   QRegularExpression * m_mainKeywordRegExp;
   QRegularExpression * m_subKeywordRegExp;
   QRegularExpression * m_pseudoNumberRegExp;
   QRegularExpression * m_stringRegExp;

   QTextCharFormat * m_commentFormat;
   QTextCharFormat * m_mainFormat;
   QTextCharFormat * m_subFormat;
   QTextCharFormat * m_numberFormat;
   QTextCharFormat * m_literalStringFormat;

};

#endif // SEQUENCR_HIGHLIGHTER_H

