#ifndef BATTLESCENE3_H
#define BATTLESCENE3_H

#include <QKeyEvent>
#include <QDateTime>
#include "../FallingController/fishgenerator.h"
#include "../FallingController/fallingcontroller2.h"
#include "../UI/HealthBar.h"
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include <QList>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class BattleScene3 : public Scene {
    Q_OBJECT

public:
    explicit BattleScene3(QObject *parent);
    void processInput() override;
    void processPicking() override;
    void processMovement() override;

protected slots:
    void update() override;

signals:
    void gameOver(const QString& winner);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void checkFishCollision();
    void checkBombCollision();
    void takeDamage();
    void updateHealthBars();
    void setupHealthBars();
    void checkVictory();
    void checkGameOver();
    void updateFishCounter();
    void playEatSound();
    void showScorePopup(const QPointF& position, QString score);
    Drug *findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    
    Map *map;
    Character *myfish;
    HealthBar* myFishHealthBar;
    FallingController2* fallingController;
    FishGenerator* fishGenerator;
    
    bool gameEnded;
    bool isInvincible;
    qint64 lastDamageTime;
    int lives;
    int fishEaten;
    
    QMediaPlayer* eatSoundPlayer;
    QAudioOutput* audioOutput;
    QGraphicsTextItem* fishCounterText;
    
    static constexpr qint64 INVINCIBILITY_DURATION = 2000;
    static constexpr int MAX_LIVES = 5;
    static constexpr int WIN_FISH_COUNT = 60;
};

#endif
