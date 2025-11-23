#ifndef SMALLFISH_H
#define SMALLFISH_H

#include "Character.h"
#include <QPixmap>
class smallfish : public Character
{
public:
    explicit smallfish(QGraphicsItem *parent = nullptr);
    virtual void processDirection() override;
    virtual void pickUpWeapon(Weapon* weaponToPick) override;
    virtual void pickUpArmor(Armor* newArmor) override;

private:
    void loadSprites();
    QPixmap extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight);

    QPixmap leftSprite;   // 朝左的图片
    QPixmap rightSprite;  // 朝右的图片
};


#endif // SMALLFISH_H
