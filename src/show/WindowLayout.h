#ifndef WINDOW_LAYOUT_H
#define WINDOW_LAYOUT_H

class QDataStream;
class QByteArray;


class WindowLayout
{
public:
    WindowLayout()
    {
    }

    void save( QDataStream &sink,
               const QByteArray &windowState,
               const QByteArray &windowGeometry );

    void load( QDataStream &sink,
               QByteArray &windowState,
               QByteArray &windowGeometry);
};

#endif
