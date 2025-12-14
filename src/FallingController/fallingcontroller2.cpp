#include "fallingcontroller2.h"
#include "../Items/Weapons/Rifle.h"
#include "../Items/Weapons/sniper.h"
#include "../Items/Weapons/ballhand.h"
#include "../Items/Weapons/knife.h"
#include "../Items/Drugs/bandage.h"
#include "../Items/Drugs/medbag.h"
#include "../Items/Drugs/medical.h"
#include "../Items/Armors/BulletArmor.h"
#include "../Items/Armors/NormalArmor.h"
#include <random>
#include <QGraphicsScene>
#include <QDateTime>

Bomb::Bomb(QGraphicsItem *parent) : QGraphicsEllipseItem(0, 0, 40, 40, parent) {
    setBrush(QBrush(Qt::black));
    setPen(QPen(Qt::red, 3));
    createTime = QDateTime::currentMSecsSinceEpoch();
}

void Bomb::applyGravity() {
    if (isFalling && !isOnGround) {
        fallingVelocity.setY(fallingVelocity.y() + BOMB_GRAVITY);
    }
}

FallingController2::FallingController2(QObject *parent)
    : QObject{parent}, fallingTimer(new QTimer(this)), vanishTimer(new QTimer(this))
{
    std::srand(time(0));
    connect(fallingTimer, &QTimer::timeout, this, &FallingController2::fall);
    connect(vanishTimer, &QTimer::timeout, this, &FallingController2::vanish);
}

void FallingController2::setScene(QGraphicsScene* scene) {
    parentScene = scene;
}

void FallingController2::startLoop() {
    fallingTimer->start(fallingInterval);
    vanishTimer->start(vanishInterval);
}

void FallingController2::setFallingParams(int fallingInterval){
    FallingController2::fallingInterval = fallingInterval;
}

void FallingController2::setVanishParams(int vanishInterval){
    FallingController2::vanishInterval = vanishInterval;
}

void FallingController2::fall(){
    int randomX = rand() % (int)(fallingRange.right() - fallingRange.left() - 250) + fallingRange.left();
    int choice = std::rand() % 4 + 1;
    
    switch(choice){
    case 1:{
        Drug *medbag = new MedBag();
        medbag->mountToParent(randomX, fallingRange.top());
        medbag->setScene(parentScene);
        parentScene->addItem(medbag);
        createdDrug.push_back(medbag);
        break;
    }
    case 2:{
        Drug *medical = new Medical();
        medical->mountToParent(randomX, fallingRange.top());
        medical->setScene(parentScene);
        parentScene->addItem(medical);
        createdDrug.push_back(medical);
        break;
    }
    case 3:{
        Drug *bandage = new Bandage();
        bandage->mountToParent(randomX, fallingRange.top());
        bandage->setScene(parentScene);
        parentScene->addItem(bandage);
        createdDrug.push_back(bandage);
        break;
    }
    case 4:{
        Bomb *bomb = new Bomb();
        bomb->setPos(randomX, fallingRange.top());
        bomb->setCreateTime(QDateTime::currentMSecsSinceEpoch());
        parentScene->addItem(bomb);
        createdBombs.push_back(bomb);
        break;
    }
    }
}

