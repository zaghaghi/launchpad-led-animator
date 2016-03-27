#include "mainanimation.h"
#include "ui_mainanimation.h"
#include "about.h"
#include "launchpadcontroller.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QTimer>
#include <QFileInfo>

MainAnimation::MainAnimation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainAnimation),
    currentDeviceId(0),
    livePreview(true),
    playInterval(0),
    currentFile("")
{
    isPlay.store(0);
    ui->setupUi(this);
    this->setupUi();
}

MainAnimation::~MainAnimation()
{
    delete ui;
}

void MainAnimation::setupUi()
{
    QGraphicsScene *scene = new QGraphicsScene();
    int r,c;
    for (c = 0; c < GRID_COLS; ++c) {
        for (r = 0; r < GRID_ROWS; ++r) {
            buttons[c][r] = scene->addRect(c*(GRID_PAD+GRID_WIDTH), r*(GRID_PAD+GRID_HEIGHT), GRID_WIDTH, GRID_HEIGHT);
            buttons[c][r]->setBrush(QColor(0,0,0));
        }
    }
    ui->buttonGrid->setScene(scene);
    QList<QColor> colorlist = colors.generateColors();

    foreach (QColor color, colorlist) {
        QListWidgetItem * item = new QListWidgetItem();
        item->setBackground(QBrush(color));
        item->setFlags(Qt::ItemIsEnabled);
        ui->colorList->addItem(item);
    }
    deviceCombo = new QComboBox;
    QLabel* label = new QLabel(" Device: ");
    RtMidiOut midi;
    int midiPorts = midi.getPortCount();

    int i = 0;
    for (i = 0; i < midiPorts; ++i) {
        deviceCombo->addItem(QString::fromStdString(midi.getPortName(i)), QVariant(i));
    }
    ui->toolBar->addWidget(label);
    ui->toolBar->addWidget(deviceCombo);
    QObject::connect(deviceCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_deviceCombo_changed(QString)));

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

    ui->frameSlider->setMinimum(ui->startEdit->value());
    ui->frameSlider->setMaximum(ui->endEdit->value());

}


void MainAnimation::on_deviceCombo_changed(QString)
{
    currentDeviceId = deviceCombo->currentData().toInt();
}

void MainAnimation::on_colorList_currentRowChanged(int currentRow)
{
    if (currentRow < 0) {
        return;
    }
    ui->activeColorList->item(0)->setBackgroundColor(ui->colorList->item(currentRow)->backgroundColor());
}

void MainAnimation::on_actionSend_triggered()
{
    LaunchpadController controller;
    controller.connect(currentDeviceId);
    LaunchpadController::LayoutType layout = (LaunchpadController::LayoutType) layoutCombo->currentData().toInt();
    for (int c = 0; c < GRID_COLS; ++c) {
        for (int r = 0; r < GRID_ROWS; ++r) {
            if (buttons[c][r]) {
                QColor cc = buttons[c][r]->brush().color();
                int ic = colors.mapToMidiVelocity(cc);
                controller.sendColor(ic, c, r, layout);
            }
        }
    }
    controller.disconnect();
}

void MainAnimation::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui->centralwidget->mapFromParent(event->pos());
    remapped = ui->buttonGrid->mapFromParent(remapped);
    if ( ui->buttonGrid->rect().contains( remapped ) )
    {
        QPointF mousePoint = ui->buttonGrid->mapToScene( remapped );
        int x = mousePoint.x() / (GRID_PAD+GRID_WIDTH);
        int y = mousePoint.y() / (GRID_PAD+GRID_HEIGHT);
        if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS) {
            return;
        }
        if (mousePoint.x() <= x * (GRID_PAD+GRID_WIDTH) ||  mousePoint.x() >= x * (GRID_PAD+GRID_WIDTH) + GRID_WIDTH ||
            mousePoint.y() <= y * (GRID_PAD+GRID_HEIGHT) ||  mousePoint.y() >= y * (GRID_PAD+GRID_HEIGHT) + GRID_HEIGHT) {
            return;
        }
        QGraphicsRectItem* button = buttons[x][y];
        if (button == NULL) {
            return;
        }
        if (button->brush().color() != ui->activeColorList->item(0)->backgroundColor()) {
            button->setBrush(ui->activeColorList->item(0)->background());
        }
        else {
            button->setBrush(QColor(0,0,0));
        }
        if (livePreview) {
            ui->actionSend->trigger();
        }
    }
}

