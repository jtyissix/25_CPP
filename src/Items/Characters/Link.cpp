//
// Created by gerw on 8/20/24.
//

#include <memory>
#include "Link.h"
#include "../Weapons/Weapon.h"
#include "../Figure/FigureA.h"
#include "../Figure/FigureA_KneeDown.h"
#include "../Figure/figurea_hand.h"
#include<QGraphicsScene>

Link::Link(QGraphicsItem *parent) : Character(parent) {
    figure = new FigureA(this);
    figure->mountToParent(0,-115);
    figureKneeDown = new FigureA_KneeDown(this);
    figureKneeDown->mountToParent(0,-115);
    figureKneeDown->setVisible(false);
    figureAttack = new Figurea_hand(this);
    figureAttack->mountToParent(0,-115);
    figureAttack->setVisible(false);
    weaponMountX=50;
    weaponMountY=-50;
    weaponAttackX=100;
    weaponAttackY=-80;
}

void Link::processDirection()  {

    if(getDirection()){
        figure->setTransform(QTransform::fromScale(1, 1));
        figureKneeDown->setTransform(QTransform::fromScale(1, 1));
        figureAttack->setTransform(QTransform::fromScale(1, 1));
        if(weapon!=nullptr){
            weapon->setTransform(QTransform::fromScale(1, 1));
            weapon->mountToParent(50,-50);
        }
        if(armor!=nullptr){
            armor->setTransform(QTransform::fromScale(-1, 1));
            armor->mountToParent(230,-50);
        }
    }
    else{
        figure->setTransform(QTransform::fromScale(-1, 1));
        figureKneeDown->setTransform(QTransform::fromScale(-1, 1));
        figureAttack->setTransform(QTransform::fromScale(-1, 1));
        if(weapon!=nullptr){
            weapon->setTransform(QTransform::fromScale(-1, 1));
            weapon->mountToParent(-50,-50);
        }
        if(armor!=nullptr){
            armor->setTransform(QTransform::fromScale(1, 1));
            armor->mountToParent(-80,-50);
        }
    }

}

void Link::pickUpWeapon(Weapon* weaponToPick){
    if(weapon!=nullptr){
        weapon->setParentItem(nullptr);
        if(weapon->scene()!=nullptr)
        {
            weapon->scene()->removeItem(weapon);
        }
        //weapon->deleteLater();
        weapon=nullptr;
    }
    weaponToPick->setParentItem(this);
    weaponToPick->setBulletCharacter();
    if(facingRight){
        weaponToPick->setTransform(QTransform::fromScale(1, 1));
        weaponToPick->mountToParent(50,-50);
    }
    else{
        weaponToPick->setTransform(QTransform::fromScale(-1, 1));
        weaponToPick->mountToParent(-50,-50);
    }
    weaponToPick->setIsPicked(true);
    weapon=weaponToPick;
}
void Link::pickUpArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->setVisible(false);
        oldArmor->setIsPicked(false);
    }
    newArmor->setParentItem(this);
    newArmor->setIsPicked(true);
    if(facingRight){
        newArmor->setTransform(QTransform::fromScale(-1, 1));
        newArmor->mountToParent(230,-50);
    }
    else{
        newArmor->setTransform(QTransform::fromScale(1, 1));
        newArmor->mountToParent(-80,-50);
    }
    //newArmor->mountToParent(25,-50);
    armor = newArmor;
    //return oldArmor;
}
