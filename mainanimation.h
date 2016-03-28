#ifndef MAINANIMATION_H
#define MAINANIMATION_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QComboBox>
#include <QSpinBox>
#include <QMap>
#include <QAtomicInt>
#include <QMutex>
#include <QTimer>

#include "ledcolors.h"
#include "animation.h"
#include "launchpadcontroller.h"

namespace Ui {
class MainAnimation;
}

#define GRID_COLS   8
#define GRID_ROWS   8
#define GRID_PAD    5
#define GRID_WIDTH  70
#define GRID_HEIGHT 70

class MainAnimation : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainAnimation(QWidget *parent = 0);
    ~MainAnimation();

private slots:
    void on_colorList_currentRowChanged(int currentRow);

    void on_actionSend_triggered();

    void on_actionClear_triggered();

    void on_actionLive_Preview_triggered(bool checked);

    void on_deviceCombo_changed(QString);
    void on_action_Save_triggered();

    void on_startEdit_valueChanged(int arg1);

    void on_endEdit_valueChanged(int arg1);

    void on_gotoStart_clicked();

    void on_gotoPrev_clicked();

    void on_play_clicked();

    void on_gotoNext_clicked();

    void on_gotoEnd_clicked();

    void on_curEdit_valueChanged(int arg1);

    void on_keyButton_clicked();

    void play();
    void on_action_Open_triggered();

    void on_actionSave_As_triggered();

    void on_actionNew_triggered();

    void on_actionAbout_triggered();

    void on_actionSet_Trigger_triggered(bool checked);

    void on_actionListen_toggled(bool arg1);

    void on_actionSet_Trigger_toggled(bool arg1);

    void processReceivedKeys();
private:
    Ui::MainAnimation *ui;

    void setupUi();
    void mousePressEvent(QMouseEvent* event);
    void warning(const QString& title, const QString& text);

    int currentDeviceId;
    bool livePreview;
    QGraphicsRectItem* buttons[GRID_COLS][GRID_ROWS];
    int trigger;

    LEDColors colors;
    QComboBox* outputDeviceCombo;
    QComboBox* inputDeviceCombo;
    QComboBox* layoutCombo;
    QSpinBox* intervalBox;

    QMap<int, Frame> frameMap;

    void next(int val);
    QAtomicInt isPlay;
    int playInterval;

    void setFileName(QString filename);
    QString currentFile;

    LaunchpadController controller;
    QMutex mutex;
    QVector<char> receivedKeys;
    QTimer checkReceivedKeys;
    static void onLaunchpadKeyDown(char key, void *data);
};

#endif // MAINANIMATION_H
