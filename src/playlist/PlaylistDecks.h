#ifndef PLAYLIST_DECKS_H
#define PLAYLIST_DECKS_H

#include <QChar>


namespace Playlist {

   /// Deck number is from 0 (deck 'A') to NUMBER_OF_MEDIA_DECKS-1
   typedef int Deck;

   /// @note no check on 'deck'
   /// @param deck has range 0 .. NUMBER_OF_MEDIA_DECKS-1
   /// @return a capital letter describing the deck
   inline QChar toLetter( int deck)
   {
      return QChar('A' + deck);
   }

   /// @note no check on 'ch'
   /// @param ch is in range 'A' to last valid letter for deck
   /// @return an integer in range 0 .. NUMBER_OF_MEDIA_DECKS-1
   inline int toDeck( QChar ch)
   {
      return (int)(ch.unicode() - QChar('A').unicode());
   }

}  // namespace Playlist

#endif // PLAYLIST_DECKS_H
