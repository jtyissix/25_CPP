

#ifndef QT_PROGRAMMING_2024_BATTLESCENE_H
#define QT_PROGRAMMING_2024_BATTLESCENE_H

#include <QKeyEvent>
#include <QDateTime>
#include "../FallingController/fishgenerator.h"
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
#include "Platform.h"
#include <QList>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QAudioOutput>
class BattleScene : public Scene {
Q_OBJECT

public:
    explicit BattleScene(QObject *parent);

    void processInput() override;

    void processPicking() override;

    void processMovement() override;

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
    FishGenerator* fishGenerator;
    void checkFishCollision();
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
    void processBullets();
    void processBalls();
    void checkBulletCollision(Bullet* bullet);
    void checkBallCollision(Ball* ball);
    void processKnife();
    void checkKnifeCollision(Knife* knife);
    void checkGameOver();
    void takeDamage();
    void updateFishCounter();
    void checkVictory();
    
    Map *map;
    Character *character;
    Character *character2;
    Character *myfish;
    Armor *spareArmor;
    HealthBar* player1HealthBar;
    HealthBar* player2HealthBar;
    HealthBar* myFishHealthBar;
    FallingController* fallingController;
    QList<Platform*> platforms;
    bool gameEnded;
    bool isInvincible;
    qint64 lastDamageTime;
    static constexpr qint64 INVINCIBILITY_DURATION = 2000; // 2秒无敌时间
    int lives;
    int fishEaten;
    QMediaPlayer* eatSoundPlayer;
    QAudioOutput* audioOutput;


    void playEatSound();
    void showScorePopup(const QPointF& position, QString score);
    QGraphicsTextItem* fishCounterText;
    static constexpr int MAX_LIVES = 3;
    static constexpr int WIN_FISH_COUNT = 200;
};


#endif //QT_PROGRAMMING_2024_BATTLESCENE_H
