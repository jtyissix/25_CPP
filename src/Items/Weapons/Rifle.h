//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_CAPOFTHEHERO_H
#define QT_PROGRAMMING_2024_CAPOFTHEHERO_H


#include "Weapon.h"
#include "bullet.h"
#include<vector>
#include "../Characters/Character.h"
class Rifle: public Weapon {
public:
    explicit Rifle(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void attack() override;
    virtual void setBullet() override;
    friend bool Character::getDirection() const;
    virtual void setBulletCharacter() override;
    //virtual void setBulletCharacter(Character* character) override;
private:
    static constexpr int NUMBER_OF_BULLET=10;
    static constexpr int HURT=15;
    static constexpr int BULLET_SPEED = 1;
    static constexpr qint64 SHOOT_COOLDOWN = 200; // 射击间隔200毫秒
    std::vector<Bullet*> bulletSet;
    int bulletUsage{0};
    qint64 lastShotTime{0}; // 上次射击时间
};


#endif //QT_PROGRAMMING_2024_CAPOFTHEHERO_H
