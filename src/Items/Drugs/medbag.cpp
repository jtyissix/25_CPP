#include "medbag.h"

MedBag::MedBag(QGraphicsItem *parent) :Drug(parent,
           ":/Items/Drugs/first_aid_kit_32x32.png") {

}

void MedBag::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(1.3);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void MedBag::effect(){
    auto character = dynamic_cast<Character*>(parentItem());
    character->takeDamage(-CURE);
}
