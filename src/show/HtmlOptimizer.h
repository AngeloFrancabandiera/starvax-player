#ifndef HTMLOPTIMIZER_H
#define HTMLOPTIMIZER_H

#include <QObject>
#include <QStringList>
#include <QTextStream>

/*!
 *  \class CHtmlOptimizer
 *  \brief this class is usefull to optimize html for saving.
 *  \ingroup script_handling
 *
 *  This class operates on a (long) html string, parsing many of the
 *  styles and grouping them in a css section. Given the fixed qt
 *  generated html pattern, this usually shrinks the total html output.
 */

#define OPTIMIZE_1

class HtmlOptimizer : public QObject
{
   Q_OBJECT
public:
   explicit HtmlOptimizer(QObject *parent = nullptr);

   /** \brief optimize html string \a source_html for save. */
   QString & optimizeHtml( QString *source_html );

   /** when FALSE, info on font size is removed from generated html.
    * This preserves zoom capability on text (that is lost otherwise)
    */
   void setSaveScriptFontSize( bool save) {
      save_font_size_flag = save;
   }

private:
   void initialize();
   void parseAndOptimize();
   void split_pre_post_stylesheet();
   bool is_pre_stylesheet( const QString &line );
   void replace_common_styles();
   QString extract_style_per_line( const QString &line );
   QString extract_style_per_tag( QStringList *list, const QString &tag );
   QString get_or_create_class_suffix( QStringList *list, const QString &attributes );
   QString className_for_list( QStringList *list );
   void    insert_stylesheet();
   QString compose_stylesheet( QStringList *styles_list );
   void removeFontSize();

private:
   QString optimized_html;
   QTextStream *input_reader;

   QString qt_private_header;
   QStringList paragraph_list;
   QStringList span_list;
   bool save_font_size_flag;

};

#endif // HTMLOPTIMIZER_H
