#include <QDateTime>
#include "BattleScene3.h"
#include "../Items/Characters/myfish.h"
#include "../Items/Maps/Battlefield3.h"
#include "../FallingController/fallingcontroller2.h"
#include "../Items/Drugs/medbag.h"
#include <QTimer>
#include <QFont>
#include <QPropertyAnimation>

BattleScene3::BattleScene3(QObject *parent) : Scene(parent)
    , gameEnded(false)
    , lives(MAX_LIVES)
    , fishEaten(0)
    , myFishHealthBar(nullptr)
    , fishCounterText(nullptr)
    , isInvincible(false)
    , lastDamageTime(0) {
    
    setSceneRect(0, 0, 1180, 640);
    
    map = new Battlefield3();
    myfish = new MyFish();
    
    addItem(map);
    addItem(myfish);
    
    map->scaleToFitScene(this);
    myfish->setPos(sceneRect().left(), map->getFloorHeight());
    
    setupHealthBars();
    
    eatSoundPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    eatSoundPlayer->setAudioOutput(audioOutput);
    eatSoundPlayer->setSource(QUrl("qrc:/eat.mp3"));
    audioOutput->setVolume(0.5);
    
    fishGenerator = new FishGenerator(this);
    fishGenerator->setScene(this);
    fishGenerator->setSpawnRange(map->sceneBoundingRect());
    fishGenerator->setSpawnRate(2.0, 2.0);
    fishGenerator->setSharkSpawnRate(0.3);
    fishGenerator->setVelocityRange(0.15, 0.35, -0.1, 0.1);
    fishGenerator->setTargetFish(myfish);
    fishGenerator->setMidfishSpawnRate(0.5);
    fishGenerator->startGeneration();
    
    fallingController = new FallingController2();
    fallingController->setScene(this);
    fallingController->setFallingParams(3000);
    fallingController->setVanishParams(1000);
    fallingController->setFallingRange(map->sceneBoundingRect());
    fallingController->setGroundLevel(map->getFloorHeight()-60);
    fallingController->startLoop();
}

void BattleScene3::setupHealthBars() {
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
    
    updateHealthBars();
    updateFishCounter();
}

void BattleScene3::updateHealthBars() {
    if (myFishHealthBar) {
        myFishHealthBar->setHealthValues(lives, MAX_LIVES);
    }
}

void BattleScene3::updateFishCounter() {
    if (fishCounterText) {
        fishCounterText->setPlainText(QString("Fish: %1/%2").arg(fishEaten).arg(WIN_FISH_COUNT));
    }
}

void BattleScene3::processInput() {
    Scene::processInput();
    if (myfish != nullptr) {
        myfish->processInput();
    }
}

void BattleScene3::keyPressEvent(QKeyEvent *event) {
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

void BattleScene3::keyReleaseEvent(QKeyEvent *event) {
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

void BattleScene3::update() {
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
    
    if (myfish && fishGenerator) {
        checkFishCollision();
    }
    
    checkBombCollision();
    checkVictory();
    checkGameOver();
}

void BattleScene3::processMovement() {
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

void BattleScene3::processPicking() {
    Scene::processPicking();
    auto drug = findNearestUnmountedDrug(myfish->pos(), 200);
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

Drug* BattleScene3::findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold) {
    Drug *nearest = nullptr;
    qreal minDistance = distance_threshold;
    
    for (QGraphicsItem *item: items()) {
        if (auto weapon = dynamic_cast<Drug *>(item)) {
            if (!weapon->getIsPicked()) {
                qreal distance = QLineF(pos, item->sceneBoundingRect().center()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = weapon;
                }
            }
        }
    }
    
    return nearest;
}

void BattleScene3::checkFishCollision() {
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

void BattleScene3::checkBombCollision() {
    if (gameEnded || isInvincible) return;
    
    QRectF myFishBounds = myfish->sceneBoundingRect();
    const std::vector<Bomb*>& allBombs = fallingController->getBombs();
    
    for (Bomb* bomb : allBombs) {
        if (!bomb) continue;
        
        QRectF bombBounds = bomb->sceneBoundingRect();
        QPointF bombCenter = bombBounds.center();
        QPointF fishCenter = myFishBounds.center();
        
        qreal dx = (bombCenter.x() - fishCenter.x()) / (bombBounds.width() * 0.5);
        qreal dy = (bombCenter.y() - fishCenter.y()) / (bombBounds.height() * 0.5);
        qreal normalizedDistance = qSqrt(dx * dx + dy * dy);
        
        qreal fx = 1.0 / (myFishBounds.width() * 0.5);
        qreal fy = 1.0 / (myFishBounds.height() * 0.5);
        normalizedDistance += qSqrt(fx * fx + fy * fy) * 0.5;
        
        if (normalizedDistance < 1.5) {

            takeDamage();
            playEatSound();
            showScorePopup(bomb->pos(), "💣");
            return;
        }
    }
}

void BattleScene3::takeDamage() {
    if (gameEnded) return;
    
    lives--;
    updateHealthBars();
    isInvincible = true;
    lastDamageTime = QDateTime::currentMSecsSinceEpoch();
    
    if (lives <= 0) {
        checkGameOver();
    }
}

void BattleScene3::checkVictory() {
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
            emit gameOver("Victory! Level 3 Complete!");
        });
    }
}

void BattleScene3::checkGameOver() {
    if (gameEnded) return;
    
    if (lives <= 0) {
        gameEnded = true;
        Scene::stopAllTimers();
        if (fallingController) {
            fallingController->stopAllTimers();
        }
        if (fishGenerator) {
            fishGenerator->stopGeneration();
        }
        
        QTimer::singleShot(100, [this]() {
            emit gameOver("Game Over!");
        });
    }
}

void BattleScene3::playEatSound() {
    if (eatSoundPlayer->playbackState() == QMediaPlayer::PlayingState) {
        eatSoundPlayer->stop();
    }
    eatSoundPlayer->setPosition(0);
    eatSoundPlayer->play();
}

void BattleScene3::showScorePopup(const QPointF& position, QString score) {
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
