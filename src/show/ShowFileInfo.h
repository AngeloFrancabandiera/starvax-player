#ifndef SHOWFILEINFO_H
#define SHOWFILEINFO_H

#include <QObject>

/**
 * @brief The ShowFileInfo class is a simple utility to
 * store file path of the current show.
 * This info is needed by some class that must not inherit
 * from QObject
 */
class ShowFileInfo : public QObject
{
   Q_OBJECT
public:
   explicit ShowFileInfo( QObject *parent = nullptr) :
      QObject(parent)
   {}

   QString getCurrentShowFilePath() const {
      return m_showFilePath;
   }

public slots:
   void onShowFilePathChanged( const QString & newPath) {
      m_showFilePath = newPath;
   }

private:
   QString m_showFilePath;

};

#endif // SHOWFILEINFO_H
