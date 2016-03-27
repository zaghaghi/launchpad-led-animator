#include "launchpadcontroller.h"

LaunchpadController::LaunchpadController()
{

}

void LaunchpadController::connect(int id)
{
    midi.openPort(id);
}

void LaunchpadController::disconnect()
{
    midi.closePort();
}

void LaunchpadController::sendColor(int color, int x, int y, LayoutType layout)
{
    if (x < 0 || y < 0 || x > 7 || y > 7) {
        return;
    }

    int key = 0;
    switch (layout) {
    case DRUM_RANK_LAYOUT:
        key = x < 4 ? (7 - y) * 4 + x + 36 : (7 - y) * 4 + (x - 4) + 68;
        break;
    case XY_LAYOUT:
        key = y*16 + x;
        break;
    default:
        return;
    }


    int velocity = color;

    std::vector<unsigned char> message;

    message.push_back(144);
    message.push_back(key);
    message.push_back(velocity);
    midi.sendMessage(&message);

}
