//
// Created by gerw on 8/20/24.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsEllipseItem>
#include "../Weapons/Weapon.h"
#include "../Drugs/drug.h"
#include "../Armors/Armor.h"
#include "../Figure/Figure.h"
#include <QRectF>
class Character : public Item {
public:
    explicit Character(QGraphicsItem *parent);

    [[nodiscard]] bool isJumpDown() const;

    void setJumpDown(bool jumpDown);

    [[nodiscard]] bool getIsJumping() const;

    [[nodiscard]] bool getIsOnGround() const;

    void setIsOnGround(bool onGround);

    [[nodiscard]] bool getDirection() const;

    void setDirection(bool direction);

    // 物理处理方法
    void applyGravity();
    void processJump();

    [[nodiscard]] bool isKneeDown() const;

    void setKneeDown(bool kneeDown);

    [[nodiscard]] bool isLeftDown() const;

    void setLeftDown(bool leftDown);

    [[nodiscard]] bool isRightDown() const;

    void setUp(bool setUp){Character::up=setUp;}

    [[nodiscard]] bool isUp() {return Character::up;}

    void setDown(bool setDown){Character::down=setDown;}

    [[nodiscard]] bool isDown() {return Character::down;}

    void setRightDown(bool rightDown);

    [[nodiscard]] bool isPickDown() const;

    void setPickDown(bool pickDown);

    [[nodiscard]] const QPointF &getVelocity() const;

    [[nodiscard]] bool isPicking() const;

    void setVelocity(const QPointF &velocity);

    void processInput();

    virtual void pickUpArmor(Armor* newArmor){};

    virtual void processDirection() = 0 ;

    void setAttack(bool isattack);

    [[nodiscard]] bool getAttack() const;

    void processAttack();

    // 生命值系统
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    [[nodiscard]] qreal getHealthPercentage() const;
    void setHealth(int health);
    void setMaxHealth(int maxHealth);
    void takeDamage(int damage);
    void heal(int amount);
    [[nodiscard]] bool isAlive() const;

    // 攻击系统
    [[nodiscard]] QRectF getAttackBounds() const;
    [[nodiscard]] QRectF getCollisionBounds() const;
    [[nodiscard]] bool isAttacking() const;
    [[nodiscard]] bool canDealDamage() const;
    void setLastAttackTime(qint64 time);
    [[nodiscard]] qint64 getLastAttackTime() const;

    //拾取武器
    virtual void pickUpWeapon(Weapon* weaponToPick);

    void pickUpDrug(Drug* drugToPick);

    void setWeapon(Weapon* weapon){Character:: weapon = weapon;}

    Weapon* getWeapon(){return weapon;}

    void setPill(bool isPill){Character:: isPill = isPill;}

    [[nodiscard]] bool getIsPill(){return isPill;}

    Armor* getArmor(){return armor;}

    void setArmor(Armor* armor){Character::armor=armor;}
    int id;
    bool isInStealth() const { return inStealth; }
    void setInStealth(bool stealth) { inStealth = stealth; }
    bool isInIce() const { return inIce; }
    void setInIce(bool stealth) { inIce = stealth; }
    // 修改可见性处理
    void updateVisibility();
protected:
    Figure* figure;
    Figure* figureKneeDown;
    Figure* figureAttack;
    Weapon* weapon{nullptr};
    Armor* armor{nullptr};
    QPointF velocity{QPointF(0, 0)};
    // 生命值
    int health;
    int maxHealth;
    bool facingRight{true};
    bool inStealth{false};
    bool inIce{false};
    // 攻击相关
    qint64 lastAttackTime;
    qint64 lastPillTime{0};
    qint64 lastPillCureTime{0};
    int weaponMountX;
    int weaponMountY;
    int weaponAttackX;
    int weaponAttackY;
//    QGraphicsEllipseItem *ellipseItem; // for debugging
private:
    bool leftDown{}, rightDown{}, pickDown{};
    bool up{},down{};
    bool lastPickDown{};
    bool picking{};
    bool kneeDown{};
    // jumping property
    bool jumpDown{};
    bool lastJumpDown{};
    bool isJumping{};
    bool isOnGround{true};
    bool isPill{false};
    bool lastPill{false};
    //no use
    bool isAttack{false};
    bool lastDirection;
    // 物理常量
    static constexpr qreal GRAVITY = 0.01;        // 重力加速度
    static constexpr qreal JUMP_SPEED = -1;      // 初始跳跃速度(负值表示向上)
    static constexpr qreal TERMINAL_VELOCITY = 0.8; // 最大下落速度
    static constexpr qreal TERMINAL_HORIZONTAL_VELOCITY = 0.3;
    static constexpr int ATTACK_DAMAGE = 10;       // 攻击伤害
    static constexpr qint64 ATTACK_COOLDOWN = 500; // 攻击冷却时间(毫秒)
    static constexpr qreal ATTACK_RANGE = 120;      // 攻击范围
    static constexpr int PILL_TIME=5000;
    static constexpr int PILL_CURE=5;
    static constexpr int PILL_SPEED=1;
    static constexpr qreal ICE_SPEEDUP=0.4;
    friend class Knife;
    friend class Medical;
};


#endif //QT_PROGRAMMING_2024_CHARACTER_H
