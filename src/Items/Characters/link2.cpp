#include <memory>
#include "Link2.h"
#include "../Weapons/Weapon.h"

#include "../Figure/FigureB.h"
#include "../Figure/FigureB_KneeDown.h"
#include "../Figure/figureb_hand.h"
#include<QGraphicsScene>

Link2::Link2(QGraphicsItem *parent) : Character(parent) {
    figure = new FigureB(this);
    figure->mountToParent(0,-68);
    figureKneeDown = new FigureB_KneeDown(this);
    figureKneeDown->mountToParent(0,-68);
    figureKneeDown->setVisible(false);
    figureAttack = new Figureb_hand(this);
    figureAttack->mountToParent(0,-68);
    figureAttack->setVisible(false);
    weaponMountX=-80;
    weaponMountY=-50;
    weaponAttackX=-30;
    weaponAttackY=-90;
}

void Link2::processDirection()  {
    //QPointF originalPos1 = figure->pos();
    //QPointF originalPos2 = figureKneeDown->pos();
    if(getDirection()){
        figure->setTransform(QTransform::fromScale(-1, 1));
        figureKneeDown->setTransform(QTransform::fromScale(-1, 1));
        figureAttack->setTransform(QTransform::fromScale(-1, 1));
        if(weapon!=nullptr){
            weapon->setTransform(QTransform::fromScale(1, 1));
            weapon->mountToParent(-80,-50);
        }
        if(armor!=nullptr){
            armor->setTransform(QTransform::fromScale(-1, 1));
            armor->mountToParent(-30,-50);
        }
    }
    else{
        figure->setTransform(QTransform::fromScale(1, 1));
        figureKneeDown->setTransform(QTransform::fromScale(1, 1));
        figureAttack->setTransform(QTransform::fromScale(1, 1));
        if(weapon!=nullptr){
            weapon->setTransform(QTransform::fromScale(-1, 1));
            weapon->mountToParent(80,-50);
        }
        if(armor!=nullptr){
            armor->setTransform(QTransform::fromScale(1, 1));
            armor->mountToParent(20,-50);
        }
    }
    //figure->setPos(originalPos1);
    //figureKneeDown->setPos(originalPos2);
}

void Link2::pickUpWeapon(Weapon* weaponToPick){
    if(weapon!=nullptr){
        weapon->setParentItem(nullptr);
        if(weapon->scene()!=nullptr)
        {
            weapon->scene()->removeItem(weapon);
        }
        weapon=nullptr;
    }
    weaponToPick->setParentItem(this);
    weaponToPick->setBulletCharacter();
    if(facingRight){
        weaponToPick->setTransform(QTransform::fromScale(1, 1));
        weaponToPick->mountToParent(-80,-50);
    }
    else{
        weaponToPick->setTransform(QTransform::fromScale(-1, 1));
        weaponToPick->mountToParent(80,-50);
    }
    weaponToPick->setIsPicked(true);
    weapon=weaponToPick;
}

void Link2::pickUpArmor(Armor *newArmor) {
    auto oldArmor = armor;
    if (oldArmor != nullptr) {
        oldArmor->setVisible(false);
        oldArmor->setIsPicked(false);
    }
    newArmor->setParentItem(this);
    newArmor->setIsPicked(true);
    if(facingRight){
        newArmor->setTransform(QTransform::fromScale(-1, 1));
        newArmor->mountToParent(-30,-50);
    }
    else{
        newArmor->setTransform(QTransform::fromScale(1, 1));
        newArmor->mountToParent(20,-50);
    }
    //newArmor->mountToParent(25,-50);
    armor = newArmor;
    //return oldArmor;
}
