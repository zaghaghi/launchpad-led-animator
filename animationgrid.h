#ifndef ANIMATIONGRID_H
#define ANIMATIONGRID_H

#include <QDialog>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <ledcolors.h>

namespace Ui {
class AnimationGrid;
}

#define GRID_COLS   8
#define GRID_ROWS   8
#define GRID_PAD    5
#define GRID_WIDTH  70
#define GRID_HEIGHT 70


class AnimationGrid : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationGrid(QWidget *parent = 0);
    ~AnimationGrid();
    void mousePressEvent(QMouseEvent* event);


private slots:
    void on_colorList_currentRowChanged(int currentRow);

    void on_sendBtn_clicked();

private:
    Ui::AnimationGrid *ui;

    QGraphicsRectItem* buttons[GRID_COLS][GRID_ROWS];

    LEDColors colors;
};

#endif // ANIMATIONGRID_H
