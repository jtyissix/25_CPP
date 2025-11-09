#include "ice.h"
Ice::Ice(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {

}

void Ice::mountToParent(qreal x, qreal y) {
    Mountable::mountToParent(x,y);
    setScale(0.5);
    Item::setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}
bool Ice::isCharacterInIce(const Character* character, qreal characterWidth) const {
    QRectF IceArea = getIceArea();
    QRectF characterArea=character->getCollisionBounds();

    return IceArea.intersects(characterArea);
}

QRectF Ice::getIceArea() const {
    // 草地的有效隐身区域，考虑缩放
    QRectF bounds = sceneBoundingRect();
    // 适当扩大一些隐身范围，让角色更容易隐身
    return QRectF(bounds.x()+300 , bounds.y(),  // 向上扩展100像素
                  bounds.width() , bounds.height() + 200);
}
