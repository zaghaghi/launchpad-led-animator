#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainanimation.h"
#include "about.h"
#include <QLabel>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentDeviceId(0)
{
    ui->setupUi(this);

    outputDeviceCombo = new QComboBox;
    QLabel* label = new QLabel(" Output: ");
    RtMidiOut midiOut;
    int midiPorts = midiOut.getPortCount();

    int i = 0;
    for (i = 0; i < midiPorts; ++i) {
        outputDeviceCombo->addItem(QString::fromStdString(midiOut.getPortName(i)), QVariant(i));
    }
    ui->toolBar->addWidget(label);
    ui->toolBar->addWidget(outputDeviceCombo);
    QObject::connect(outputDeviceCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_deviceCombo_changed(QString)));

    inputDeviceCombo = new QComboBox;
    label = new QLabel(" Input: ");
    RtMidiIn midiIn;
    midiPorts = midiIn.getPortCount();

    for (i = 0; i < midiPorts; ++i) {
        inputDeviceCombo->addItem(QString::fromStdString(midiIn.getPortName(i)), QVariant(i));
    }
    ui->toolBar->addWidget(label);
    ui->toolBar->addWidget(inputDeviceCombo);
    ui->toolBar->addSeparator();
    label = new QLabel(" Interval: ");
    ui->toolBar->addWidget(label);
    intervalBox = new QSpinBox();
    intervalBox->setButtonSymbols(QSpinBox::NoButtons);
    intervalBox->setMaximum(10000);
    intervalBox->setMinimum(1);
    intervalBox->setValue(200);
    ui->toolBar->addWidget(intervalBox);

    label = new QLabel(" Layout: ");
    layoutCombo = new QComboBox;
    layoutCombo->addItem("Drum Rack", 0);
    layoutCombo->addItem("X-Y", 1);

    ui->toolBar->addWidget(label);
    ui->toolBar->addWidget(layoutCombo);

    ui->action_New->trigger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getOutputDeviceId()
{
    if (outputDeviceCombo) {
        return outputDeviceCombo->currentData().toInt();
    }
    return -1;
}

int MainWindow::getInputDeviceId()
{
    if (inputDeviceCombo) {
        return inputDeviceCombo->currentData().toInt();
    }
    return -1;
}

int MainWindow::getLayout()
{
    if (layoutCombo) {
        return layoutCombo->currentData().toInt();
    }
    return -1;
}

int MainWindow::getInterval()
{
    if (intervalBox) {
        return intervalBox->value();
    }
    return 200;
}

void MainWindow::on_deviceCombo_changed(QString)
{
    currentDeviceId = outputDeviceCombo->currentData().toInt();
}

void MainWindow::on_action_About_triggered()
{
    About about;
    about.exec();
}

void MainWindow::on_action_Open_triggered()
{
    MainAnimation *animationWindow = new MainAnimation(&controller, this);
    animationWindow->open();
    ui->mdiArea->addSubWindow(animationWindow);
    animationWindow->showMaximized();
}

void MainWindow::on_action_New_triggered()
{
    MainAnimation *animationWindow = new MainAnimation(&controller, this);
    ui->mdiArea->addSubWindow(animationWindow);
    animationWindow->showMaximized();
}

void MainWindow::on_action_Save_triggered()
{
    QMdiSubWindow* window = ui->mdiArea->activeSubWindow();
    MainAnimation* animWindow = (MainAnimation*) window->widget();
    animWindow->save();
}

void MainWindow::on_actionS_ave_As_triggered()
{
    QMdiSubWindow* window = ui->mdiArea->activeSubWindow();
    MainAnimation* animWindow = (MainAnimation*) window->widget();
    animWindow->saveas();
}

void MainWindow::on_action_Close_triggered()
{
    QMdiSubWindow* window = ui->mdiArea->activeSubWindow();
    MainAnimation* animWindow = (MainAnimation*) window->widget();
    //animWindow->doclose();
    window->close();
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}
