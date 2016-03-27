#include "launchpadcontroller.h"

LaunchpadController::LaunchpadController()
{

}

void LaunchpadController::connect(QString id)
{
    midi.connect(id);
}

void LaunchpadController::disconnect()
{
    midi.disconnect();
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

    //midi.noteOn(key, 0, velocity);

    qint32 msg = 144;
    msg |= key << 8;
    msg |= velocity << 16;
    midi.sendMsg(msg);

}
