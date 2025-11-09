#include "knife.h"

#include "../Characters/Character.h"
#include<QGraphicsScene>
#include<QDateTime>
Knife::Knife(QGraphicsItem *parent) :Weapon(parent,
             ":/Items/Weapons/messer.png") {

}

void Knife::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(0.1);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Knife::attack(){
    auto character = dynamic_cast<Character*>(parentItem());
    resetPos = character->weapon->pos();
    character->figure->setVisible(false);
    character->figureKneeDown->setVisible(false);
    character->figureAttack->setVisible(true);
    if(character->getDirection())
    {
    setPos(character->weaponAttackX,character->weaponAttackY);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(character->weaponAttackX,character->weaponAttackY);
    }
    }
    else{
        setPos(-character->weaponAttackX,character->weaponAttackY);
        if (pixmapItem != nullptr) {
            pixmapItem->setPos(-character->weaponAttackX,character->weaponAttackY);
        }
    }
}

void Knife::setBullet(){
//no use
}

void Knife::setBulletCharacter(){
sourceID=dynamic_cast<Character*>(parentItem())->id;
}

void Knife::reset(){
    auto character = dynamic_cast<Character*>(parentItem());
    if(character->getDirection()){
    setPos(character->weaponMountX,character->weaponMountY);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(character->weaponMountX,character->weaponMountY);
    }
    }
    else{
        setPos(-character->weaponMountX,character->weaponMountY);
        if (pixmapItem != nullptr) {
            pixmapItem->setPos(-character->weaponMountX,character->weaponMountY);
        }
    }
}

