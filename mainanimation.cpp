#include "mainanimation.h"
#include "ui_mainanimation.h"
#include "about.h"
#include "launchpadcontroller.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QTimer>
#include <QFileInfo>

MainAnimation::MainAnimation(LaunchpadController* controller, MainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainAnimation),
    livePreview(true),
    playInterval(0),
    currentFile(""),
    trigger(-1),
    controller(controller),
    parent(parent)
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
    removeToolBar(ui->toolBar);
    addToolBar(Qt::LeftToolBarArea, ui->toolBar);
    ui->toolBar->show();
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
    ui->frameSlider->setMinimum(ui->startEdit->value());
    ui->frameSlider->setMaximum(ui->endEdit->value());

    checkReceivedKeys.setInterval(50);
    QObject::connect(&checkReceivedKeys, SIGNAL(timeout()), this, SLOT(processReceivedKeys()));

    ui->actionNew->trigger();
}

void MainAnimation::save()
{
    QString filename = currentFile;
    if (filename.isEmpty()) {
        filename = QFileDialog::getSaveFileName(this, "Save...", QString(), "LaunchPad Animation (*.lpa)");
        if (filename.isEmpty()) {
            return;
        }
    }
    setFileName(filename);

    //Animation anim(inputDeviceCombo->currentText(), outputDeviceCombo->currentText(), intervalBox->value());
    Animation anim("", "", 200);
    anim.setStart(ui->startEdit->value());
    anim.setEnd(ui->endEdit->value());
    anim.setTrigger(trigger);
    for (int fid = ui->startEdit->value(); fid <= ui->endEdit->value(); ++fid) {
        if (frameMap.contains(fid)) {
            anim.addFrame(frameMap[fid]);
        }
    }

    anim.save(filename);
}

void MainAnimation::open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open...", QString(), "LaunchPad Animation (*.lpa)");
    if (filename.isEmpty()) {
        return;
    }

    Animation anim;

    if (!anim.load(filename)) {
        QMessageBox::critical(this, "Error", "Can not load or parse file.");
        return;
    }
    setFileName(filename);
    //intervalBox->setValue(anim.getInterval());
    //outputDeviceCombo->setCurrentText(anim.getOutput());
    //inputDeviceCombo->setCurrentText(anim.getInput());
    ui->startEdit->setValue(anim.getStart());
    ui->endEdit->setValue(anim.getEnd());
    trigger = anim.getTrigger();
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
    if (trigger >= 0) {
        ui->actionSet_Trigger->setIcon(QIcon(":/icons/Icons/triggered.svg"));
    }
    else {
        ui->actionSet_Trigger->setIcon(QIcon(":/icons/Icons/trigger.svg"));
    }
    ui->curEdit->setValue(anim.getStart());
    next(anim.getStart());
}

void MainAnimation::saveas()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as...", QString(), "LaunchPad Animation (*.lpa)");
    if (filename.isEmpty()) {
        return;
    }
    setFileName(filename);
    save();
}
#include <iostream>

