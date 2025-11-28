#ifndef MYFISH_H
#define MYFISH_H

#include "Character.h"
#include <QPixmap>
#include <QTimer>
class MyFish : public Character {
public:
    explicit MyFish(QGraphicsItem *parent = nullptr);
    virtual void processDirection() override;
    virtual void pickUpWeapon(Weapon* weaponToPick) override;
    virtual void pickUpArmor(Armor* newArmor) override;
    // 张嘴动画控制
    void startEatingAnimation(bool facingRight);
    void stopEatingAnimation();
    bool isEating() const { return eating; }
private:
    void loadSprites();
    QPixmap extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight);
    void updateEatingSprite();
    QPixmap leftSprite;   // 朝左的图片
    QPixmap rightSprite;  // 朝右的图片
    QPixmap leftEatSprite;     // 朝左张嘴的图片
    QPixmap rightEatSprite;    // 朝右张嘴的图片

    bool eating;               // 是否正在吃鱼
    QTimer* eatAnimationTimer; // 张嘴动画定时器
};

#endif // MYFISH_H
