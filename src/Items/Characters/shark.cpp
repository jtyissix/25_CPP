#include "myfish.h"
#include "shark.h"
#include "../Weapons/Weapon.h"
#include "../Figure/Figure.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QBitmap>
#include <QRandomGenerator>
shark::shark(QGraphicsItem *parent) : Character(parent) {
    // 加载精灵图
    qDebug() << "=== shark Constructor ===";

    loadSprites();

    // 创建figure
    figure = new Figure(this, "");
    figure->pixmapItem = new QGraphicsPixmapItem(rightSprite, figure);
    figure->setPos(0, -0);  // ✅ 使用setPos
    figure->setVisible(true);

    figureKneeDown = new Figure(this, "");
    figureKneeDown->pixmapItem = new QGraphicsPixmapItem(rightSprite, figureKneeDown);
    figureKneeDown->setPos(0, -115);
    figureKneeDown->setVisible(false);

    figureAttack = new Figure(this, "");
    figureAttack->pixmapItem = new QGraphicsPixmapItem(rightSprite, figureAttack);
    figureAttack->setPos(0, -115);
    figureAttack->setVisible(false);

    // ⭐⭐⭐ 关键修复：给shark自己的pixmapItem赋值
    // 这样boundingRect()才不会返回空矩形
    this->pixmapItem = new QGraphicsPixmapItem(rightSprite, this);
    this->pixmapItem->setVisible(false);  // 隐藏，我们用figure显示

    qDebug() << "shark boundingRect:" << boundingRect();
    qDebug() << "Right sprite size:" << rightSprite.size();

    weaponMountX = 50;
    weaponMountY = -50;
    weaponAttackX = 100;
    weaponAttackY = -80;
    // Shark 特有属性初始化
    moveSpeed = 0.5;  // 默认速度（快速）
    isEnemy = false;  // 默认不是敌人
}


void shark::loadSprites() {
    // 加载完整的精灵图
    QPixmap spriteSheet(":/Items/Characters/shop_ths.png");

    if (spriteSheet.isNull()) {
        qDebug() << "Failed to load Fish2.bmp";
        return;
    }

    // 精灵图参数：12列5行，每帧64x64
    int frameWidth = 202;
    int frameHeight = 115;

    // 提取朝右的图片（比如第0行第0列）
    leftSprite = extractFrame(spriteSheet, 0, 0, frameWidth, frameHeight);

    // 简单镜像
    rightSprite = leftSprite.transformed(QTransform().scale(-1, 1));
    // 如果你想用其他帧，修改行列参数
    // 例如：rightSprite = extractFrame(spriteSheet, 2, 5, frameWidth, frameHeight);

    qDebug() << "Loaded right sprite:" << !rightSprite.isNull();
    qDebug() << "Loaded left sprite:" << !leftSprite.isNull();
}

QPixmap shark::extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight) {
    int x = col * frameWidth;
    int y = row * frameHeight;

    QPixmap frame = spriteSheet.copy(x, y, frameWidth, frameHeight);
    QBitmap mask = frame.createMaskFromColor(Qt::white, Qt::MaskInColor);
    frame.setMask(mask);
    return frame;
}

void shark::processDirection() {
    // 根据方向切换图片
    QPixmap currentSprite = getDirection() ? leftSprite : rightSprite;

    // 更新所有 figure 的图片
    if (figure && figure->pixmapItem) {
        figure->pixmapItem->setPixmap(currentSprite);
    }

    if (figureKneeDown && figureKneeDown->pixmapItem) {
        figureKneeDown->pixmapItem->setPixmap(currentSprite);
    }

    if (figureAttack && figureAttack->pixmapItem) {
        figureAttack->pixmapItem->setPixmap(currentSprite);
    }

    // 处理武器和护甲的翻转
    if (getDirection()) {
        // 朝右
        if (weapon != nullptr) {
            weapon->setTransform(QTransform::fromScale(1, 1));
            weapon->mountToParent(50, -50);
        }
        if (armor != nullptr) {
            armor->setTransform(QTransform::fromScale(-1, 1));
            armor->mountToParent(230, -50);
        }
    } else {
        // 朝左
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

void shark::pickUpWeapon(Weapon* weaponToPick) {
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

void shark::pickUpArmor(Armor *newArmor) {
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

// 初始化为敌人（从左或右随机出现）
void shark::initializeAsEnemy(const QRectF& sceneRect) {
    isEnemy = true;

    // 随机选择从左边还是右边出现
    bool spawnFromLeft = (QRandomGenerator::global()->bounded(2) == 0);

    // 随机Y坐标（在场景范围内，留出上下边距）
    qreal margin = 100;  // 上下边距
    qreal minY = sceneRect.top() + margin;
    qreal maxY = sceneRect.bottom() - margin - 115; // 115是shark高度
    qreal randomY = minY + (maxY - minY) * (QRandomGenerator::global()->bounded(1000) / 1000.0);

    // 设置位置和方向
    if (spawnFromLeft) {
        // 从左边出现，向右移动
        setPos(sceneRect.left() - 202, randomY);  // 202是shark宽度
        setDirection(false);   // 朝右
        setVelocity(QPointF(moveSpeed, 0));  // 向右移动
    } else {
        // 从右边出现，向左移动
        setPos(sceneRect.right() + 202, randomY);
        setDirection(true);  // 朝左
        setVelocity(QPointF(-moveSpeed, 0));  // 向左移动
    }

    processDirection();  // 更新图片方向

    qDebug() << "Shark spawned at" << pos() << "moving" << (spawnFromLeft ? "right" : "left");
}

// 更新移动（每帧调用）
void shark::updateMovement(qint64 deltaTime) {
    if (!isEnemy) return;  // 如果不是敌人模式，不自动移动

    // 获取当前速度
    QPointF vel = getVelocity();

    // 只保留水平移动，清除任何竖直速度
    vel.setY(0);
    setVelocity(vel);

    // 更新位置
    QPointF currentPos = pos();
    QPointF newPos = currentPos + vel * deltaTime;
    setPos(newPos);
}

// 检查是否越界（用于删除）
bool shark::isOutOfBounds(const QRectF& sceneRect) const {
    QPointF currentPos = pos();

    // Shark宽度约202
    qreal sharkWidth = 202;

    // 完全移出左边界
    if (currentPos.x() + sharkWidth < sceneRect.left()) {
        return true;
    }

    // 完全移出右边界
    if (currentPos.x() > sceneRect.right()) {
        return true;
    }

    return false;
}
