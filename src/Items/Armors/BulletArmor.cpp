//
// Created by gerw on 8/20/24.
//

#include "BulletArmor.h"

BulletArmor::BulletArmor(QGraphicsItem *parent) : Armor(parent, ":/Items/Armors/OldShirt/BotW_Old_Shirt_Icon.png") {

}

bool BulletArmor::validate(){
    usage++;
    if(usage<MAX_USAGE){
        return true;
    }
    else{
        return false;
    }
}
