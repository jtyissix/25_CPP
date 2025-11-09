//
// Created by gerw on 8/20/24.
//

#include "Figure.h"

Figure::Figure(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {

}

void Figure::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(0.8);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Figure::unmount() {
    Mountable::unmount();
    setScale(0.8);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(0, -120);
    }
}


