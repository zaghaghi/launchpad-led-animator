#ifndef LEDCOLORS_H
#define LEDCOLORS_H

#include <QList>
#include <QColor>
#include <QMap>

class LEDColors
{
public:
    LEDColors();
    QList<QColor> generateColors();

    static QColor mapToQColor(int velocity);
    int mapToMidiVelocity(QColor);

private:
    QMap<int, int> colorToVelocity;

    int colorToInt(QColor);
};

#endif // LEDCOLORS_H
