#include "ledcolors.h"

LEDColors::LEDColors()
{

}

QList<QColor> LEDColors::generateColors()
{
    QList<QColor> colors;
    for (int redgreen = 0; redgreen < 16; ++redgreen) {
        short int red = redgreen & 3;
        short int green = (redgreen & 12) >> 2;
        int color = red | 12 | green << 4;
        QColor trueColor(red*85, green*85, 0);
        colors.append(trueColor);
        colorToVelocity[colorToInt(trueColor)] = color;
    }
    return colors;
}

QColor LEDColors::mapToQColor(int velocity)
{
    short int red = velocity & 3;
    short int green = (velocity >> 4) & 3;
    return QColor(red*85, green*85, 0);
}

int LEDColors::mapToMidiVelocity(QColor color)
{
    int ret = 12;
    int val = colorToInt(color);
    if (colorToVelocity.contains(val)) {
        ret = colorToVelocity[val];
    }
    return ret;
}

int LEDColors::colorToInt(QColor color)
{
    int ret = (color.red() << 16) + (color.green() << 8) + color.blue();
    return ret;
}
