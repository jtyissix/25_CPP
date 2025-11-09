//
// Created by gerw on 8/20/24.
//

#include "Weapon.h"
#include<QDateTime>
Weapon::Weapon(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {
    createTime = QDateTime::currentMSecsSinceEpoch(); // 记录创建时间
}

void Weapon::mountToParent(double x,double y) {
    Mountable::mountToParent(x,y);
    setScale(1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

bool Weapon::getIsPicked(){
    return isPicked;
}

void Weapon::setIsPicked(bool isPicked){
    Weapon::isPicked=isPicked;
}

void Weapon::applyGravity() {
    if (isFalling && !isOnGround) {
        fallingVelocity.setY(fallingVelocity.y() + WEAPON_GRAVITY);
    }
}

bool Weapon::getIsFalling() {
    return isFalling;
}

void Weapon::setIsFalling(bool falling) {
    isFalling = falling;
    if (!falling) {
        // 如果停止下落，清零垂直速度
        fallingVelocity.setY(0);
    }
}

bool Weapon::getIsOnGround() {
    return isOnGround;
}

void Weapon::setIsOnGround(bool onGround) {
    isOnGround = onGround;
    if (onGround) {
        setIsFalling(false);
    }
}
void Weapon::setBullet(){}
void Weapon::attack(){

}
