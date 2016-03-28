#ifndef LAUNCHPADCONTROLLER_H
#define LAUNCHPADCONTROLLER_H
#include "RtMidi.h"

class LaunchpadController
{
public:
    LaunchpadController();

    typedef enum { DRUM_RANK_LAYOUT, XY_LAYOUT }    LayoutType;
    typedef void (*OnKeyDownCallback) (char key, void *userData);

    static int getKeyFromLocation(int x, int y, LayoutType layout);

    void connect(int id);
    void disconnect();
    void sendColor(int color, int x, int y, LayoutType layout);

    void startListen(int id, LayoutType layout, OnKeyDownCallback callback, void* userData);
    void stopListen();
    static void rtCallback(double timeStamp, std::vector<unsigned char> *message, void *data);

private:
    RtMidiOut midiOut;

    RtMidiIn midiIn;
    OnKeyDownCallback callback;
    void * userData;
    LayoutType layout;
};

#endif // LAUNCHPADCONTROLLER_H
