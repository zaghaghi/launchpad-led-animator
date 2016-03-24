#include "animationgrid.h"
#include "ui_animationgrid.h"
#include <QGraphicsScene>
#include <QMessageBox>
#include <launchpadcontroller.h>

AnimationGrid::AnimationGrid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationGrid)
{
    ui->setupUi(this);

}

AnimationGrid::~AnimationGrid()
{
    delete ui;
}

void AnimationGrid::mousePressEvent(QMouseEvent* event)
{
    QPoint remapped = ui->buttonGrid->mapFromParent( event->pos() );
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
        button->setBrush(ui->activeColorList->item(0)->background());
    }
}

void AnimationGrid::on_colorList_currentRowChanged(int )
{
}

void AnimationGrid::on_sendBtn_clicked()
{


}
