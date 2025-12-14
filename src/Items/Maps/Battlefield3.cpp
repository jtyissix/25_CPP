#include "Battlefield3.h"

Battlefield3::Battlefield3(QGraphicsItem *parent) : Map(parent, ":/Items/background/bg3.png") {}

qreal Battlefield3::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return sceneRect.bottom();
}
