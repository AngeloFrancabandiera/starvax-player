#include "SequencerHighlighter.h"
#include <QRegularExpression>
#include "ApplicationSettings.h"

#include <QStringList>

SequencerHighlighter::SequencerHighlighter( const ApplicationSettings & settings,
                                            QTextDocument * parent) :
   QSyntaxHighlighter (parent),
   m_settings(settings)
{
   const  QStringList mainKeywords = QStringList() << "repeat"
                                                   << "end"
                                                   << "playlist"
                                                   << "light"
                                                   << "sequence"
                                                   << "hall"
                                                   << "entry";

   const  QStringList subKeywords = QStringList() << "forever"
                                                  << "play"
                                                  << "playAll"
                                                  << "stop"
                                                  << "show"
                                                  << "hide"
                                                  << "top"
                                                  << "bottom"
                                                  << "pause"
                                                  << "rewind"
                                                  << "fadeout"
                                                  << "volume"
                                                  << "trigger"
                                                  << "on"
                                                  << "off"
                                                  << "wait";

   // regular expression matching primary keywords
   QString mainKeywordPattern = mainKeywords.join('|');
   mainKeywordPattern.prepend("\\b(");
   mainKeywordPattern.append(")\\b");

   m_mainKeywordRegExp = new QRegularExpression( mainKeywordPattern);

   // regular expression matching sub keywords
   QString subKeywordPattern = subKeywords.join('|');
   subKeywordPattern.prepend("\\b(");
   subKeywordPattern.append(")\\b");

   m_subKeywordRegExp = new QRegularExpression( subKeywordPattern);

   QString numerPatter = QString("\\b((\\d|\\.)+|A|B|s)\\b");
   m_pseudoNumberRegExp = new QRegularExpression( numerPatter);

   // single line string enclosed in double quotes
   QString literalStringPatter="\".*\"";
   m_stringRegExp = new QRegularExpression( literalStringPatter);

   QTextCharFormat baseFormat;
   baseFormat.setFontFamilies( QStringList("courier new"));
   baseFormat.setFontPointSize( m_settings.getSidePanelsFontSize());

   m_commentFormat = new QTextCharFormat( baseFormat);
   m_commentFormat->setForeground(Qt::darkGreen);

   m_mainFormat = new QTextCharFormat( baseFormat);
   m_mainFormat->setFontWeight(QFont::Bold);
   m_mainFormat->setForeground(Qt::darkMagenta);

   m_subFormat = new QTextCharFormat( baseFormat);
   m_subFormat->setFontWeight(QFont::Bold);
   m_subFormat->setForeground(Qt::darkBlue);

   m_numberFormat = new QTextCharFormat( baseFormat);
   m_numberFormat->setForeground(Qt::darkCyan);

   m_literalStringFormat = new QTextCharFormat( baseFormat);
   m_literalStringFormat->setForeground(Qt::darkRed);

   connect( &m_settings, & ApplicationSettings::panelFontSizeChanged,
            this, & SequencerHighlighter::onFontSizeChnaged);
}


void SequencerHighlighter::highlightBlock(const QString & text)
{
   QString trimmedText = text.trimmed();

   if (trimmedText.startsWith('#'))
   {
      // this is a comment
      setFormat(0, text.size(), *m_commentFormat);
   }
   else
   {
      QRegularExpressionMatchIterator mainIterator = m_mainKeywordRegExp->globalMatch(text);
      while (mainIterator.hasNext())
      {
         QRegularExpressionMatch match = mainIterator.next();
         setFormat(match.capturedStart(), match.capturedLength(), *m_mainFormat);
      }

      QRegularExpressionMatchIterator subIterator = m_subKeywordRegExp->globalMatch(text);
      while (subIterator.hasNext())
      {
         QRegularExpressionMatch match = subIterator.next();
         setFormat(match.capturedStart(), match.capturedLength(), *m_subFormat);
      }

      QRegularExpressionMatchIterator numberIterator = m_pseudoNumberRegExp->globalMatch(text);
      while (numberIterator.hasNext())
      {
         QRegularExpressionMatch match = numberIterator.next();
         setFormat(match.capturedStart(), match.capturedLength(), *m_numberFormat);
      }

      // string literal are parsed as last
      QRegularExpressionMatchIterator stringIterator = m_stringRegExp->globalMatch(text);
      while (stringIterator.hasNext())
      {
         QRegularExpressionMatch match = stringIterator.next();
         setFormat(match.capturedStart(), match.capturedLength(), *m_literalStringFormat);
      }
   }
}

void SequencerHighlighter::onFontSizeChnaged(int pointSize)
{
  m_commentFormat->setFontPointSize( (qreal)pointSize);
  m_mainFormat->setFontPointSize( (qreal)pointSize);
  m_subFormat->setFontPointSize( (qreal)pointSize);
  m_numberFormat->setFontPointSize( (qreal)pointSize);
  m_literalStringFormat->setFontPointSize( (qreal)pointSize);

  rehighlight();
}

