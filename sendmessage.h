#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SendMessage;
}

class SendMessage : public QMainWindow
{
    Q_OBJECT

public:
    explicit SendMessage(QWidget *parent = 0);
    ~SendMessage();

private slots:
    void on_sendBtn_clicked();

    void on_layoutCombo_currentIndexChanged(int index);

private:
    Ui::SendMessage *ui;
};

#endif // MAINWINDOW_H
