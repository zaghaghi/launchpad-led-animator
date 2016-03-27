#ifndef LAUNCHPADCONTROLLER_H
#define LAUNCHPADCONTROLLER_H
#include <QMidi/QMidiOut.h>

class LaunchpadController
{
public:
    LaunchpadController();

    typedef enum { DRUM_RANK_LAYOUT, XY_LAYOUT }    LayoutType;

    void connect(QString id);
    void disconnect();
    void sendColor(int color, int x, int y, LayoutType layout);
private:
    QMidiOut midi;
};

#endif // LAUNCHPADCONTROLLER_H
