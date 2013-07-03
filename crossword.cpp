#include "crossword.h"

crossword::crossword(QWidget *parent) :
    QWidget(parent)
{
    cell_size=15;
    border1=15;
}

void crossword::paintEvent(QPaintEvent *e){
    if(pixmap!=NULL){
        QPainter p(this);
        p.drawPixmap(0,0,width(),height(), *pixmap);
        p.end();
    }
}

void crossword::setSize(short wCells, short hCells){
    delete pixmap;
    w_cells=wCells;
    h_cells=hCells;
    matrix = new bool[w_cells*h_cells];
    for(int i=0;i<w_cells*h_cells;i++) matrix[i]=false;
    draw_layout();
    repaint();
}

void crossword::draw_layout(){
    int w = w_cells*cell_size+2*border1;
    int h = h_cells*cell_size+2*border1;
    this->setMinimumSize(w, h);
    this->setMaximumSize(w, h);
    pixmap = new QPixmap(width(), height());
    pixmap->fill(Qt::white);
    QPainter p(pixmap);
    p.setPen(QPen(Qt::black, 2));
    p.drawRect(1,1,width()-3,height()-3);
    p.setPen(QPen(Qt::gray, 1));
    int a, b=h_cells*cell_size;
    for(int i=1;i<w_cells;i++){
        a=border1+cell_size*i;
        p.drawLine(a, border1, a, b+border1);
    }
    p.setPen(QPen(Qt::gray, 2));
    for(int i=5;i<w_cells;i+=5){
        a=border1+cell_size*i;
        p.drawLine(a, border1, a, b+border1);
    }

    b=w_cells*cell_size;
    p.setPen(QPen(Qt::gray, 1));
    for(int i=1;i<h_cells;i++){
        a=border1+cell_size*i;
        p.drawLine(border1, a, b+border1, a);
    }
    p.setPen(QPen(Qt::gray, 2));
    for(int i=5;i<h_cells;i+=5){
        a=border1+cell_size*i;
        p.drawLine(border1, a, b+border1, a);
    }

    p.setPen(QPen(Qt::black, 2));
    b=h_cells*cell_size;
    p.drawLine(border1, border1, border1, border1+b);
    p.drawLine(border1+cell_size*w_cells, border1, border1+cell_size*w_cells, b+border1);
    b=w_cells*cell_size;
    p.drawLine(border1, border1, border1+b, border1);
    p.drawLine(border1, border1+cell_size*h_cells, b+border1, border1+cell_size*h_cells);
    p.end();
}

void crossword::set_cell_size(short size){
    cell_size=size;
}

void crossword::mousePressEvent(QMouseEvent *e){

}

crossword::~crossword(){
    delete [] matrix;
}