void MainAnimation::on_actionClear_triggered()
{
    for (int c = 0; c < GRID_COLS; ++c) {
        for (int r = 0; r < GRID_ROWS; ++r) {
            if (buttons[c][r]) {
                buttons[c][r]->setBrush(QColor(0,0,0));
            }
        }
    }
    if (livePreview) {
        ui->actionSend->trigger();
    }
}

void MainAnimation::on_actionLive_Preview_triggered(bool checked)
{
    livePreview = checked;
}

void MainAnimation::on_startEdit_valueChanged(int value)
{
    ui->frameSlider->setMinimum(value);
}

void MainAnimation::on_endEdit_valueChanged(int value)
{
    ui->frameSlider->setMaximum(value);
}

void MainAnimation::on_gotoStart_clicked()
{
    for(int fid = ui->curEdit->value() - 1; fid >= ui->startEdit->value(); --fid) {
        if (frameMap.contains(fid)) {
            ui->frameSlider->setValue(fid);
            return;
        }
    }
    ui->frameSlider->setValue(ui->frameSlider->minimum());
}

void MainAnimation::on_gotoPrev_clicked()
{
    if (ui->frameSlider->value() > ui->frameSlider->minimum()) {
        ui->frameSlider->setValue(ui->frameSlider->value() - 1);
    }
    else {
        ui->frameSlider->setValue(ui->frameSlider->maximum());
    }
}

void MainAnimation::on_play_clicked()
{
    if (isPlay.load() == 0) {
        ui->play->setIcon(QIcon(":/icons/Icons/pause.svg"));
        isPlay.store(1);
        playInterval = intervalBox->value();
        QTimer::singleShot(playInterval, this, SLOT(play()));
    }
    else {
        ui->play->setIcon(QIcon(":/icons/Icons/play.svg"));
        isPlay.store(0);
    }
}

void MainAnimation::play()
{
    if (isPlay.load() == 1) {
        on_gotoNext_clicked();
        if (livePreview) {
            ui->actionSend->trigger();
        }
        QTimer::singleShot(playInterval, this, SLOT(play()));
    }
}

void MainAnimation::on_gotoNext_clicked()
{
    if (ui->frameSlider->value() < ui->frameSlider->maximum()) {
        ui->frameSlider->setValue(ui->frameSlider->value() + 1);
    }
    else {
        ui->frameSlider->setValue(ui->frameSlider->minimum());
    }
}

void MainAnimation::on_gotoEnd_clicked()
{
    for(int fid = ui->curEdit->value() + 1; fid <= ui->endEdit->value(); ++fid) {
        if (frameMap.contains(fid)) {
            ui->frameSlider->setValue(fid);
            return;
        }
    }
    ui->frameSlider->setValue(ui->frameSlider->maximum());
}

void MainAnimation::on_curEdit_valueChanged(int val)
{
    //isPlay.store(0);
    //ui->play->setIcon(QIcon(":/icons/Icons/play.svg"));
    next(val);
}

void MainAnimation::next(int val)
{
    if (frameMap.contains(val)) {
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-del.svg"));
        QVector<int> vels = frameMap[val].getColors();
        if (vels.length() != GRID_COLS * GRID_ROWS) {
            return;
        }
        int i=0;
        for (int c = 0; c < GRID_COLS; ++c) {
            for (int r = 0; r < GRID_ROWS; ++r) {
                buttons[c][r]->setBrush(LEDColors::mapToQColor(vels[i]));
                i++;
            }
        }
    }
    else {
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-add.svg"));
        int nval;
        bool found = false;
        for (nval = val; nval >= ui->startEdit->value(); --nval) {
            if (frameMap.contains(nval)) {
                found = true;
                break;
            }
        }
        QVector<int> vels = frameMap[nval].getColors();
        if (vels.length() != GRID_COLS * GRID_ROWS) {
            found = false;
        }

        int i = 0;
        for (int c = 0; c < GRID_COLS; ++c) {
            for (int r = 0; r < GRID_ROWS; ++r) {
                if (found) {
                    buttons[c][r]->setBrush(LEDColors::mapToQColor(vels[i]));
                    i++;
                }
                else {
                    buttons[c][r]->setBrush(QColor(0,0,0));
                }
            }
        }
    }
}

