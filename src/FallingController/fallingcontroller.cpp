#include "fallingcontroller.h"
#include "../Items/Weapons/Rifle.h"
#include "../Items/Weapons/sniper.h"
#include "../Items/Weapons/ballhand.h"
#include "../Items/Weapons/knife.h"
#include "../Items/Drugs/bandage.h"
#include "../Items/Drugs/medbag.h"
#include "../Items/Drugs/medical.h"
#include "../Items/Armors/BulletArmor.h"
#include "../Items/Armors/NormalArmor.h"
#include<random>
#include<QGraphicsScene>
#include<QDateTime>
FallingController::FallingController(QObject *parent)
    : QObject{parent},fallingTimer(new QTimer(this)),vanishTimer(new QTimer(this))
{
    std::srand(time(0));
    connect(fallingTimer, &QTimer::timeout, this, &FallingController::fall);
    connect(vanishTimer, &QTimer::timeout, this, &FallingController::vanish);
}

void FallingController::setScene(QGraphicsScene* scene) {
    parentScene = scene;
}

void FallingController::startLoop() {
    fallingTimer->start(fallingInterval);
    vanishTimer->start(vanishInterval);
}

void FallingController::setFallingParams(int fallingInterval){
    FallingController::fallingInterval=fallingInterval;
}

void FallingController::setVanishParams(int vanishInterval){
    FallingController::vanishInterval=vanishInterval;
}

void FallingController::fall(){
    int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
    int choice = std::rand()%9+1;
    switch(choice){
    case 1:{
        Weapon *rifle = new Rifle();
        rifle->mountToParent(randomX,fallingRange.top());
        rifle->setScene(parentScene);
        rifle->setBullet();
        parentScene->addItem(rifle);
        createdWeapon.push_back(rifle);
        break;
    }
    case 2:{
        Weapon *sniper = new Sniper();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        sniper->mountToParent(randomX,fallingRange.top());
        sniper->setScene(parentScene);
        sniper->setBullet();
        parentScene->addItem(sniper);
        createdWeapon.push_back(sniper);
        break;
    }
    case 3:{
        Weapon *ballHand = new BallHand();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        ballHand->mountToParent(randomX,fallingRange.top());
        ballHand->setScene(parentScene);
        ballHand->setBullet();
        parentScene->addItem(ballHand);
        createdWeapon.push_back(ballHand);
        break;
    }
    case 4:{
        Weapon *knife = new Knife();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        knife->mountToParent(randomX,fallingRange.top());
        knife->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(knife);
        createdWeapon.push_back(knife);
        break;
    }
    case 5:{
        Drug *bandage = new Bandage();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        bandage->mountToParent(randomX,fallingRange.top());
        bandage->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(bandage);
        createdDrug.push_back(bandage);
        break;
    }
    case 6:{
        Drug *medbag = new MedBag();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        medbag->mountToParent(randomX,fallingRange.top());
        medbag->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(medbag);
        createdDrug.push_back(medbag);
        break;
    }
    case 7:{
        Drug *medical = new Medical();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        medical->mountToParent(randomX,fallingRange.top());
        medical->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(medical);
        createdDrug.push_back(medical);
        break;
    }
    case 8:{
        Armor *normArmor = new NormalArmor();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        normArmor->mountToParent(randomX,fallingRange.top());
        normArmor->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(normArmor);
        createdArmor.push_back(normArmor);
        break;
    }
    case 9:{
        Armor *normArmor = new BulletArmor();
        //int randomX = rand()%(int)(fallingRange.right()-fallingRange.left()-250)+fallingRange.left();
        normArmor->mountToParent(randomX,fallingRange.top());
        normArmor->setScene(parentScene);
        //ballHand->setBullet();
        parentScene->addItem(normArmor);
        createdArmor.push_back(normArmor);
        break;
    }
    }
}

