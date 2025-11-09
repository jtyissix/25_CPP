#include "FigureB.h"

FigureB::FigureB(QGraphicsItem *parent) : Figure(parent,
             ":/Items/Characters/soldier_idle.png") {

}

void FigureB::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(1.6);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}
