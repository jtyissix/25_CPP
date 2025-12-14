#include <QDateTime>
#include "BattleScene2.h"
#include "../Items/Characters/myfish.h"
#include "../Items/Maps/Battlefield2.h"
#include "../FallingController/fallingcontroller.h"
#include "../Items/Drugs/medbag.h"
#include <QTimer>
#include <QFont>
#include <QPropertyAnimation>
#include <QDebug>

BattleScene2::BattleScene2(QObject *parent) : Scene(parent)
    , gameEnded(false)
    , lives(MAX_LIVES)
    , fishEaten(0)
    , myFishHealthBar(nullptr)
    , fishCounterText(nullptr)
    , timerText(nullptr)
    , isInvincible(false)
    , lastDamageTime(0)
    , gameStartTime(QDateTime::currentMSecsSinceEpoch()) {
    
    setSceneRect(0, 0, 1180, 640);
    
    map = new Battlefield2();
    myfish = new MyFish();
    
    addItem(map);
    addItem(myfish);
    
    map->scaleToFitScene(this);
    myfish->setPos(sceneRect().left(), map->getFloorHeight());
    myfish->moveSpeed=0.6;
    setupHealthBars();
    
    eatSoundPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    eatSoundPlayer->setAudioOutput(audioOutput);
    eatSoundPlayer->setSource(QUrl("qrc:/eat.mp3"));
    audioOutput->setVolume(0.5);
    
    fishGenerator = new FishGenerator(this);
    fishGenerator->setScene(this);
    fishGenerator->setSpawnRange(map->sceneBoundingRect());
    fishGenerator->setSpawnRate(2.5, 2.5);
    fishGenerator->setSharkSpawnRate(0.4);
    fishGenerator->setVelocityRange(0.2, 0.4, -0.15, 0.15);
    fishGenerator->setTargetFish(myfish);
    fishGenerator->setMidfishSpawnRate(0.6);
    fishGenerator->startGeneration();
    
    fallingController = new FallingController();
    fallingController->setScene(this);
    fallingController->setFallingParams(4000);
    fallingController->setVanishParams(1000);
    fallingController->setFallingRange(map->sceneBoundingRect());
    fallingController->setGroundLevel(map->getFloorHeight()-60);
    fallingController->startLoop();
    
    gameStartTime = QDateTime::currentMSecsSinceEpoch();
}

void BattleScene2::setupHealthBars() {
    myFishHealthBar = new HealthBar("Lives");
    myFishHealthBar->setMaxWidth(200);
    myFishHealthBar->setPosition(20, 50, false);
    addItem(myFishHealthBar);
    
    fishCounterText = new QGraphicsTextItem();
    QFont font("Arial", 16, QFont::Bold);
    fishCounterText->setFont(font);
    fishCounterText->setDefaultTextColor(QColor(255, 255, 255));
    fishCounterText->setPos(sceneRect().width() - 200, 20);
    addItem(fishCounterText);
    
    timerText = new QGraphicsTextItem();
    timerText->setFont(font);
    timerText->setDefaultTextColor(QColor(255, 50, 50));
    timerText->setPos(sceneRect().width() / 2 - 50, 20);
    addItem(timerText);
    
    updateHealthBars();
    updateFishCounter();
    updateTimer();
}

void BattleScene2::updateHealthBars() {
    if (myFishHealthBar) {
        myFishHealthBar->setHealthValues(lives, MAX_LIVES);
    }
}

void BattleScene2::updateFishCounter() {
    if (fishCounterText) {
        fishCounterText->setPlainText(QString("Fish: %1/%2").arg(fishEaten).arg(WIN_FISH_COUNT));
    }
}

void BattleScene2::updateTimer() {
    if (timerText && !gameEnded) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        qint64 elapsedTime = currentTime - gameStartTime;
        qint64 remainingTime = TIME_LIMIT - elapsedTime;
        
        if (remainingTime <= 0) {
            timerText->setPlainText("00:00");
            checkGameOver();
        } else {
            int seconds = remainingTime / 1000;
            int minutes = seconds / 60;
            seconds = seconds % 60;
            timerText->setPlainText(QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0')));
        }
    }
}

void BattleScene2::processInput() {
    Scene::processInput();
    if (myfish != nullptr) {
        myfish->processInput();
    }
}

void BattleScene2::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
        if (myfish != nullptr) {
            myfish->setLeftDown(true);
            myfish->setDirection(false);
        }
        break;
    case Qt::Key_Right:
        if (myfish != nullptr) {
            myfish->setRightDown(true);
            myfish->setDirection(true);
        }
        break;
    case Qt::Key_Up:
        if (myfish != nullptr) {
            myfish->setUp(true);
        }
        break;
    case Qt::Key_Down:
        if (myfish != nullptr) {
            myfish->setDown(true);
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
}