void FallingController2::processMovement(){
    if (!parentScene) return;
    qreal deltaTime = 16.67;

    auto it = createdWeapon.begin();
    while(it != createdWeapon.end()) {
        Weapon* weapon = *it;
        if(weapon != nullptr && !weapon->getIsPicked()) {
            QPointF currentPos = weapon->pos();
            weapon->applyGravity();
            QPointF velocity = weapon->getFallingVelocity();
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);
            
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                weapon->setFallingVelocity(velocity);
            }
            
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                weapon->setIsOnGround(true);
                weapon->setIsFalling(false);
                weapon->setFallingVelocity(QPointF(0, 0));
            } else {
                weapon->setIsOnGround(false);
            }
            
            weapon->setPos(newPos);
        }
        ++it;
    }

    auto itD = createdDrug.begin();
    while(itD != createdDrug.end()) {
        Drug* drug = *itD;
        if(drug != nullptr && !drug->getIsPicked()) {
            QPointF currentPos = drug->pos();
            drug->applyGravity();
            QPointF velocity = drug->getFallingVelocity();
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);
            
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                drug->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                drug->setFallingVelocity(velocity);
            }
            
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                drug->setIsOnGround(true);
                drug->setIsFalling(false);
                drug->setFallingVelocity(QPointF(0, 0));
            } else {
                drug->setIsOnGround(false);
            }
            
            drug->setPos(newPos);
        }
        ++itD;
    }

    auto itA = createdArmor.begin();
    while(itA != createdArmor.end()) {
        Armor* armor = *itA;
        if(armor != nullptr && !armor->getIsPicked()) {
            QPointF currentPos = armor->pos();
            armor->applyGravity();
            QPointF velocity = armor->getFallingVelocity();
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);
            
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                armor->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                armor->setFallingVelocity(velocity);
            }
            
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                armor->setIsOnGround(true);
                armor->setIsFalling(false);
                armor->setFallingVelocity(QPointF(0, 0));
            } else {
                armor->setIsOnGround(false);
            }
            
            armor->setPos(newPos);
        }
        ++itA;
    }
    
    auto itB = createdBombs.begin();
    while(itB != createdBombs.end()) {
        Bomb* bomb = *itB;
        if(bomb != nullptr) {
            QPointF currentPos = bomb->pos();
            bomb->applyGravity();
            QPointF velocity = bomb->getFallingVelocity();
            QPointF newPos = currentPos + velocity * (deltaTime / 1000.0);
            
            if (newPos.x() < fallingRange.left()) {
                newPos.setX(fallingRange.left());
                velocity.setX(0);
                bomb->setFallingVelocity(velocity);
            } else if (newPos.x() > fallingRange.right()) {
                newPos.setX(fallingRange.right());
                velocity.setX(0);
                bomb->setFallingVelocity(velocity);
            }
            
            if (newPos.y() >= groundLevel) {
                newPos.setY(groundLevel);
                bomb->setIsOnGround(true);
                bomb->setIsFalling(false);
                bomb->setFallingVelocity(QPointF(0, 0));
            } else {
                bomb->setIsOnGround(false);
            }
            
            bomb->setPos(newPos);
        }
        ++itB;
    }
}

void FallingController2::setFallingRange(QRectF range){
    FallingController2::fallingRange = range;
}

void FallingController2::vanish(){
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    auto it = createdWeapon.begin();
    int removedCount = 0;
    
    while(it != createdWeapon.end()) {
        Weapon* weapon = *it;
        if(weapon != nullptr) {
            bool isExpired = (currentTime - weapon->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !weapon->getIsPicked();
            
            if(isExpired && isNotPicked) {
                parentScene->removeItem(weapon);
                delete weapon;
                it = createdWeapon.erase(it);
                removedCount++;
            } else {
                ++it;
            }
        } else {
            it = createdWeapon.erase(it);
        }
    }
    
    auto itD = createdDrug.begin();
    while(itD != createdDrug.end()) {
        Drug* drug = *itD;
        if(drug != nullptr) {
            bool isExpired = (currentTime - drug->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !drug->getIsPicked();
            
            if(isExpired && isNotPicked) {
                parentScene->removeItem(drug);
                delete drug;
                itD = createdDrug.erase(itD);
                removedCount++;
            } else {
                ++itD;
            }
        } else {
            itD = createdDrug.erase(itD);
        }
    }
    
    auto itA = createdArmor.begin();
    while(itA != createdArmor.end()) {
        Armor* armor = *itA;
        if(armor != nullptr) {
            bool isExpired = (currentTime - armor->getCreateTime()) > weaponLifetime;
            bool isNotPicked = !armor->getIsPicked();
            
            if(isExpired && isNotPicked) {
                parentScene->removeItem(armor);
                delete armor;
                itA = createdArmor.erase(itA);
                removedCount++;
            } else {
                ++itA;
            }
        } else {
            itA = createdArmor.erase(itA);
        }
    }
    
    auto itB = createdBombs.begin();
    while(itB != createdBombs.end()) {
        Bomb* bomb = *itB;
        if(bomb != nullptr) {
            bool isExpired = (currentTime - bomb->getCreateTime()) > weaponLifetime;
            
            if(isExpired) {
                parentScene->removeItem(bomb);
                delete bomb;
                itB = createdBombs.erase(itB);
                removedCount++;
            } else {
                ++itB;
            }
        } else {
            itB = createdBombs.erase(itB);
        }
    }
}

void FallingController2::stopAllTimers() {
    if (fallingTimer) {
        fallingTimer->stop();
    }
    if (vanishTimer) {
        vanishTimer->stop();
    }
}
