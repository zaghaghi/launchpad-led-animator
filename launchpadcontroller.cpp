#include "launchpadcontroller.h"

LaunchpadController::LaunchpadController():
    callback(NULL),
    userData(NULL)
{

}

void LaunchpadController::connect(int id)
{
    midiOut.openPort(id);
}

void LaunchpadController::disconnect()
{
    midiOut.closePort();
}

int LaunchpadController::getKeyFromLocation(int x, int y, LayoutType layout)
{
    int key = 0;
    switch (layout) {
    case DRUM_RANK_LAYOUT:
        key = x < 4 ? (7 - y) * 4 + x + 36 : (7 - y) * 4 + (x - 4) + 68;
        break;
    case XY_LAYOUT:
        key = y*16 + x;
        break;
    }
    return key;
}

void LaunchpadController::sendColor(int color, int x, int y, LayoutType layout)
{
    if (x < 0 || y < 0 || x > 7 || y > 7) {
        return;
    }

    int key = getKeyFromLocation(x, y, layout);


    int velocity = color;

    std::vector<unsigned char> message;

    message.push_back(144);
    message.push_back(key);
    message.push_back(velocity);
    midiOut.sendMessage(&message);

}

void LaunchpadController::startListen(int id, LayoutType layout, OnKeyDownCallback callback, void* userData)
{
    midiIn.openPort(id);
    this->callback = callback;
    this->userData = userData;
    this->layout = layout;
    midiIn.setCallback(rtCallback, this);
}

void LaunchpadController::stopListen()
{
    midiIn.cancelCallback();
    midiIn.closePort();
}

void LaunchpadController::rtCallback(double /*timeStamp*/, std::vector<unsigned char> *message, void *data)
{
    LaunchpadController * thiz = (LaunchpadController*) data;
    if (message->size() == 3 && (*message)[0] == 148) { // key down
        if (thiz->callback) {
            thiz->callback((*message)[1], thiz->userData);
        }
    }
}
