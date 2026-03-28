#include "HtmlOptimizer.h"
#include <QRegularExpression>
#include <QIODevice>

#define TERMINATED(string)      (string + "\n")

HtmlOptimizer::HtmlOptimizer(QObject *parent) :
   QObject(parent)
{
   input_reader = nullptr;
}

/**
 *  This is the only public function of the class. Input string \p
 *  source_html is parsed and spanned styles are collected in css
 *  styles. The optimized html is returned.
 */
QString & HtmlOptimizer::optimizeHtml( QString *source_html )
{
   input_reader  = new QTextStream( source_html, QIODevice::ReadOnly );
   initialize();
   parseAndOptimize();
   delete input_reader;

   // removing font size preserves zoom functionality
   if (save_font_size_flag == false)
   {
      removeFontSize();
   }

   return optimized_html;
}

void HtmlOptimizer::initialize()
{
   optimized_html = QString();
   qt_private_header = QString();
   paragraph_list = QStringList();
   span_list = QStringList();
}

void HtmlOptimizer::parseAndOptimize()
{
   split_pre_post_stylesheet();
   replace_common_styles();
   insert_stylesheet();
}

/*!
 * This function separates what goes before generated css styles
 * and what goes after them.
 */
void HtmlOptimizer::split_pre_post_stylesheet()
{
   bool go_on = true;

   while( go_on )
   {
      QString line = input_reader->readLine();

      if( is_pre_stylesheet( line ))
      {
         qt_private_header.append( TERMINATED(line) );
      }
      else
      {
         optimized_html.append( TERMINATED(line) );
         go_on = false;
      }
   }
}

bool HtmlOptimizer::is_pre_stylesheet( const QString &line )
{
   return !( line.isNull() || line.startsWith( QString("</style>")) );
}

/*!
 * This is the main cycle that parses input html line by line
 */
void HtmlOptimizer::replace_common_styles()
{
   while( !input_reader->atEnd() )
   {
      QString line = input_reader->readLine();

      optimized_html.append( extract_style_per_line( line ) );
   }
}

QString HtmlOptimizer::extract_style_per_line( const QString &line )
{
   // every string of this list is the internal of
   // an html tag
   QStringList split_by_tag = line.split( QChar('<'), STR_SPLIT::SkipEmptyParts );
   QString extracted_line;

   foreach( QString tag, split_by_tag )
   {
      tag.insert(0, "<" );

      if( tag.startsWith("<p style=") )
      {
         extracted_line += extract_style_per_tag( &paragraph_list, tag );
      }
      else if( tag.startsWith("<span style=") )
      {
         extracted_line += extract_style_per_tag( &span_list, tag );
      }
      else
      {
         extracted_line.append( tag );
      }
   }

   return extracted_line + "\n";
}

/*!
 * This function extract 'style' tags in a given fragment \a tag, that is
 * actually everything in between two successive '<' characters.
 */
QString HtmlOptimizer::extract_style_per_tag( QStringList *list, const QString &tag )
{
   QString tag_class = className_for_list(list);

   QString extracted_tag;

   extracted_tag += ("<"+tag_class+" class=\""+tag_class.at(0));
   extracted_tag += get_or_create_class_suffix( list, tag.section('\"', 1,1) );
   extracted_tag += "\"";
   extracted_tag += tag.section('\"', 2, 2 );

   return extracted_tag;
}

QString HtmlOptimizer::className_for_list( QStringList *list )
{
   QString className = QString("Ukn");

   if( list == &paragraph_list )
   {
      className = QString("p");
   }
   else if( list == &span_list )
   {
      className = QString("span");
   }

   return className;
}

/*!
 * This function compare given \a attributes string with all previously parsed.
 * If a match is found, the corresponding style suffix is returned, otherwise a
 * new one is created.
 */
QString HtmlOptimizer::get_or_create_class_suffix( QStringList *list, const QString &attributes )
{
   int match_index = list->indexOf( attributes );

   if( match_index == -1 )
   {
      // append given attributes as new
      match_index = list->count();
      list->append( attributes );
   }

   return QString("%1").arg( match_index );
}

void HtmlOptimizer::insert_stylesheet()
{
   QString stylesheet = compose_stylesheet(&paragraph_list) +
         compose_stylesheet(&span_list);

   // insert header and stylesheet
   optimized_html.insert(0, stylesheet);
   optimized_html.insert(0, qt_private_header);
}

/*!
 * This function composes a single css line, e.g.  p.p1 { <i>attributes</i> }
 */
QString HtmlOptimizer::compose_stylesheet( QStringList *styles_list )
{
   QString stylesheet;
   QString tag_class = className_for_list(styles_list);

   for( int i = 0; i < styles_list->count(); i++ )
   {
      stylesheet += QString("%1.%2%3 {%4 }\n").arg(tag_class)
            .arg(tag_class.at(0))
            .arg(i)
            .arg( styles_list->at(i) );
   }

   return stylesheet;
}

void HtmlOptimizer::removeFontSize()
{
   static QRegularExpression regexp("font-size:\\d+\\.?(\\d+)?p[tx];", QRegularExpression::MultilineOption);

   optimized_html.replace( regexp, "");
}
