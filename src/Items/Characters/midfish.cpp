#include "midfish.h"
#include "../Weapons/Weapon.h"
#include "../Figure/Figure.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QBitmap>
#include <QRandomGenerator>
#include <QtMath>

midfish::midfish(QGraphicsItem *parent) : Character(parent) {
    loadSprites();
    
    figure = new Figure(this, "");
    figure->pixmapItem = new QGraphicsPixmapItem(leftSprite, figure);
    figure->setPos(0, 0);
    figure->setVisible(true);
    
    figureKneeDown = new Figure(this, "");
    figureKneeDown->pixmapItem = new QGraphicsPixmapItem(leftSprite, figureKneeDown);
    figureKneeDown->setPos(0, -115);
    figureKneeDown->setVisible(false);
    
    figureAttack = new Figure(this, "");
    figureAttack->pixmapItem = new QGraphicsPixmapItem(leftSprite, figureAttack);
    figureAttack->setPos(0, -115);
    figureAttack->setVisible(false);
    
    this->pixmapItem = new QGraphicsPixmapItem(leftSprite, this);
    this->pixmapItem->setVisible(false);
    
    weaponMountX = 50;
    weaponMountY = -50;
    weaponAttackX = 100;
    weaponAttackY = -80;
    
    moveSpeed = 0.3;
    isEnemy = false;
}

void midfish::loadSprites() {
    QPixmap spriteSheet(":/Items/Characters/fishIconL15.png");
    
    if (spriteSheet.isNull()) {
        qDebug() << "Failed to load fishIconL15.png";
        return;
    }
    
    int frameWidth = spriteSheet.width();
    int frameHeight = spriteSheet.height();
    
    leftSprite = spriteSheet.copy(0, 0, frameWidth, frameHeight);
    QBitmap mask = leftSprite.createMaskFromColor(Qt::white, Qt::MaskInColor);
    leftSprite.setMask(mask);
    
    rightSprite = leftSprite.transformed(QTransform().scale(-1, 1));
}

QPixmap midfish::extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight) {
    int x = col * frameWidth;
    int y = row * frameHeight;
    
    QPixmap frame = spriteSheet.copy(x, y, frameWidth, frameHeight);
    QBitmap mask = frame.createMaskFromColor(Qt::white, Qt::MaskInColor);
    frame.setMask(mask);
    return frame;
}

void midfish::processDirection() {
    QPixmap currentSprite = getDirection() ? leftSprite : rightSprite;
    
    if (figure && figure->pixmapItem) {
        figure->pixmapItem->setPixmap(currentSprite);
    }
    
    if (figureKneeDown && figureKneeDown->pixmapItem) {
        figureKneeDown->pixmapItem->setPixmap(currentSprite);
    }
    
    if (figureAttack && figureAttack->pixmapItem) {
        figureAttack->pixmapItem->setPixmap(currentSprite);
    }
    
    if (getDirection()) {
        if (weapon != nullptr) {
            weapon->setTransform(QTransform::fromScale(1, 1));
            weapon->mountToParent(50, -50);
        }
        if (armor != nullptr) {
            armor->setTransform(QTransform::fromScale(-1, 1));
            armor->mountToParent(230, -50);
        }
    } else {
        if (weapon != nullptr) {
            weapon->setTransform(QTransform::fromScale(-1, 1));
            weapon->mountToParent(-50, -50);
        }
        if (armor != nullptr) {
            armor->setTransform(QTransform::fromScale(1, 1));
            armor->mountToParent(-80, -50);
        }
    }
}

void midfish::pickUpWeapon(Weapon* weaponToPick) {
    if (weapon != nullptr) {
        weapon->setParentItem(nullptr);
        if (weapon->scene() != nullptr) {
            weapon->scene()->removeItem(weapon);
        }
        weapon = nullptr;
    }
    
    weaponToPick->setParentItem(this);
    weaponToPick->setBulletCharacter();
    
    if (facingRight) {
        weaponToPick->setTransform(QTransform::fromScale(1, 1));
        weaponToPick->mountToParent(50, -50);
    } else {
        weaponToPick->setTransform(QTransform::fromScale(-1, 1));
        weaponToPick->mountToParent(-50, -50);
    }
    
    weaponToPick->setIsPicked(true);
    weapon = weaponToPick;
}

void midfish::pickUpArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->setVisible(false);
        oldArmor->setIsPicked(false);
    }
    
    newArmor->setParentItem(this);
    newArmor->setIsPicked(true);
    
    if (facingRight) {
        newArmor->setTransform(QTransform::fromScale(-1, 1));
        newArmor->mountToParent(230, -50);
    } else {
        newArmor->setTransform(QTransform::fromScale(1, 1));
        newArmor->mountToParent(-80, -50);
    }
    
    armor = newArmor;
}

void midfish::initializeAsEnemy(const QRectF& sceneRect, const QPointF& targetPos) {
    isEnemy = true;
    
    bool spawnFromLeft = (QRandomGenerator::global()->bounded(2) == 0);
    
    qreal margin = 100;
    qreal minY = sceneRect.top() + margin;
    qreal maxY = sceneRect.bottom() - margin - 64;
    qreal randomY = minY + (maxY - minY) * (QRandomGenerator::global()->bounded(1000) / 1000.0);
    
    QPointF spawnPos;
    if (spawnFromLeft) {
        spawnPos = QPointF(sceneRect.left() - 64, randomY);
    } else {
        spawnPos = QPointF(sceneRect.right() + 64, randomY);
    }
    
    setPos(spawnPos);
    
    QPointF direction = targetPos - spawnPos;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0) {
        direction /= length;
    }
    
    QPointF velocity = direction * moveSpeed;
    setVelocity(velocity);
    
    if (velocity.x() > 0) {
        setDirection(false);
    } else {
        setDirection(true);
    }
    
    processDirection();
}

void midfish::updateMovement(qint64 deltaTime) {
    if (!isEnemy) return;
    
    QPointF vel = getVelocity();
    
    QPointF currentPos = pos();
    QPointF newPos = currentPos + vel * deltaTime;
    setPos(newPos);
}

bool midfish::isOutOfBounds(const QRectF& sceneRect) const {
    QPointF currentPos = pos();
    
    qreal midfishWidth = 64;
    
    if (currentPos.x() + midfishWidth < sceneRect.left()) {
        return true;
    }
    
    if (currentPos.x() > sceneRect.right()) {
        return true;
    }
    
    if (currentPos.y() + 64 < sceneRect.top()) {
        return true;
    }
    
    if (currentPos.y() > sceneRect.bottom()) {
        return true;
    }
    
    return false;
}
