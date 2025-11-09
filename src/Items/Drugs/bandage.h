#ifndef BANDAGE_H
#define BANDAGE_H

#include "drug.h"
#include<vector>
#include "../Characters/Character.h"
class Bandage: public Drug {
public:
    explicit Bandage(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void effect() override;
private:
    static constexpr int CURE = 20;
    //static constexpr int NUMBER_OF_BULLET=5;
    //static constexpr int HURT=30;
    //static constexpr int BULLET_SPEED = 5;
    //static constexpr qint64 SHOOT_COOLDOWN = 1000; // 射击间隔200毫秒
    //std::vector<Bullet*> bulletSet;
    //int bulletUsage{0};
    //qint64 lastShotTime{0}; // 上次射击时间
};

#endif // BANDAGE_H
