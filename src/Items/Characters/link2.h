#ifndef LINK2_H
#define LINK2_H

#include "Character.h"

class Link2 : public Character {
public:
    explicit Link2(QGraphicsItem *parent = nullptr);
    virtual void processDirection();
    virtual void pickUpWeapon(Weapon* weaponToPick);
    virtual void pickUpArmor(Armor* newArmor) override;
    //int id{1};
};

#endif // LINK2_H
