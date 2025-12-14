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
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>

Bomb::Bomb(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // TODO: 替换为你的炸弹图片路径
    setPixmap(QPixmap(":/Items/mine0.jpg").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    explodePlayer = new QMediaPlayer(this);
    explodeOutput = new QAudioOutput(this);
    explodePlayer->setAudioOutput(explodeOutput);
    // TODO: 替换为你的爆炸音效路径
    explodePlayer->setSource(QUrl("qrc:/mineexplode.wav"));
    explodeOutput->setVolume(0.6);
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

        qint64 randomDelay = 1000 + (rand() % 3000);
        bomb->setExplodeTime(bomb->getCreateTime() + randomDelay);

        parentScene->addItem(bomb);
        createdBombs.push_back(bomb);
        break;
    }
    }
}

void FallingController2::processMovement(){
    if (!parentScene) return;
    qreal deltaTime = 16.67;

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

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    auto itB = createdBombs.begin();
    while(itB != createdBombs.end()) {
        Bomb* bomb = *itB;
        if(bomb != nullptr && !bomb->isExploding()) {
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

            if (currentTime >= bomb->getExplodeTime()) {
                bomb->setExploding(true);
                bomb->setVisible(false);
                createExplosionEffect(bomb->pos());

                if (explodePlayer->playbackState() == QMediaPlayer::PlayingState) {
                    explodePlayer->stop();
                }
                explodePlayer->setPosition(0);
                explodePlayer->play();
            }
        }
        ++itB;
    }
}

void FallingController2::createExplosionEffect(const QPointF& position) {
    for (int i = 0; i < 3; ++i) {
        QGraphicsEllipseItem* ring = new QGraphicsEllipseItem(-20 - i*10, -20 - i*10,
                                                              40 + i*20, 40 + i*20);
        int alpha = 200 - i*50;
        ring->setBrush(QBrush(QColor(255, 100 + i*30, 0, alpha)));
        ring->setPen(QPen(QColor(255, 200, 0), 2));
        ring->setPos(position);
        ring->setZValue(1000 - i);
        parentScene->addItem(ring);

        QTimer::singleShot(i * 100 + 400, [this, ring]() {
            if (parentScene && ring->scene() == parentScene) {
                parentScene->removeItem(ring);
                delete ring;
            }
        });
    }

    QGraphicsTextItem* explosion = new QGraphicsTextItem("💥");
    QFont font("Arial", 48);
    explosion->setFont(font);
    explosion->setPos(position.x() - 24, position.y() - 24);
    explosion->setZValue(1001);
    parentScene->addItem(explosion);

    QPropertyAnimation* fadeAnim = new QPropertyAnimation(explosion, "opacity");
    fadeAnim->setDuration(500);
    fadeAnim->setStartValue(1.0);
    fadeAnim->setEndValue(0.0);

    connect(fadeAnim, &QPropertyAnimation::finished, [this, explosion]() {
        parentScene->removeItem(explosion);
        delete explosion;
    });

    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void FallingController2::setFallingRange(QRectF range){
    FallingController2::fallingRange = range;
}

void FallingController2::vanish(){
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

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
            } else {
                ++itD;
            }
        } else {
            itD = createdDrug.erase(itD);
        }
    }

    auto itB = createdBombs.begin();
    while(itB != createdBombs.end()) {
        Bomb* bomb = *itB;
        if(bomb != nullptr) {
            bool shouldRemove = bomb->isExploding() &&
                                (currentTime - bomb->getExplodeTime()) > 600;

            if(shouldRemove) {
                parentScene->removeItem(bomb);
                delete bomb;
                itB = createdBombs.erase(itB);
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