void FallingController::processMovement(){
    if (!parentScene) return;

    // 获取当前帧时间间隔（假设从场景获取）
    // 如果无法获取，使用固定值
    qreal deltaTime = 16.67; // 约60FPS (1000/60)

    auto it = createdWeapon.begin();
    while(it != createdWeapon.end()) {
        Weapon* weapon = *it;

        if(weapon != nullptr && !weapon->getIsPicked()) {
            // 只有未被捡起的武器才处理物理
            QPointF currentPos = weapon->pos();

            // 应用重力
            weapon->applyGravity();

            // 获取当前速度
            QPointF velocity = weapon->getFallingVelocity();

            // 计算新位置
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);

            // 边界检测 - 左右边界
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            }

            // 地面检测
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                weapon->setIsOnGround(true);
                weapon->setIsFalling(false);
                // 停止所有运动
                weapon->setFallingVelocity(QPointF(0, 0));


            } else {
                weapon->setIsOnGround(false);
            }

            // 更新武器位置
            weapon->setPos(newPos);
        }

        ++it;
    }



    auto itD = createdDrug.begin();
    while(itD != createdDrug.end()) {
        Drug* weapon = *itD;

        if(weapon != nullptr && !weapon->getIsPicked()) {
            // 只有未被捡起的武器才处理物理
            QPointF currentPos = weapon->pos();

            // 应用重力
            weapon->applyGravity();

            // 获取当前速度
            QPointF velocity = weapon->getFallingVelocity();

            // 计算新位置
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);

            // 边界检测 - 左右边界
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            }

            // 地面检测
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                weapon->setIsOnGround(true);
                weapon->setIsFalling(false);
                // 停止所有运动
                weapon->setFallingVelocity(QPointF(0, 0));

            } else {
                weapon->setIsOnGround(false);
            }

            // 更新武器位置
            weapon->setPos(newPos);
        }

        ++itD;
    }



    auto itA = createdArmor.begin();
    while(itA != createdArmor.end()) {
        Armor* weapon = *itA;

        if(weapon != nullptr && !weapon->getIsPicked()) {
            // 只有未被捡起的武器才处理物理
            QPointF currentPos = weapon->pos();

            // 应用重力
            weapon->applyGravity();

            // 获取当前速度
            QPointF velocity = weapon->getFallingVelocity();

            // 计算新位置
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);

            // 边界检测 - 左右边界
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            }

            // 地面检测
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                weapon->setIsOnGround(true);
                weapon->setIsFalling(false);
                // 停止所有运动
                weapon->setFallingVelocity(QPointF(0, 0));


            } else {
                weapon->setIsOnGround(false);
            }

            // 更新武器位置
            weapon->setPos(newPos);
        }

        ++itA;
    }
}

void FallingController::setFallingRange(QRectF range){
    FallingController::fallingRange = range;

}

void FallingController::vanish(){
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    auto it = createdWeapon.begin();
    int removedCount = 0;

    while(it != createdWeapon.end()) {
        Weapon* weapon = *it;

        if(weapon != nullptr) {
            // 检查武器是否超时且未被捡起
            bool isExpired = (currentTime - weapon->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !weapon->getIsPicked(); // 假设 isMounted() 表示武器被捡起

            if(isExpired && isNotPicked) {
                // 武器超时且未被捡起，删除它
                parentScene->removeItem(weapon);
                delete weapon;
                it = createdWeapon.erase(it);
                removedCount++;

            } else {
                ++it;
            }
        } else {
            // 空指针，直接移除
            it = createdWeapon.erase(it);
        }
    }

    if(removedCount > 0) {

    }


    auto itD = createdDrug.begin();
    while(itD != createdDrug.end()) {
        Drug* weapon = *itD;

        if(weapon != nullptr) {
            // 检查武器是否超时且未被捡起
            bool isExpired = (currentTime - weapon->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !weapon->getIsPicked(); // 假设 isMounted() 表示武器被捡起

            if(isExpired && isNotPicked) {
                // 武器超时且未被捡起，删除它
                parentScene->removeItem(weapon);
                delete weapon;
                itD = createdDrug.erase(itD);
                removedCount++;


            } else {
                ++itD;
            }
        } else {
            // 空指针，直接移除
            itD = createdDrug.erase(itD);
        }
    }

    auto itA = createdArmor.begin();
    while(itA != createdArmor.end()) {
        Armor* weapon = *itA;

        if(weapon != nullptr) {
            // 检查武器是否超时且未被捡起
            bool isExpired = (currentTime - weapon->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !weapon->getIsPicked(); // 假设 isMounted() 表示武器被捡起

            if(isExpired && isNotPicked) {
                // 武器超时且未被捡起，删除它
                parentScene->removeItem(weapon);
                delete weapon;
                itA = createdArmor.erase(itA);
                removedCount++;


            } else {
                ++itA;
            }
        } else {
            // 空指针，直接移除
            itA = createdArmor.erase(itA);
        }
    }
}

void FallingController::stopAllTimers() {
    if (fallingTimer) {
        fallingTimer->stop();

    }

    if (vanishTimer) {
        vanishTimer->stop();

    }
}

