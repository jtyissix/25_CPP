#include "grass.h"

Grass::Grass(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {

}

void Grass::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(0.06);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}
bool Grass::isCharacterInGrass(const Character* character, qreal characterWidth) const {
    QRectF grassArea = getGrassArea();
    QRectF characterArea=character->getCollisionBounds();
    return grassArea.intersects(characterArea);
}

QRectF Grass::getGrassArea() const {
    // 草地的有效隐身区域，考虑缩放
    QRectF bounds = sceneBoundingRect();
    // 适当扩大一些隐身范围，让角色更容易隐身
    return bounds;
}
