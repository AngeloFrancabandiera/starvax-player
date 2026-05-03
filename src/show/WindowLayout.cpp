#include "WindowLayout.h"

#include <QDataStream>
#include <QByteArray>


void WindowLayout::save( QDataStream &sink,
                         const QByteArray &windowState,
                         const QByteArray &windowGeometry )
{
    sink << windowState;
    sink << windowGeometry;
}

void WindowLayout::load( QDataStream &source,
                         QByteArray &windowState,
                         QByteArray &windowGeometry)
{
    source >> windowState;
    source >> windowGeometry;
}

