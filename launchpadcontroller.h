#ifndef LAUNCHPADCONTROLLER_H
#define LAUNCHPADCONTROLLER_H
#include "RtMidi.h"

class LaunchpadController
{
public:
    LaunchpadController();

    typedef enum { DRUM_RANK_LAYOUT, XY_LAYOUT }    LayoutType;

    void connect(int id);
    void disconnect();
    void sendColor(int color, int x, int y, LayoutType layout);
private:
    RtMidiOut midi;
};

#endif // LAUNCHPADCONTROLLER_H
