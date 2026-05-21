#ifndef MEDIA_AUTOMATION_SET_H
#define MEDIA_AUTOMATION_SET_H

#include <QObject>
#include <array>

#include "MediaAutomation.h"
#include "testableAssert.h"
#include "PlaylistDecks.h"

/* This is a facility class that holds a set of pointers to
 * MediaAutomation instances. It is used to dispatch signals
 * with deck parameter to slots of MediaAutomation class, that has
 * no deck information.
*/
class MediaAutomationSet : public QObject
{
    Q_OBJECT
public:
    explicit MediaAutomationSet( QObject *parent = nullptr)
    {
    }

    void set( Playlist::Deck deck, MediaAutomation* item)
    {
        m_pointers.at(deck) = item;
    }

    MediaAutomation* get( int deck)
    {
        return m_pointers.at(deck);
    }

public slots:
    /* same as MediaAutomation::activateMediaById but with added deck parameter */
    void activateMediaById( const QString & stringId, const QString & deck_tag,
                            bool startImmediately, bool dontStopFlag)
    {
        Playlist::Deck deck_num = Playlist::toDeck(deck_tag.at(0));
        m_pointers.at(deck_num)->activateMediaById( stringId, startImmediately, dontStopFlag);
    }

private:
    std::array<MediaAutomation*, NUMBER_OF_MEDIA_DECKS> m_pointers;
};


#endif // MEDIA_AUTOMATION_SET_H

