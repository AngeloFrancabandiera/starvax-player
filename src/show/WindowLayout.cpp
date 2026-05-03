#include "WindowLayout.h"

#include <QDataStream>
#include <QByteArray>


void WindowLayout::save( QDataStream &sink,
                         const QByteArray &windowState,
                         const QByteArray &windowGeometry )
{
    // sink << windowState.size();
    // sink << windowGeometry.size();

    sink << windowState;
    sink << windowGeometry;
}

void WindowLayout::load( QDataStream &sink,
                         QByteArray &windowState,
                         QByteArray &windowGeometry)
{
    // qsizetype state_size = 0;
    // qsizetype geometry_size = 0;

    // sink >> state_size;
    // sink >> geometry_size;

    sink >> windowState;
    sink >> windowGeometry;
}