void BattleScene2::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
        if (myfish != nullptr) {
            myfish->setLeftDown(false);
        }
        break;
    case Qt::Key_Right:
        if (myfish != nullptr) {
            myfish->setRightDown(false);
        }
        break;
    case Qt::Key_Up:
        if (myfish != nullptr) {
            myfish->setUp(false);
        }
        break;
    case Qt::Key_Down:
        if (myfish != nullptr) {
            myfish->setDown(false);
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
}

void BattleScene2::update() {
    if (gameEnded) {
        return;
    }
    
    if (isInvincible) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - lastDamageTime >= INVINCIBILITY_DURATION) {
            isInvincible = false;
        }
    }
    
    Scene::update();
    updateTimer();
    
    if (myfish && fishGenerator) {
        checkFishCollision();
    }
    
    checkVictory();
    checkGameOver();
}

void BattleScene2::processMovement() {
    Scene::processMovement();
    
    if (myfish != nullptr) {
        QPointF newPos = myfish->pos() + myfish->getVelocity() * (double) deltaTime;
        
        if (newPos.x() < map->boundingRect().left()) {
            newPos.setX(map->boundingRect().left());
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        } else if (newPos.x() + 64 > map->boundingRect().right()) {
            newPos.setX(map->boundingRect().right() - 64);
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        }
        
        int fishHeight = 64;
        if (newPos.y() < map->boundingRect().top()) {
            newPos.setY(map->boundingRect().top());
            QPointF vel = myfish->getVelocity();
            vel.setY(0);
            myfish->setVelocity(vel);
        } else if (newPos.y() + fishHeight > map->boundingRect().bottom()) {
            newPos.setY(map->boundingRect().bottom() - fishHeight);
            myfish->setIsOnGround(true);
            QPointF vel = myfish->getVelocity();
            vel.setY(0);
            myfish->setVelocity(vel);
        } else {
            myfish->setIsOnGround(false);
        }
        
        myfish->setPos(newPos);
    }
    
    if (fallingController != nullptr) {
        fallingController->processMovement();
    }
    
    if (fishGenerator) {
        fishGenerator->updateFishMovement(deltaTime);
    }
}

void BattleScene2::processPicking() {
    Scene::processPicking();
    auto drug = findNearestUnmountedDrug(myfish->pos(), 20);
    if (drug != nullptr) {
        myfish->pickUpDrug(drug);
        if (dynamic_cast<MedBag*>(drug)) {
            if (lives < MAX_LIVES) {
                lives++;
                updateHealthBars();
            }
        }
    }
}

Drug* BattleScene2::findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold) {
    Drug *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto drug = dynamic_cast<Drug*>(item)) {
            if (!drug->getIsPicked()) {
                QPointF drugCenter = drug->sceneBoundingRect().center();
                QPointF fishCenter = pos + QPointF(32, 32);

                qreal dx = (drugCenter.x() - fishCenter.x()) ;
                qreal dy = (drugCenter.y() - fishCenter.y()) ;
                qreal distance = qSqrt(dx * dx + dy * dy);
                qDebug()<<distance;
                if (distance < minDistance ) {
                    //minDistance = distance * 50.0;
                    nearest = drug;
                }
            }
        }
    }

    return nearest;
}

void BattleScene2::checkFishCollision() {
    if (gameEnded || isInvincible) return;
    
    QRectF myFishBounds = myfish->sceneBoundingRect();
    const QList<FishInfo>& allFish = fishGenerator->getAllFish();
    
    for (const FishInfo& info : allFish) {
        Character* smallFish = info.fish;
        if (!smallFish) continue;
        
        QRectF smallFishBounds = smallFish->sceneBoundingRect();
        
        if (myFishBounds.intersects(smallFishBounds)) {
            QPointF myFishPos = myfish->pos();
            QPointF smallFishPos = smallFish->pos();
            bool fishIsOnRight = (smallFishPos.x() > myFishPos.x());
            
            if (MyFish* myFishPtr = dynamic_cast<MyFish*>(myfish)) {
                myFishPtr->startEatingAnimation(fishIsOnRight);
            }
            
            fishGenerator->removeFish(smallFish);
            fishEaten++;
            updateFishCounter();
            playEatSound();
            showScorePopup(smallFishPos, "+1");
            break;
        }
    }
    
    const QList<shark*>& allSharks = fishGenerator->getAllSharks();
    for (shark* sharkEnemy : allSharks) {
        if (!sharkEnemy) continue;
        
        QRectF sharkBounds = sharkEnemy->sceneBoundingRect();
        QPointF sharkCenter = sharkBounds.center();
        QPointF fishCenter = myFishBounds.center();
        
        qreal dx = (sharkCenter.x() - fishCenter.x()) / (sharkBounds.width() * 0.4);
        qreal dy = (sharkCenter.y() - fishCenter.y()) / (sharkBounds.height() * 0.35);
        qreal normalizedDistance = qSqrt(dx * dx + dy * dy);
        
        qreal fx = 1.0 / (myFishBounds.width() * 0.4);
        qreal fy = 1.0 / (myFishBounds.height() * 0.35);
        normalizedDistance += qSqrt(fx * fx + fy * fy) * 0.5;
        
        if (normalizedDistance < 1.5) {
            if (myfish->hasShield() && myfish->consumeShield()) {
                showScorePopup(myfish->pos(), "免伤");
                isInvincible = true;
                lastDamageTime = QDateTime::currentMSecsSinceEpoch();
                return;
            }
            takeDamage();
            playEatSound();
            showScorePopup(sharkEnemy->pos(), "-1");
            return;
        }
    }
    
    const QList<midfish*>& allMidfish = fishGenerator->getAllMidfish();
    for (midfish* midfishEnemy : allMidfish) {
        if (!midfishEnemy) continue;
        
        QRectF midfishBounds = midfishEnemy->sceneBoundingRect();
        QPointF midfishCenter = midfishBounds.center();
        QPointF fishCenter = myFishBounds.center();
        
        qreal dx = (midfishCenter.x() - fishCenter.x()) / (midfishBounds.width() * 0.4);
        qreal dy = (midfishCenter.y() - fishCenter.y()) / (midfishBounds.height() * 0.35);
        qreal normalizedDistance = qSqrt(dx * dx + dy * dy);
        
        qreal fx = 1.0 / (myFishBounds.width() * 0.4);
        qreal fy = 1.0 / (myFishBounds.height() * 0.35);
        normalizedDistance += qSqrt(fx * fx + fy * fy) * 0.5;
        
        if (normalizedDistance < 1.5) {
            if (myfish->hasShield() && myfish->consumeShield()) {
                showScorePopup(myfish->pos(), "免伤");
                isInvincible = true;
                lastDamageTime = QDateTime::currentMSecsSinceEpoch();
                return;
            }
            takeDamage();
            playEatSound();
            showScorePopup(midfishEnemy->pos(), "-1");
            return;
        }
    }
}

