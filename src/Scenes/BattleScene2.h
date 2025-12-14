#ifndef BATTLESCENE2_H
#define BATTLESCENE2_H

#include <QKeyEvent>
#include <QDateTime>
#include "../FallingController/fishgenerator.h"
#include "../UI/HealthBar.h"
#include "Scene.h"
#include "../Items/Maps/Map.h"
#include "../Items/Characters/Character.h"
#include "../FallingController/fallingcontroller.h"
#include <QList>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class BattleScene2 : public Scene {
    Q_OBJECT

public:
    explicit BattleScene2(QObject *parent);
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
    void takeDamage();
    void updateHealthBars();
    void setupHealthBars();
    void checkVictory();
    void checkGameOver();
    void updateFishCounter();
    void updateTimer();
    void playEatSound();
    void showScorePopup(const QPointF& position, QString score);
    Drug *findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold = std::numeric_limits<qreal>::max());
    
    Map *map;
    Character *myfish;
    HealthBar* myFishHealthBar;
    FallingController* fallingController;
    FishGenerator* fishGenerator;
    
    bool gameEnded;
    bool isInvincible;
    qint64 lastDamageTime;
    qint64 gameStartTime;
    int lives;
    int fishEaten;
    
    QMediaPlayer* eatSoundPlayer;
    QAudioOutput* audioOutput;
    QGraphicsTextItem* fishCounterText;
    QGraphicsTextItem* timerText;
    
    static constexpr qint64 INVINCIBILITY_DURATION = 2000;
    static constexpr int MAX_LIVES = 3;
    static constexpr int WIN_FISH_COUNT = 300;
    static constexpr qint64 TIME_LIMIT = 60000;
};

#endif