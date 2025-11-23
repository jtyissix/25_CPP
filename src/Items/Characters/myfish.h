#ifndef MYFISH_H
#define MYFISH_H

#include "Character.h"
#include <QPixmap>
class MyFish : public Character {
public:
    explicit MyFish(QGraphicsItem *parent = nullptr);
    virtual void processDirection() override;
    virtual void pickUpWeapon(Weapon* weaponToPick) override;
    virtual void pickUpArmor(Armor* newArmor) override;

private:
    void loadSprites();
    QPixmap extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight);

    QPixmap leftSprite;   // 朝左的图片
    QPixmap rightSprite;  // 朝右的图片
};

#endif // MYFISH_H
