#ifndef FIGUREB_HAND_H
#define FIGUREB_HAND_H

#include "Figure.h"

class Figureb_hand : public Figure
{
public:
    explicit Figureb_hand(QGraphicsItem *parent = nullptr);
    void mountToParent(qreal x, qreal y);
};

#endif // FIGUREB_HAND_H
