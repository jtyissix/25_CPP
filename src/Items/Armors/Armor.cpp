//
// Created by gerw on 8/20/24.
//

#include "Armor.h"
#include <QDateTime>
Armor::Armor(QGraphicsItem *parent, const QString &pixmapPath) : Item(parent, pixmapPath) {
    createTime = QDateTime::currentMSecsSinceEpoch();
}

void Armor::mountToParent(double x,double y) {
    Mountable::mountToParent(x,y);
    setScale(0.5);
    setPos(x, y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x,y);
    }
}

void Armor::unmount() {
    Mountable::unmount();
    setScale(0.8);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(0, -120);
    }
}


bool Armor::getIsPicked(){
    return isPicked;
}

void Armor::setIsPicked(bool isPicked){
    Armor::isPicked=isPicked;
}

void Armor::applyGravity() {
    if (isFalling && !isOnGround) {
        fallingVelocity.setY(fallingVelocity.y() + ARMOR_GRAVITY);
    }
}

bool Armor::getIsFalling() {
    return isFalling;
}

void Armor::setIsFalling(bool falling) {
    isFalling = falling;
    if (!falling) {
        // 如果停止下落，清零垂直速度
        fallingVelocity.setY(0);
    }
}

bool Armor::getIsOnGround() {
    return isOnGround;
}

void Armor::setIsOnGround(bool onGround) {
    isOnGround = onGround;
    if (onGround) {
        setIsFalling(false);
    }
}
void Armor::setBullet(){}
void Armor::effect(){

}
