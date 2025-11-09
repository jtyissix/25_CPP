#ifndef FIGUREB_KNEEDOWN_H
#define FIGUREB_KNEEDOWN_H

#include "Figure.h"

class FigureB_KneeDown : public Figure {
public:
    explicit FigureB_KneeDown(QGraphicsItem *parent = nullptr);
    void mountToParent(qreal x, qreal y);
};

#endif // FIGUREB_KNEEDOWN_H
