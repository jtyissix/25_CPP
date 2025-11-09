#include "figureb_hand.h"

Figureb_hand::Figureb_hand(QGraphicsItem *parent) : Figure(parent,
             ":/Items/Characters/soldier_hold1.png") {

}

void Figureb_hand::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(1.6);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}
