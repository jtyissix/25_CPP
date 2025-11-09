#include "drug.h"
#include<QDateTime>
Drug::Drug(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {
    createTime = QDateTime::currentMSecsSinceEpoch(); // 记录创建时间
}

void Drug::mountToParent(double x,double y) {
    Mountable::mountToParent(x,y);
    setScale(1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

bool Drug::getIsPicked(){
    return isPicked;
}

void Drug::setIsPicked(bool isPicked){
    Drug::isPicked=isPicked;
}

void Drug::applyGravity() {
    if (isFalling && !isOnGround) {
        fallingVelocity.setY(fallingVelocity.y() + DRUG_GRAVITY);
    }
}

bool Drug::getIsFalling() {
    return isFalling;
}

void Drug::setIsFalling(bool falling) {
    isFalling = falling;
    if (!falling) {
        // 如果停止下落，清零垂直速度
        fallingVelocity.setY(0);
    }
}

bool Drug::getIsOnGround() {
    return isOnGround;
}

void Drug::setIsOnGround(bool onGround) {
    isOnGround = onGround;
    if (onGround) {
        setIsFalling(false);
    }
}
void Drug::setBullet(){}
void Drug::effect(){

}

