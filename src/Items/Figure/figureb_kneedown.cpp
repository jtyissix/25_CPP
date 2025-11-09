#include "figureb_kneedown.h"

FigureB_KneeDown::FigureB_KneeDown(QGraphicsItem *parent) : Figure(parent,
             ":/Items/Characters/soldier_duck.png") {

}
void FigureB_KneeDown::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(1.6);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}
