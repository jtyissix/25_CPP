#include "bandage.h"
#include<QGraphicsScene>
#include<QDateTime>
#include"../Characters/Character.h"
Bandage::Bandage(QGraphicsItem *parent) :Drug(parent,
             ":/Items/Drugs/bandage_32x32.png") {

}

void Bandage::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(1.3);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Bandage::effect(){
    auto character = dynamic_cast<Character*>(parentItem());
    character->takeDamage(-CURE);
}

