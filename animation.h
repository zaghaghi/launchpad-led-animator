#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"
#include <QVector>
#include <QString>

class Animation
{
public:
    Animation();
    Animation(QString input, QString output, int interval);

    const QVector<Frame>& getFrames() const { return frames; }
    void addFrame(const Frame &frame) { frames.push_back(frame); }

    QString getOutput() const { return output; }
    void setOutput(const QString& output) { this->output= output; }

    QString getInput() const { return input; }
    void setInput(const QString& input) { this->input= input; }

    int getInterval() const {return interval; }
    void setInterval(int interval){this->interval = interval; }

    int getStart() const { return start; }
    void setStart(int start) { this->start = start; }

    int getEnd() const { return end; }
    void setEnd(int end) { this->end = end; }

    int getTrigger() const { return trigger; }
    void setTrigger(int key) { trigger = key; }

    void save(QString filename);
    bool load(QString filename);
private:
    QVector<Frame> frames;
    QString input;
    QString output;
    int interval;
    int start;
    int end;
    int trigger;
};

#endif // ANIMATION_H