void MainAnimation::doclose()
{
    std::cout << windowTitle().toStdString() << std::endl;
    if (windowTitle().endsWith('*')) {
        if (QMessageBox::question(this, "Unsaved work", "Do you want to save before close?") == QMessageBox::Yes) {
            save();
        }
    }
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
    static QVector<int> oldColors;
    int i;
    if (oldColors.size() == 0) {
        for (i = 0; i < GRID_COLS * GRID_ROWS; ++i) {
            oldColors.push_back(12);
        }
    }
    controller->connect(parent->getOutputDeviceId());
    LaunchpadController::LayoutType layout = (LaunchpadController::LayoutType) parent->getLayout();

    i = 0;
    for (int c = 0; c < GRID_COLS; ++c) {
        for (int r = 0; r < GRID_ROWS; ++r) {
            if (buttons[c][r]) {
                QColor cc = buttons[c][r]->brush().color();
                int ic = colors.mapToMidiVelocity(cc);
                if (ic != oldColors[i]) {
                    controller->sendColor(ic, c, r, layout);
                    oldColors[i] = ic;
                }
                i++;
            }
        }
    }
    controller->disconnect();
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
        setFileName(currentFile, false);
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
        playInterval = parent->getInterval();
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
        int pFrame = ui->frameSlider->value();
        on_gotoNext_clicked();
        int nFrame = ui->frameSlider->value();
        if (livePreview) {
            ui->actionSend->trigger();
        }
        if (nFrame > pFrame || ui->actionPlay_Indifinitly->isChecked()) {
            QTimer::singleShot(playInterval, this, SLOT(play()));
        }
        else {
            ui->play->setIcon(QIcon(":/icons/Icons/play.svg"));
            isPlay.store(0);
        }
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

void MainAnimation::setFileName(QString filename, bool saved)
{
    if (!filename.startsWith("Unsaved")) {
        currentFile = filename;
    }
    if (!filename.isEmpty()) {
        QFileInfo fileInfo(filename);
        setWindowTitle(fileInfo.fileName() + (saved?QString():QString(" *")));
    }
    else {
        setWindowTitle(saved?"Unsaved":"Unsaved *");
    }
}

//void MainAnimation::on_actionNew_triggered()
//{
//    isPlay.store(0);
//    ui->play->setIcon(QIcon(":/icons/Icons/play.svg"));
//    setFileName("");
//    int r,c;
//    for (c = 0; c < GRID_COLS; ++c) {
//        for (r = 0; r < GRID_ROWS; ++r) {
//            buttons[c][r]->setBrush(QColor(0,0,0));
//            buttons[c][r]->setPen(QPen(QColor(0,0,0),1));
//        }
//    }
//    frameMap.clear();
//    trigger = -1;
//    ui->startEdit->setValue(0);
//    ui->endEdit->setValue(100);
//    ui->curEdit->setValue(0);
//    ui->keyButton->setIcon(QIcon(":/icons/Icons/key-add.svg"));
//    ui->actionSet_Trigger->setIcon(QIcon(":/icons/Icons/trigger.svg"));
//}

void MainAnimation::on_actionAbout_triggered()
{
}

void MainAnimation::warning(const QString& title, const QString& text)
{
    QMessageBox msgBox;
    msgBox.setParent(this);
    msgBox.setIconPixmap(QPixmap(":/icons/Icons/warning.svg"));
    msgBox.setWindowTitle(title);
    msgBox.setWindowModality(Qt::ApplicationModal);
    msgBox.setText(text);
    msgBox.exec();
}

void MainAnimation::on_actionSet_Trigger_triggered(bool checked)
{
    if (!checked) {
        ui->actionSet_Trigger->setIcon(QIcon(":/icons/Icons/trigger.svg"));
    }
}

void MainAnimation::on_actionSet_Trigger_toggled(bool toggled)
{
    ui->actionListen->setEnabled(!toggled);
    if (toggled) {
        controller->startListen(parent->getInputDeviceId(),
                               (LaunchpadController::LayoutType) parent->getLayout(),
                               onLaunchpadKeyDown, this);
        checkReceivedKeys.start();
    }
    else {
        controller->stopListen();
        checkReceivedKeys.stop();
    }
}

void MainAnimation::processReceivedKeys()
{
    mutex.lock();
    if (receivedKeys.size() > 0) {
        char key = receivedKeys.front();
        receivedKeys.clear();
        if (ui->actionSet_Trigger->isChecked()) {
            ui->actionSet_Trigger->setChecked(false);
            ui->actionSet_Trigger->setIcon(QIcon(":/icons/Icons/triggered.svg"));
            trigger = key;
        }
        else if (ui->actionListen->isChecked()) {
            if (trigger == (int)key && isPlay.load() == 0) {
                ui->curEdit->setValue(ui->startEdit->value());
                on_play_clicked();
            }
        }
    }
    mutex.unlock();
}

void MainAnimation::on_actionListen_toggled(bool toggled)
{
    ui->actionSet_Trigger->setEnabled(!toggled);
    if (toggled) {
        controller->startListen(parent->getInputDeviceId(),
                               (LaunchpadController::LayoutType) parent->getInterval(),
                               onLaunchpadKeyDown, this);
        checkReceivedKeys.start();
        ui->actionListen->setIcon(QIcon(":/icons/Icons/listening.svg"));
    }
    else {
        controller->stopListen();
        checkReceivedKeys.stop();
        ui->actionListen->setIcon(QIcon(":/icons/Icons/listen.svg"));
    }
}

void MainAnimation::onLaunchpadKeyDown(char key, void *data)
{
    MainAnimation* thiz = (MainAnimation*) data;
    thiz->mutex.lock();
    thiz->receivedKeys.push_back(key);
    thiz->mutex.unlock();
}