void BattleScene2::takeDamage() {
    if (gameEnded) return;
    
    lives--;
    updateHealthBars();
    isInvincible = true;
    lastDamageTime = QDateTime::currentMSecsSinceEpoch();
    
    if (lives <= 0) {
        checkGameOver();
    }
}

void BattleScene2::checkVictory() {
    if (gameEnded) return;
    
    if (fishEaten >= WIN_FISH_COUNT) {
        gameEnded = true;
        Scene::stopAllTimers();
        if (fallingController) {
            fallingController->stopAllTimers();
        }
        if (fishGenerator) {
            fishGenerator->stopGeneration();
        }
        
        QTimer::singleShot(100, [this]() {
            emit gameOver("Victory!解锁下一关，获得技能：生命增多");
        });
    }
}

void BattleScene2::checkGameOver() {
    if (gameEnded) return;
    
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsedTime = currentTime - gameStartTime;
    
    if (lives <= 0 || elapsedTime >= TIME_LIMIT) {
        gameEnded = true;
        Scene::stopAllTimers();
        if (fallingController) {
            fallingController->stopAllTimers();
        }
        if (fishGenerator) {
            fishGenerator->stopGeneration();
        }
        
        QString message = (lives <= 0) ? "Game Over - No Lives!" : "Time's Up!";
        QTimer::singleShot(100, [this, message]() {
            emit gameOver(message);
        });
    }
}

void BattleScene2::playEatSound() {
    if (eatSoundPlayer->playbackState() == QMediaPlayer::PlayingState) {
        eatSoundPlayer->stop();
    }
    eatSoundPlayer->setPosition(0);
    eatSoundPlayer->play();
}

void BattleScene2::showScorePopup(const QPointF& position, QString score) {
    QGraphicsTextItem* scoreText = new QGraphicsTextItem();
    scoreText->setPlainText(score);
    
    QFont font("Arial", 24, QFont::Bold);
    scoreText->setFont(font);
    scoreText->setDefaultTextColor(QColor(255, 50, 50));
    
    scoreText->setHtml(QString(
        "<span style='color: #ff3232; "
        "text-shadow: -2px -2px 0 #000, 2px -2px 0 #000, "
        "-2px 2px 0 #000, 2px 2px 0 #000;'>"
        "%1</span>"
    ).arg(score));
    
    scoreText->setPos(position.x(), position.y() - 50);
    scoreText->setZValue(1000);
    addItem(scoreText);
    
    QPropertyAnimation* moveAnimation = new QPropertyAnimation(scoreText, "pos");
    moveAnimation->setDuration(1000);
    moveAnimation->setStartValue(scoreText->pos());
    moveAnimation->setEndValue(scoreText->pos() + QPointF(0, -80));
    moveAnimation->setEasingCurve(QEasingCurve::OutQuad);
    
    QPropertyAnimation* fadeAnimation = new QPropertyAnimation(scoreText, "opacity");
    fadeAnimation->setDuration(1000);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    fadeAnimation->setEasingCurve(QEasingCurve::InQuad);
    
    connect(moveAnimation, &QPropertyAnimation::finished, [this, scoreText]() {
        removeItem(scoreText);
        delete scoreText;
    });
    
    moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
