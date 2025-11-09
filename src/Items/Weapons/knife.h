#ifndef KNIFE_H
#define KNIFE_H

#include "Weapon.h"

#include "bullet.h"
#include<vector>
#include "../Characters/Character.h"
class Knife: public Weapon {
public:
    explicit Knife(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void attack() override;
    virtual void setBullet() override;
    friend bool Character::getDirection() const;
    virtual void setBulletCharacter() override;
    virtual void reset() override;
    //virtual void setBulletCharacter(Character* character) override;
    int sourceID;
    static constexpr int HURT=5;
private:
    static constexpr int NUMBER_OF_BULLET=10;
    //static constexpr int HURT=5;
    static constexpr int BULLET_SPEED = 1;
    static constexpr qint64 SHOOT_COOLDOWN = 200; // 射击间隔200毫秒
    //std::vector<Bullet*> bulletSet;
    //int bulletUsage{0};
    qint64 lastShotTime{0}; // 上次射击时间
    QPointF resetPos;
};


#endif // KNIFE_H
