#ifndef FIGUREB_H
#define FIGUREB_H

#include "Figure.h"

class FigureB : public Figure {
public:
    explicit FigureB(QGraphicsItem *parent = nullptr);
    void mountToParent(qreal x, qreal y);
};


#endif // FIGUREB_H