void MainAnimation::on_keyButton_clicked()
{
    if (frameMap.contains(ui->curEdit->value())) {
        frameMap.remove(ui->curEdit->value());
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-add.svg"));
    }
    else {
        Frame frame(ui->curEdit->value(), GRID_COLS, GRID_ROWS);
        for (int c = 0; c < GRID_COLS; ++c) {
            for (int r = 0; r < GRID_ROWS; ++r) {
                int ic = 12;
                if (buttons[c][r]) {
                    QColor cc = buttons[c][r]->brush().color();
                    ic = colors.mapToMidiVelocity(cc);
                }
                frame.addColor(ic);
            }
        }

        frameMap[ui->curEdit->value()] = frame;
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-del.svg"));
    }
}

void MainAnimation::on_action_Open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open...", QString(), "LaunchPad Animation (*.lpa)");
    if (filename.isEmpty()) {
        return;
    }

    Animation anim("", 0);

    if (!anim.load(filename)) {
        QMessageBox::critical(this, "Error", "Can not load or parse file.");
        return;
    }
    setFileName(filename);
    intervalBox->setValue(anim.getInterval());
    deviceCombo->setCurrentText(anim.getDevice());
    ui->startEdit->setValue(anim.getStart());
    ui->endEdit->setValue(anim.getEnd());

    const QVector<Frame>& frames = anim.getFrames();
    frameMap.clear();
    foreach (Frame frame, frames) {
        frameMap[frame.getTime()] = frame;
    }
    if (frameMap.contains(anim.getStart())) {
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-del.svg"));
    }
    else {
        ui->keyButton->setIcon(QIcon(":/icons/Icons/key-add.svg"));
    }
    ui->curEdit->setValue(anim.getStart());
    next(anim.getStart());
}

void MainAnimation::on_action_Save_triggered()
{
    QString filename = currentFile;
    if (filename.isEmpty()) {
        filename = QFileDialog::getSaveFileName(this, "Save...", QString(), "LaunchPad Animation (*.lpa)");
        if (filename.isEmpty()) {
            return;
        }
        setFileName(filename);
    }

    Animation anim(deviceCombo->currentText(), intervalBox->value());
    anim.setStart(ui->startEdit->value());
    anim.setEnd(ui->endEdit->value());
    for (int fid = ui->startEdit->value(); fid <= ui->endEdit->value(); ++fid) {
        if (frameMap.contains(fid)) {
            anim.addFrame(frameMap[fid]);
        }
    }

    anim.save(filename);
}

void MainAnimation::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as...", QString(), "LaunchPad Animation (*.lpa)");
    if (filename.isEmpty()) {
        return;
    }
    setFileName(filename);
    ui->action_Save->trigger();

}

void MainAnimation::setFileName(QString filename)
{
    currentFile = filename;
    if (!filename.isEmpty()) {
        QFileInfo fileInfo(filename);
        setWindowTitle("LaunchPad LED Animator - " + fileInfo.fileName());
    }
    else {
        setWindowTitle("LaunchPad LED Animator");
    }
}

void MainAnimation::on_actionNew_triggered()
{
    isPlay.store(0);
    ui->play->setIcon(QIcon(":/icons/Icons/play.svg"));
    setFileName("");
    int r,c;
    for (c = 0; c < GRID_COLS; ++c) {
        for (r = 0; r < GRID_ROWS; ++r) {
            buttons[c][r]->setBrush(QColor(0,0,0));
        }
    }
    frameMap.clear();

    ui->startEdit->setValue(0);
    ui->endEdit->setValue(100);
    ui->curEdit->setValue(0);
    ui->keyButton->setIcon(QIcon(":/icons/Icons/key-add.svg"));
}

void MainAnimation::on_actionAbout_triggered()
{
    About about;
    about.exec();
}
