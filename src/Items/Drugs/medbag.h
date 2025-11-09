#ifndef MEDBAG_H
#define MEDBAG_H

#include "drug.h"
#include<vector>
#include "../Characters/Character.h"
class MedBag: public Drug {
public:
    explicit MedBag(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void effect() override;
private:
    static constexpr int CURE = 100;
    //static constexpr int NUMBER_OF_BULLET=5;
    //static constexpr int HURT=30;
    //static constexpr int BULLET_SPEED = 5;
    //static constexpr qint64 SHOOT_COOLDOWN = 1000; // 射击间隔200毫秒
    //std::vector<Bullet*> bulletSet;
    //int bulletUsage{0};
    //qint64 lastShotTime{0}; // 上次射击时间
};

#endif // MEDBAG_H
