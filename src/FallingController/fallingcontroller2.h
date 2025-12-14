#ifndef FALLINGCONTROLLER2_H
#define FALLINGCONTROLLER2_H

#include "../Items/Weapons/Rifle.h"
#include "../Items/Drugs/drug.h"
#include <QObject>
#include <QTimer>
#include <vector>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class Bomb : public QGraphicsPixmapItem {
public:
    explicit Bomb(QGraphicsItem *parent = nullptr);

    void applyGravity();
    bool getIsFalling() const { return isFalling; }
    void setIsFalling(bool falling) { isFalling = falling; }
    bool getIsOnGround() const { return isOnGround; }
    void setIsOnGround(bool onGround) {
        isOnGround = onGround;
        if (onGround) {
            setIsFalling(false);
        }
    }

    QPointF getFallingVelocity() const { return fallingVelocity; }
    void setFallingVelocity(const QPointF& velocity) { fallingVelocity = velocity; }
    qint64 getCreateTime() const { return createTime; }
    void setCreateTime(qint64 time) { createTime = time; }

    bool isExploding() const { return exploding; }
    void setExploding(bool exp) { exploding = exp; }

    qint64 getExplodeTime() const { return explodeTime; }
    void setExplodeTime(qint64 time) { explodeTime = time; }

private:
    QPointF fallingVelocity{QPointF(0, 0)};
    bool isFalling{true};
    bool isOnGround{false};
    qint64 createTime{0};
    bool exploding{false};
    qint64 explodeTime{0};
    static constexpr qreal BOMB_GRAVITY = 1;
};

class FallingController2 : public QObject
{
    Q_OBJECT
public:
    explicit FallingController2(QObject *parent = nullptr);

    void setFallingParams(int fallingInterval);
    void setVanishParams(int VanishInterval);
    void startLoop();
    void processMovement();
    void setFallingRange(QRectF range);
    void setScene(QGraphicsScene* scene);
    void setWeaponLifetime(int lifetime) { weaponLifetime = lifetime; }
    void setGroundLevel(qreal level) { groundLevel = level; }
    void stopAllTimers();
    const std::vector<Bomb*>& getBombs() const { return createdBombs; }

protected:
    std::vector<Weapon*> createdWeapon;
    std::vector<Drug*> createdDrug;
    std::vector<Armor*> createdArmor;
    std::vector<Bomb*> createdBombs;
    QGraphicsScene* parentScene;
    qreal groundLevel{0};
    QMediaPlayer* explodePlayer;
    QAudioOutput* explodeOutput;

protected slots:
    void fall();
    void vanish();

private:
    QTimer *fallingTimer;
    QTimer *vanishTimer;
    int fallingInterval;
    int vanishInterval;
    QRectF fallingRange;
    int weaponLifetime{4700};

    void createExplosionEffect(const QPointF& position);
};

#endif
