#include "Battlefield2.h"

Battlefield2::Battlefield2(QGraphicsItem *parent) : Map(parent, ":/Items/background/bg2_level.jpg") {}

qreal Battlefield2::getFloorHeight() {
    auto sceneRect = sceneBoundingRect();
    return sceneRect.bottom();
}
