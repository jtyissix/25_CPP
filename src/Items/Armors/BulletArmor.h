//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_OLDSHIRT_H
#define QT_PROGRAMMING_2024_OLDSHIRT_H


#include "Armor.h"

class BulletArmor: public Armor {
public:
    explicit BulletArmor(QGraphicsItem *parent=nullptr);
    static constexpr int MAX_USAGE=5;
    static constexpr double COEFFICIENT=0.5;
    int usage{0};
    bool validate();
};


#endif //QT_PROGRAMMING_2024_OLDSHIRT_H
