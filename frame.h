#ifndef FRAME_H
#define FRAME_H

#include <QVector>

class Frame
{
public:
    Frame(int time, int width, int height);
    Frame(const Frame&);
    Frame();

    void setTime(int time) { this->time = time; }
    int getTime() const { return time; }

    void addColor(int color) { colors.push_back(color); }
    const QVector<int>& getColors() const { return colors; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
private:
    int time;
    int width;
    int height;
    QVector<int> colors;

};

#endif // FRAME_H
