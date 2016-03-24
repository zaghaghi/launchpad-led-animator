#ifndef LAUNCHPADCONTROLLER_H
#define LAUNCHPADCONTROLLER_H
#include <QMidi/QMidiOut.h>

class LaunchpadController
{
public:
    LaunchpadController();

    void connect(QString id);
    void disconnect();
    void sendColor(int color, int x, int y);
private:
    QMidiOut midi;
};

#endif // LAUNCHPADCONTROLLER_H
