#ifndef CROSSWORD_H
#define CROSSWORD_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QSizePolicy>
#include <QDebug>

class crossword : public QWidget
{
    Q_OBJECT
public:
    explicit crossword(QWidget *parent = 0);
    ~crossword();
    void setSize(short wCells, short hCells);
    void draw_layout();
    void set_cell_size(short size);

private:
    QPixmap *pixmap;
    unsigned  short w_cells, h_cells, cell_size, border1;
    bool *matrix;

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent *e);
    
signals:
    
public slots:
    
};

#endif // CROSSWORD_H
