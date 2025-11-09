#include "ball.h"


Ball::Ball(QGraphicsItem *parent) : Item(parent, ":/Items/Weapons/bullet4.png"){

}
void Ball::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(0.1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Ball::setVelocity(const QPointF& vel) {
    velocity = vel;
}

QPointF Ball::getVelocity() const {
    return velocity;
}

void Ball::applyGravity(){
    velocity.setY(velocity.y()+GRAVITY);
}
