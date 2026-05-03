#ifndef WINDOW_LAYOUT_H
#define WINDOW_LAYOUT_H

#include <QString>

class WindowLayout
{
public:
    WindowLayout();

    void save(const QString &path);

    void load(const QString &path);
};

#endif
