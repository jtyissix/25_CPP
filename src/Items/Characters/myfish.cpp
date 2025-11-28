#include "myfish.h"
#include "MyFish.h"
#include "../Weapons/Weapon.h"
#include "../Figure/Figure.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QBitmap>
MyFish::MyFish(QGraphicsItem *parent) : Character(parent), eating(false)
    , eatAnimationTimer(nullptr)
{
    // 加载精灵图
    qDebug() << "=== MyFish Constructor ===";

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

    // ⭐⭐⭐ 关键修复：给MyFish自己的pixmapItem赋值
    // 这样boundingRect()才不会返回空矩形
    this->pixmapItem = new QGraphicsPixmapItem(rightSprite, this);
    this->pixmapItem->setVisible(false);  // 隐藏，我们用figure显示

    qDebug() << "MyFish boundingRect:" << boundingRect();
    qDebug() << "Right sprite size:" << rightSprite.size();

    weaponMountX = 50;
    weaponMountY = -50;
    weaponAttackX = 100;
    weaponAttackY = -80;
    // 创建吃鱼动画定时器
    eatAnimationTimer = new QTimer(this);
    connect(eatAnimationTimer, &QTimer::timeout, this, &MyFish::stopEatingAnimation);
}


void MyFish::loadSprites() {
    // 加载完整的精灵图
    QPixmap spriteSheet(":/Items/Characters/Fish2.bmp");

    if (spriteSheet.isNull()) {
        qDebug() << "Failed to load Fish2.bmp";
        return;
    }

    // 精灵图参数：12列5行，每帧64x64
    int frameWidth = 64;
    int frameHeight = 64;

    // 提取朝右的图片（比如第0行第0列）
    rightSprite = extractFrame(spriteSheet, 0, 0, frameWidth, frameHeight);

    // 提取朝左的图片（比如第0行第1列）
    leftSprite = extractFrame(spriteSheet, 0, 11, frameWidth, frameHeight);
    // ⭐ 提取朝右张嘴的图片（TODO: 根据你的精灵图实际布局修改行列参数）
    rightEatSprite = extractFrame(spriteSheet, 0, 4, frameWidth, frameHeight);

    // ⭐ 提取朝左张嘴的图片（TODO: 根据你的精灵图实际布局修改行列参数）
    leftEatSprite = extractFrame(spriteSheet, 0, 7, frameWidth, frameHeight);

    qDebug() << "Loaded right eat sprite:" << !rightEatSprite.isNull();
    qDebug() << "Loaded left eat sprite:" << !leftEatSprite.isNull();
    // 如果你想用其他帧，修改行列参数
    // 例如：rightSprite = extractFrame(spriteSheet, 2, 5, frameWidth, frameHeight);

    qDebug() << "Loaded right sprite:" << !rightSprite.isNull();
    qDebug() << "Loaded left sprite:" << !leftSprite.isNull();
}

QPixmap MyFish::extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight) {
    int x = col * frameWidth;
    int y = row * frameHeight;

    QPixmap frame = spriteSheet.copy(x, y, frameWidth, frameHeight);
    QBitmap mask = frame.createMaskFromColor(Qt::white, Qt::MaskInColor);
    frame.setMask(mask);
    return frame;
}

void MyFish::processDirection() {
    // 根据方向切换图片
    //QPixmap currentSprite = getDirection() ? rightSprite : leftSprite;
    QPixmap currentSprite;

    if (eating) {
        // 正在吃鱼，显示张嘴的图片
        currentSprite = getDirection() ? rightEatSprite : leftEatSprite;
    } else {
        // 正常状态
        currentSprite = getDirection() ? rightSprite : leftSprite;
    }
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

void MyFish::pickUpWeapon(Weapon* weaponToPick) {
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

void MyFish::pickUpArmor(Armor *newArmor) {
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
void MyFish::startEatingAnimation(bool facingRight) {
    eating = true;

    // 更新方向
    setDirection(facingRight);

    // 更新图片为张嘴状态
    updateEatingSprite();

    // 设置200ms后恢复正常状态
    eatAnimationTimer->start(200);

    qDebug() << "Started eating animation, facing" << (facingRight ? "right" : "left");
}

void MyFish::stopEatingAnimation() {
    eating = false;
    eatAnimationTimer->stop();

    // 恢复正常图片
    processDirection();

    qDebug() << "Stopped eating animation";
}

void MyFish::updateEatingSprite() {
    QPixmap eatSprite = getDirection() ? rightEatSprite : leftEatSprite;

    if (figure && figure->pixmapItem) {
        figure->pixmapItem->setPixmap(eatSprite);
    }

    if (figureKneeDown && figureKneeDown->pixmapItem) {
        figureKneeDown->pixmapItem->setPixmap(eatSprite);
    }

    if (figureAttack && figureAttack->pixmapItem) {
        figureAttack->pixmapItem->setPixmap(eatSprite);
    }
}
