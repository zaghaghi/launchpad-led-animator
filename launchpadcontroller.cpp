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

void LaunchpadController::sendColor(int color, int x, int y)
{
    if (x < 0 || y < 0 || x > 7 || y > 7) {
        return;
    }

    int key = x < 4 ? (7 - y) * 4 + x + 36 : (7 - y) * 4 + (x - 4) + 68;
    int velocity = color;

    //midi.noteOn(key, 0, velocity);

    qint32 msg = 144;
    msg |= key << 8;
    msg |= velocity << 16;
    midi.sendMsg(msg);

}
