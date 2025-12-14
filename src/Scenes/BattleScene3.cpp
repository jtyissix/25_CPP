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
    myfish->setPos(sceneRect().center().x(), sceneRect().center().y());
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
        if (myfish) {
            myfish->setLeftDown(true);
            myfish->setDirection(false);
        }
        break;
    case Qt::Key_Right:
        if (myfish) {
            myfish->setRightDown(true);
            myfish->setDirection(true);
        }
        break;
    case Qt::Key_Up:
        if (myfish) {
            myfish->setUp(true);
        }
        break;
    case Qt::Key_Down:
        if (myfish) {
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
        if (myfish) {
            myfish->setLeftDown(false);
        }
        break;
    case Qt::Key_Right:
        if (myfish) {
            myfish->setRightDown(false);
        }
        break;
    case Qt::Key_Up:
        if (myfish) {
            myfish->setUp(false);
        }
        break;
    case Qt::Key_Down:
        if (myfish) {
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

    if (fallingController) {
        fallingController->processMovement();
    }

    if (fishGenerator) {
        fishGenerator->updateFishMovement(deltaTime);
    }
}

void BattleScene3::processPicking() {
    Scene::processPicking();
    auto drug = findNearestUnmountedDrug(myfish->pos(), 20);
    if (drug) {
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
        if (auto drug = dynamic_cast<Drug*>(item)) {
            if (!drug->getIsPicked()) {
                QPointF drugCenter = drug->sceneBoundingRect().center();
                QPointF fishCenter = pos + QPointF(32, 32);

                qreal dx = (drugCenter.x() - fishCenter.x()) ;
                qreal dy = (drugCenter.y() - fishCenter.y());
                qreal distance = qSqrt(dx * dx + dy * dy);

                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = drug;
                }
            }
        }
    }

    return nearest;
}

void BattleScene3::checkFishCollision() {
    if (gameEnded || isInvincible) return;

    QRectF myFishBounds = myfish->sceneBoundingRect();
    QPointF fishCenter = myFishBounds.center();

    const QList<FishInfo>& allFish = fishGenerator->getAllFish();

    for (const FishInfo& info : allFish) {
        Character* smallFish = info.fish;
        if (!smallFish) continue;

        QRectF smallFishBounds = smallFish->sceneBoundingRect();
        QPointF smallFishCenter = smallFishBounds.center();

        qreal dx = (smallFishCenter.x() - fishCenter.x()) / 40.0;
        qreal dy = (smallFishCenter.y() - fishCenter.y()) / 40.0;
        qreal distance = qSqrt(dx * dx + dy * dy);

        if (distance < 1.2) {
            if (MyFish* myFishPtr = dynamic_cast<MyFish*>(myfish)) {
                bool fishIsOnRight = (smallFishCenter.x() > fishCenter.x());
                myFishPtr->startEatingAnimation(fishIsOnRight);
            }

            fishGenerator->removeFish(smallFish);
            fishEaten++;
            updateFishCounter();
            playEatSound();
            showScorePopup(smallFishCenter, "+1");
            break;
        }
    }

    const QList<shark*>& allSharks = fishGenerator->getAllSharks();
    for (shark* sharkEnemy : allSharks) {
        if (!sharkEnemy) continue;

        QRectF sharkBounds = sharkEnemy->sceneBoundingRect();
        QPointF sharkCenter = sharkBounds.center();

        qreal dx = (sharkCenter.x() - fishCenter.x()) / (sharkBounds.width() * 0.4);
        qreal dy = (sharkCenter.y() - fishCenter.y()) / (sharkBounds.height() * 0.35);
        qreal distance = qSqrt(dx * dx + dy * dy);

        if (distance < 1.5) {
            if (myfish->hasShield() && myfish->consumeShield()) {
                showScorePopup(fishCenter, "免伤");
                isInvincible = true;
                lastDamageTime = QDateTime::currentMSecsSinceEpoch();
                return;
            }
            takeDamage();
            playEatSound();
            showScorePopup(sharkCenter, "-1");
            return;
        }
    }

    const QList<midfish*>& allMidfish = fishGenerator->getAllMidfish();
    for (midfish* midfishEnemy : allMidfish) {
        if (!midfishEnemy) continue;

        QRectF midfishBounds = midfishEnemy->sceneBoundingRect();
        QPointF midfishCenter = midfishBounds.center();

        qreal dx = (midfishCenter.x() - fishCenter.x()) / (midfishBounds.width() * 0.4);
        qreal dy = (midfishCenter.y() - fishCenter.y()) / (midfishBounds.height() * 0.35);
        qreal distance = qSqrt(dx * dx + dy * dy);

        if (distance < 1.5) {
            if (myfish->hasShield() && myfish->consumeShield()) {
                showScorePopup(fishCenter, "免伤");
                isInvincible = true;
                lastDamageTime = QDateTime::currentMSecsSinceEpoch();
                return;
            }
            takeDamage();
            playEatSound();
            showScorePopup(midfishCenter, "-1");
            return;
        }
    }
}

void BattleScene3::checkBombCollision() {
    if (gameEnded || isInvincible) return;

    QRectF myFishBounds = myfish->sceneBoundingRect();
    QPointF fishCenter = myFishBounds.center();

    const std::vector<Bomb*>& allBombs = fallingController->getBombs();
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    for (Bomb* bomb : allBombs) {
        if (!bomb || !bomb->isExploding()) continue;

        if (currentTime - bomb->getExplodeTime() > 500) continue;

        QPointF bombCenter = bomb->pos() + QPointF(20, 20);

        qreal dx = (bombCenter.x() - fishCenter.x()) / 80.0;
        qreal dy = (bombCenter.y() - fishCenter.y()) / 80.0;
        qreal distance = qSqrt(dx * dx + dy * dy);

        if (distance < 1.5) {
            lives=0;
            showScorePopup(bombCenter, "💣");
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
            emit gameOver("Victory! 全部通关!");
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
