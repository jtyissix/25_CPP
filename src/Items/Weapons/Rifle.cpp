//
// Created by gerw on 8/20/24.
//

#include "Rifle.h"
#include "../Characters/Character.h"
#include<QGraphicsScene>
#include<QDateTime>
Rifle::Rifle(QGraphicsItem *parent) :Weapon(parent,
                                                                  ":/Items/Weapons/02.png") {

}

void Rifle::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(0.1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Rifle::attack(){
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - lastShotTime < SHOOT_COOLDOWN) {
        qDebug() << "🔫 Rifle cooling down... Time left:" << (SHOOT_COOLDOWN - (currentTime - lastShotTime)) << "ms";
        return;
    }
    auto currentBullet=bulletSet[bulletUsage];
    currentBullet->setScale(0.05);
    QPointF rifleScenePos = mapToScene(QPointF(0, 0));
    // 根据枪的方向设置子弹初始位置和速度
    auto character = dynamic_cast<Character*>(parentItem());

    bool facingRight = character->getDirection();

    QPointF bulletStartPos;
    QPointF bulletVelocity;

    if (facingRight) {
        // 面向右，子弹从枪口右侧发射
        bulletStartPos = rifleScenePos + QPointF(70, 0);
        bulletVelocity = QPointF(BULLET_SPEED, 0);
    } else {
        // 面向左，子弹从枪口左侧发射
        currentBullet->setTransform(QTransform::fromScale(-1, 1));
        bulletStartPos = rifleScenePos + QPointF(-70, 0);
        bulletVelocity = QPointF(-BULLET_SPEED, 0);
    }

    // 设置子弹位置和速度
    currentBullet->setPos(bulletStartPos);
    currentBullet->setVelocity(bulletVelocity);

    // 添加到场景中
    parentScene->addItem(currentBullet);
    bulletUsage++;
    lastShotTime = currentTime;
    qDebug()<<bulletUsage;
    if(bulletUsage==NUMBER_OF_BULLET){
        character->getWeapon()->setParentItem(nullptr);
        if(character->getWeapon()->scene()!=nullptr)
        {
            character->getWeapon()->scene()->removeItem(character->getWeapon());
        }
        //limitation: RAM leak
        //character->getWeapon()->deleteLater();
        character->setWeapon(nullptr);

    }
    return;
}

void Rifle::setBullet(){
    for(int i=0;i<NUMBER_OF_BULLET;i++){
        bulletSet.push_back(new Bullet(nullptr));
        bulletSet[bulletSet.size()-1]->setScene(parentScene);
        bulletSet[bulletSet.size()-1]->hurt=HURT;
    }
}

void Rifle::setBulletCharacter(){
    for (auto it = bulletSet.begin(); it != bulletSet.end(); ++it) {
        (*it)->sourceID=dynamic_cast<Character*>(parentItem())->id;
    }
}
