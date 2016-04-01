#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>

#include "launchpadcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getOutputDeviceId();
    int getInputDeviceId();
    int getLayout();
    int getInterval();

private slots:
    void on_action_Open_triggered();

    void on_deviceCombo_changed(QString);

    void on_action_About_triggered();

    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_actionS_ave_As_triggered();

    void on_action_Close_triggered();

    void on_actionE_xit_triggered();

private:
    Ui::MainWindow *ui;

    LaunchpadController controller;

    QComboBox* outputDeviceCombo;
    QComboBox* inputDeviceCombo;
    QComboBox* layoutCombo;
    QSpinBox* intervalBox;
    int currentDeviceId;

};

#endif // MAINWINDOW_H
