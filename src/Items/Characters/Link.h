//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_LINK_H
#define QT_PROGRAMMING_2024_LINK_H


#include "Character.h"

class Link : public Character {
public:
    explicit Link(QGraphicsItem *parent = nullptr);
    virtual void processDirection() ;
    virtual void pickUpWeapon(Weapon* weaponToPick);
    virtual void pickUpArmor(Armor* newArmor) override;
    //int id{0};
};


#endif //QT_PROGRAMMING_2024_LINK_H
