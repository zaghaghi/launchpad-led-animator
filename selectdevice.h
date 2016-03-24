#ifndef SELECTDEVICE_H
#define SELECTDEVICE_H

#include <QDialog>

namespace Ui {
class SelectDevice;
}

class SelectDevice : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDevice(QWidget *parent = 0);
    ~SelectDevice();

    void setSelectedDevice(QString);
    QString getSelectedDevice();

private:
    Ui::SelectDevice *ui;
};

#endif // SELECTDEVICE_H
