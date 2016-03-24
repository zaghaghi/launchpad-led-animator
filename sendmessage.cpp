#include "sendmessage.h"
#include "ui_sendmessage.h"
#include <QMidi/QMidiOut.h>
#include <QMessageBox>

SendMessage::SendMessage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SendMessage)
{
    ui->setupUi(this);
    QMap<QString, QString> vals = QMidiOut::devices();
    int i = 0;
    foreach (QString val, vals) {
        ui->deviceCombo->addItem(val, i++);
    }
    ui->layoutCombo->addItem("X-Y", 1);
    ui->layoutCombo->addItem("Drum Rack", 2);
}

SendMessage::~SendMessage()
{
    delete ui;
}

void SendMessage::on_sendBtn_clicked()
{
    QMidiOut midi;
    if (midi.connect(ui->deviceCombo->currentData().toString())) {
        qint32 msg = ui->msgInput->value();
        msg |= ui->keyInput->value() << 8;
        msg |= ui->velInput->value() << 16;
        midi.sendMsg(msg);
        midi.disconnect();
    }
    else {
        QMessageBox::critical(this, "Error", "Can not connect to device [" + ui->deviceCombo->currentText() + "]");
    }
}

void SendMessage::on_layoutCombo_currentIndexChanged(int)
{
    QMidiOut midi;
    if (midi.connect(ui->deviceCombo->currentData().toString())) {
        qint32 msg = 176;
        msg |= 0 << 8;
        msg |= ui->layoutCombo->currentData().toInt() << 16;
        midi.sendMsg(msg);
        midi.disconnect();
    }
    else {
        QMessageBox::critical(this, "Error", "Can not connect to device [" + ui->deviceCombo->currentText() + "]");
    }
}
