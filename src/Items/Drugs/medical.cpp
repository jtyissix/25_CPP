#include "medical.h"
#include <QDateTime>
Medical::Medical(QGraphicsItem *parent) :Drug(parent,
           ":/Items/Drugs/pill_32x32.png") {

}

void Medical::mountToParent(double x,double y){
    Mountable::mountToParent(x,y);
    setScale(1.3);
    setPos(x,y);
    if (pixmapItem != nullptr) {
        pixmapItem->setPos(x, y);
    }
}

void Medical::effect(){
    auto character = dynamic_cast<Character*>(parentItem());
    character->setPill(true);
    character->lastPillTime=QDateTime::currentMSecsSinceEpoch();
}
