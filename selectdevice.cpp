#include "selectdevice.h"
#include "ui_selectdevice.h"
#include "QMidi/QMidiOut.h"

SelectDevice::SelectDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDevice)
{
    ui->setupUi(this);
    QMap<QString, QString> vals = QMidiOut::devices();
    int i = 0;
    foreach (QString val, vals) {
        ui->deviceCombo->addItem(val, i++);
    }
}

SelectDevice::~SelectDevice()
{
    delete ui;
}

void SelectDevice::setSelectedDevice(QString id)
{
    ui->deviceCombo->setCurrentIndex(id.toInt());
}

QString SelectDevice::getSelectedDevice()
{
    return ui->deviceCombo->currentData().toString();
}
