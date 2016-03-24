#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
#include <QVector>
#include <QString>

class Animation
{
public:
    Animation();
    Animation(QString device, int interval);

    const QVector<Frame>& getFrames() const { return frames; }
    void addFrame(const Frame &frame) { frames.push_back(frame); }

    QString getDevice() const { return device; }
    void setDevice(const QString& device) { this->device = device; }

    int getInterval() const {return interval; }
    void setInterval(int interval){this->interval = interval; }

    int getStart() const { return start; }
    void setStart(int start) { this->start = start; }

    int getEnd() const { return end; }
    void setEnd(int end) { this->end = end; }

    void save(QString filename);
    bool load(QString filename);
private:
    QVector<Frame> frames;
    QString device;
    int interval;
    int start;
    int end;
};

#endif // ANIMATION_H
