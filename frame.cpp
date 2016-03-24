#include "frame.h"

Frame::Frame(int time, int width, int height):
    time(time), width(width), height(height)
{

}

Frame::Frame(const Frame& frame):
    time(frame.time), width(frame.width), height(frame.height)
{
    foreach(int color, frame.colors) {
        colors.append(color);
    }
}

Frame::Frame():
    time(-1), width(0), height(0)
{

}
