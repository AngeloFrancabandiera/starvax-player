#ifndef ACTION_H
#define ACTION_H

#include <QAction>


class Action : public QAction
{
   Q_OBJECT
public:
   explicit Action( QObject *parent = nullptr);
   explicit Action( const QString &text, QObject *parent = nullptr);
   explicit Action( const QIcon &icon, const QString &text, QObject *parent = nullptr);

   ~ Action();

   /**
    * set visibility in main toolbars for operational modes
    * @param inEditMode
    * @param inOperatoinMode
    */
   void setVisibleInModes( bool inEditMode, bool inOperationMode);

   /**
    * @brief this is a wrapper for QAction::setShortcut that
    *   also sets the tooltip with 'name + shortcut'
    * @param shortcut - see base function argument
    */
   void bindShortcut( const QKeySequence &shortcut);

   bool isVisibleInOperationMode()
   {
      return m_showInOperationMode;
   }

   bool isVisibleInEditMode()
   {
      return m_showInEditMode;
   }

private:
   bool m_showInEditMode;
   bool m_showInOperationMode;
};

#endif // ACTION_H
