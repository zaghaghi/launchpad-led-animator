#include "animation.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

Animation::Animation(QString input, QString output, int interval):
    input(input), output(output), interval(interval), trigger(-1)
{
}

Animation::Animation():
    input(""),
    output(""),
    interval(0),
    trigger(-1)
{
}

void Animation::save(QString filename)
{
    QJsonArray animations;
    QJsonObject animation;
    animation.insert("output", output);
    animation.insert("input", input);
    animation.insert("interval", interval);
    animation.insert("start", start);
    animation.insert("end", end);
    if (trigger >= 0) {
        animation.insert("trigger", trigger);
    }
    QJsonArray framesArray;
    for (int i = 0; i < frames.length(); ++i) {
        QJsonObject frameObject;
        frameObject.insert("time", frames[i].getTime());
        frameObject.insert("width", frames[i].getWidth());
        frameObject.insert("height", frames[i].getHeight());
        QJsonArray colorArray;
        foreach (int color, frames[i].getColors()) {
            colorArray.push_back(color);
        }
        frameObject.insert("colors", colorArray);
        framesArray.push_back(frameObject);
    }
    animation.insert("frames", framesArray);
    animations.push_back(animation);

    QJsonDocument doc(animations);
    QFile jsonFile(filename);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
    jsonFile.close();

}

bool Animation::load(QString filename)
{
    QFile jsonFile(filename);
    jsonFile.open(QFile::ReadOnly);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll(), &error);
    jsonFile.close();
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    if (!doc.isArray() || doc.array().size() == 0) {
        return false;
    }
    QJsonArray animations = doc.array();
    if (!animations.at(0).isObject()) {
        return false;
    }
    QJsonObject animation = animations.at(0).toObject();

    if (!animation.contains("output") || !animation.contains("input") || !animation.contains("interval") ||
            !animation.contains("frames")) {
        return false;
    }

    input = animation["input"].toString();
    output = animation["output"].toString();
    interval = animation["interval"].toInt();
    start = animation.contains("start")? animation["start"].toInt() : 0;
    end = animation.contains("end")? animation["end"].toInt() : 100;
    trigger = animation.contains("trigger")? animation["trigger"].toInt(): -1;
    if (!animation["frames"].isArray()) {
        return false;
    }

    QJsonArray framesArray = animation["frames"].toArray();
    for (int i = 0; i < framesArray.size(); ++i) {
        if (!framesArray.at(i).isObject()) {
            return false;
        }
        QJsonObject frameObject = framesArray.at(i).toObject();
        if (!frameObject.contains("time") || !frameObject.contains("width") ||
            !frameObject.contains("height") || !frameObject.contains("colors")) {
            return false;
        }
        int time = frameObject["time"].toInt();
        int width = frameObject["width"].toInt();
        int height = frameObject["height"].toInt();
        if (!frameObject["colors"].isArray()) {
            return false;
        }
        QJsonArray colorArray = frameObject["colors"].toArray();
        Frame frame(time, width, height);
        for (int j = 0; j < colorArray.size(); ++j) {
            frame.addColor(colorArray.at(j).toInt(12));
        }
        addFrame(frame);
    }
    return true;
}
