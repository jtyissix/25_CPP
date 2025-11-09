#ifndef BALLHAND_H
#define BALLHAND_H

#include "Weapon.h"

#include "ball.h"
#include<vector>
#include "../Characters/Character.h"
class BallHand: public Weapon {
public:
    explicit BallHand(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void attack() override;
    virtual void setBullet() override;
    friend bool Character::getDirection() const;
    virtual void setBulletCharacter() override;
    //virtual void setBulletCharacter(Character* character) override;
private:
    static constexpr int NUMBER_OF_BULLET=5;
    static constexpr int HURT=8;
    static constexpr int BULLET_X_SPEED = 1;
    static constexpr int BULLET_Y_SPEED = -1;
    static constexpr qint64 SHOOT_COOLDOWN = 200; // 射击间隔200毫秒
    std::vector<Ball*> bulletSet;
    int bulletUsage{0};
    qint64 lastShotTime{0}; // 上次射击时间
};

#endif // BALLHAND_H
