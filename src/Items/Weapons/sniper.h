#ifndef SNIPPER_H
#define SNIPPER_H

#include "Weapon.h"
#include "bullet.h"
#include<vector>
#include "../Characters/Character.h"
class Sniper: public Weapon {
public:
    explicit Sniper(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void attack() override;
    virtual void setBullet() override;
    friend bool Character::getDirection() const;
    virtual void setBulletCharacter() override;
private:
    static constexpr int NUMBER_OF_BULLET=5;
    static constexpr int HURT=30;
    static constexpr int BULLET_SPEED = 5;
    static constexpr qint64 SHOOT_COOLDOWN = 1000; // 射击间隔200毫秒
    std::vector<Bullet*> bulletSet;
    int bulletUsage{0};
    qint64 lastShotTime{0}; // 上次射击时间
};


#endif // SNIPPER_H
