//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QDateTime>
#include "../UI/HealthBar.h"
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../FallingController/fallingcontroller.h"
#include "../Items/Weapons/ball.h"
#include "../Items/Weapons/knife.h"
#include "../Items/Armors/Armor.h"
#include "../Items/Maps/grass.h"
#include "../Items/Maps/ice.h"
#include "Platform.h"  // 平台头文件
#include <QList>       // 用于管理平台列表
class BattleScene : public Scene {
Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    void processInput() override;

    void processMovement() override;

    void processPicking() override;

    void processPhysics();

    void processCombat();

    void createPlatforms();

    void createGrass();
    void createIce();
    Platform* findSupportingPlatform(const QRectF& characterBounds, qreal velocityY = 0);
    qreal findGroundLevel(const QPointF& position, qreal defaultGround);
protected slots:

    void update() override;

signals:
    void gameOver(const QString& winner);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void checkGrassVisibility();
    bool isCharacterInAnyGrass(Character* character);
    QList<Grass*> grassList;
    QList<Ice*> iceList;
    void checkIceSpeedUp();
    bool isCharacterInAnyIce(Character* character);
    void stopAllTimers();
    Weapon *findNearestUnmountedWeapon(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    Drug *findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    Armor *findNearestUnmountedArmor(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    static Mountable * pickup(Character *character, Mountable *mountable);
    void processCharacterMovement(Character* character);
    QPointF handleVerticalCollision(Character* character, QPointF newPos, QPointF velocity);
    Platform* findCollidingPlatform(const QPointF& currentPos, const QPointF& newPos, const QRectF& charBounds, const QPointF& velocity);
    void checkAttackCollision(Character* attacker, Character* target);
    void updateHealthBars();
    void setupHealthBars();
    // 子弹相关处理
    void processBullets();
    void processBalls();
    void checkBulletCollision(Bullet* bullet);
    void checkBallCollision(Ball* ball);
    void processKnife();
    void checkKnifeCollision(Knife* knife);
    Map *map;
    Character *character;
    Character *character2;
    Armor *spareArmor;
    HealthBar* player1HealthBar;
    HealthBar* player2HealthBar;
    FallingController* fallingController;
    QList<Platform*> platforms;
    void checkGameOver();
    bool gameEnded;
};


#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
