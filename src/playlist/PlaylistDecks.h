#ifndef PLAYLIST_DECKS_H
#define PLAYLIST_DECKS_H

#include <QChar>


namespace Playlist {

   typedef int Deck;

   inline QChar toLetter( int deck)
   {
      return QChar('A' + deck);
   }

   inline int toDeck( QChar ch)
   {
      return (int)(ch.unicode() - QChar('A').unicode());
   }

}  // namespace Playlist

#endif // PLAYLIST_DECKS_H
