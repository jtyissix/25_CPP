#include "bullet.h"

Bullet::Bullet(QGraphicsItem *parent) : Item(parent, ":/Items/Weapons/bullet1.png"){

}
void Bullet::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(0.1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Bullet::setVelocity(const QPointF& vel) {
    velocity = vel;
}

QPointF Bullet::getVelocity() const {
    return velocity;
}
