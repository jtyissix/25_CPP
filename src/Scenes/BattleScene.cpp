


#include <QDateTime>
#include "BattleScene.h"
#include "../Items/Characters/myfish.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Characters/link2.h"
#include "../FallingController/fallingcontroller.h"
#include "../Items/Characters/myfish.h"
#include "../Items/Armors/NormalArmor.h"
#include "../Items/Armors/BulletArmor.h"
#include "../Items/Maps/grass.h"
#include "../Items/Maps/ice.h"
#include "../Items/Drugs/medbag.h"
#include <QTimer>
#include <QFont>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPropertyAnimation>
BattleScene::BattleScene(QObject *parent) : Scene(parent)
    , gameEnded(false)
    , lives(MAX_LIVES)
    , fishEaten(0)
    , myFishHealthBar(nullptr)
    , fishCounterText(nullptr),isInvincible(false)
    , lastDamageTime(0){
    // This is useful if you want the scene to have the exact same dimensions as the view
    setSceneRect(0, 0, 1180, 640);
    map = new Battlefield();
    myfish=new MyFish();
    //character = new Link();
    //character->id=1;
    //character->setDirection(true);//initially facing right
    //character2 = new Link2();
    //character2->id=2;
    //character2->setDirection(false);//initially facing left
    //spareArmor = new FlamebreakerArmor();
    addItem(map);
    addItem(myfish);
    //createGrass();
    //createIce();
    //addItem(character);
    //addItem(character2);
    //addItem(spareArmor);
    map->scaleToFitScene(this);
    myfish->setPos(sceneRect().left(),map->getFloorHeight());
    //character2->setPos(sceneRect().right()-150,map->getFloorHeight());
    //spareArmor->unmount();
    //spareArmor->setPos(sceneRect().left() + (sceneRect().right() - sceneRect().left()) *0.75, map->getFloorHeight());
    setupHealthBars();
    eatSoundPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    eatSoundPlayer->setAudioOutput(audioOutput);




    eatSoundPlayer->setSource(QUrl("qrc:/eat.mp3"));

    audioOutput->setVolume(0.5);
    //createPlatforms();
    //setup falling controller
    fishGenerator = new FishGenerator(this);
    fishGenerator->setScene(this);
    fishGenerator->setSpawnRange(map->sceneBoundingRect());

    fishGenerator->setSpawnRate(2.0, 2.0);
    fishGenerator->setSharkSpawnRate(0.3);
    fishGenerator->setVelocityRange(0.15, 0.35, -0.1, 0.1);
    fishGenerator->setTargetFish(myfish);
    fishGenerator->setMidfishSpawnRate(0.5);
    fishGenerator->startGeneration();
    
    fallingController = new FallingController();
    fallingController->setScene(this);
    fallingController->setFallingParams(5000);
    fallingController->setVanishParams(1000);
    fallingController->setFallingRange(map->sceneBoundingRect());
    fallingController->setGroundLevel(map->getFloorHeight()-60);//considering picture height
    fallingController->startLoop();
}
void BattleScene::createGrass(){
    grassList.clear();

    auto grass1=new Grass(nullptr);
    grass1->mountToParent(200,620);
    addItem(grass1);
    grassList.append(grass1);

    auto grass2=new Grass(nullptr);
    grass2->mountToParent(250,620);
    addItem(grass2);
    grassList.append(grass2);

    auto grass3=new Grass(nullptr);
    grass3->mountToParent(300,620);
    addItem(grass3);
    grassList.append(grass3);

    auto grass4=new Grass(nullptr);
    grass4->mountToParent(820,180);
    addItem(grass4);
    grassList.append(grass4);

    auto grass5=new Grass(nullptr);
    grass5->mountToParent(870,180);
    addItem(grass5);
    grassList.append(grass5);
}
void BattleScene::createIce(){
    iceList.clear();
    for(int i=0;i<11;i++){
        auto ice1=new Ice(nullptr);
        ice1->mountToParent(650-25*i,460);
        addItem(ice1);
        iceList.append(ice1);
    }

}
void BattleScene::processInput() {
    Scene::processInput();
    if(myfish!=nullptr){
        myfish->processInput();
    }
    /*
    if (character != nullptr) {
        //character->processInput();
    }
    if (character2 != nullptr) {
        //character2->processInput();
    }*/
}

void BattleScene::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        /*
        case Qt::Key_1:
            if (character != nullptr) {
                //character->setLeftDown(true);
                //character->setDirection(false);
            }
            break;
        case Qt::Key_2:
            if (character != nullptr) {
                //character->setRightDown(true);
                //character->setDirection(true);
            }
            break;
        case Qt::Key_3:
            if (character != nullptr) {
                //character->setJumpDown(true);
                //qDebug()<<'c';
            }
            break;
        case Qt::Key_4:
            if (character!= nullptr) {
                //character->setKneeDown(true);
            }
            break;
        case Qt::Key_5:
            if (character!= nullptr) {
                //character->setAttack(true);

            }
            break;
        case Qt::Key_6:
            if (character!= nullptr) {
                //character->setPickDown(true);

            }
            break;
        case Qt::Key_Left:
            if (character2 != nullptr) {
                //character2->setLeftDown(true);
                //character2->setDirection(false);
            }
            break;
        case Qt::Key_Right:
            if (character2 != nullptr) {
                //character2->setRightDown(true);
                //character2->setDirection(true);
            }
            break;
        case Qt::Key_Up:
            if (character2 != nullptr) {
                //character2->setJumpDown(true);

            }
            break;
        case Qt::Key_Down:
            if (character2!= nullptr) {
                //character2->setKneeDown(true);
            }
            break;
        case Qt::Key_Space:
            if (character2!= nullptr) {
                //character2->setAttack(true);
            }
            break;
        case Qt::Key_Shift:
            if (character2!= nullptr) {
                //character2->setPickDown(true);

            }
            break;
        default:
            Scene::keyPressEvent(event);
*/
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
            //character2->setJumpDown(true);
            myfish->setUp(true);
        }
        break;
    case Qt::Key_Down:
        if (myfish!= nullptr) {
            //character2->setKneeDown(true);
            myfish->setDown(true);
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
}

void BattleScene::keyReleaseEvent(QKeyEvent *event) {
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
            //character2->setJumpDown(true);
            myfish->setUp(false);
        }
        break;
    case Qt::Key_Down:
        if (myfish!= nullptr) {
            //character2->setKneeDown(true);
            myfish->setDown(false);
        }
        break;
    default:
        Scene::keyPressEvent(event);
    }
    /*
    switch (event->key()) {

        case Qt::Key_1:
            if (character != nullptr) {
                //character->setLeftDown(false);
            }
            break;
        case Qt::Key_2:
            if (character != nullptr) {
                //character->setRightDown(false);
            }
            break;
        case Qt::Key_3:
            if (character != nullptr) {
                //character->setJumpDown(false);
            }
            break;
        case Qt::Key_4:
            if (character!= nullptr) {
                //character->setKneeDown(false);
            }
            break;
        case Qt::Key_5:
            if (character!= nullptr) {
                //character->setAttack(false);

            }
            break;
        case Qt::Key_6:
            if (character!= nullptr) {
                //character->setPickDown(false);

            }
            break;
        case Qt::Key_Left:
            if (character2 != nullptr) {
                //character2->setLeftDown(false);
            }
            break;
        case Qt::Key_Right:
            if (character2 != nullptr) {
                //character2->setRightDown(false);
            }
            break;
        case Qt::Key_Up:
            if (character2 != nullptr) {
                //character2->setJumpDown(false);

            }
            break;
        case Qt::Key_Down:
            if (character2!= nullptr) {
                //character2->setKneeDown(false);
            }
            break;
        case Qt::Key_Space:
            if (character2!= nullptr) {
                //character2->setAttack(false);
            }
            break;
        case Qt::Key_Shift:
            if (character2!= nullptr) {
                //character2->setPickDown(false);

            }
            break;
        default:
            Scene::keyReleaseEvent(event);

    }
*/
}
//no use function
void BattleScene::processPhysics() {
/*
    if (character != nullptr) {
        //character->processJump();

        //character->applyGravity();
    }
    */
}

void BattleScene::update() {
    if (gameEnded) {
        return;
    }
    // 检查无敌时间是否结束
    if (isInvincible) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - lastDamageTime >= INVINCIBILITY_DURATION) {
            isInvincible = false;
            qDebug() << "Invincibility ended";
        }
    }
    /*
    if (!character || !character2) {

        return;
    }*/
    Scene::update();
    if (myfish && fishGenerator) {
        checkFishCollision();
    }
    checkVictory();
    /*
    processCombat();
    updateHealthBars();

    checkGrassVisibility();

    checkIceSpeedUp();

    processBullets();

    processBalls();

    processKnife();

    checkGameOver();
    //processPhysics();
*/
}

void BattleScene::processMovement() {
    Scene::processMovement();
    if (myfish != nullptr) {
        QPointF oldPos = myfish->pos();

        QPointF newPos = myfish->pos() + myfish->getVelocity() * (double) deltaTime;
        if (newPos.x() < map->boundingRect().left()) {
            newPos.setX( map->boundingRect().left());
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        } else if (newPos.x()+64 > map->boundingRect().right()) {
            newPos.setX(map->boundingRect().right()-64);
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        }

        int fishHeight=64;
        if (newPos.y() < map->boundingRect().top()) {
            newPos.setY(map->boundingRect().top());
            QPointF vel = myfish->getVelocity();
            vel.setY(0);
            myfish->setVelocity(vel);
        }
        else if (newPos.y() + fishHeight > map->boundingRect().bottom()) {
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
    /*
    if (character2 != nullptr) {
        QPointF oldPos = character2->pos();

        QPointF newPos = character2->pos() + character2->getVelocity() * (double) deltaTime;
        if (newPos.x() < map->boundingRect().left()) {
            newPos.setX( map->boundingRect().left());
            QPointF vel = character->getVelocity();
            vel.setX(0);
            character2->setVelocity(vel);
        } else if (newPos.x() > map->boundingRect().right()) {
            newPos.setX(map->boundingRect().right());
            QPointF vel = character->getVelocity();
            vel.setX(0);
            character2->setVelocity(vel);
        }

        qreal groundLevel = map->getFloorHeight();

        if (newPos.y() >= groundLevel) {
            newPos.setY(groundLevel);
            character2->setIsOnGround(true);
            QPointF vel = character->getVelocity();
            vel.setY(0);
            character2->setVelocity(vel);
        } else {
            character2->setIsOnGround(false);
        }

        character2->setPos(newPos);
    }
*/
    if (fallingController != nullptr) {
        fallingController->processMovement();
    }
    if (fishGenerator) {
        fishGenerator->updateFishMovement(deltaTime);
    }

}

/*
void BattleScene::processMovement() {
    Scene::processMovement();

    if (character != nullptr) {
        processCharacterMovement(character);
    }

    if (character2 != nullptr) {
        processCharacterMovement(character2);
    }

    if (fallingController != nullptr) {
        fallingController->processMovement();
    }

}*/
/*
void BattleScene::processCharacterMovement(Character* character) {
    if (!character) return;

    QPointF currentPos = character->pos();
    QPointF velocity = character->getVelocity();

    QPointF newPos = currentPos + velocity * (double) deltaTime;

    if (newPos.x() < map->boundingRect().left()) {
        newPos.setX(map->boundingRect().left());
        velocity.setX(0);
        character->setVelocity(velocity);
    } else if (newPos.x() > map->boundingRect().right()) {
        newPos.setX(map->boundingRect().right());
        velocity.setX(0);
        character->setVelocity(velocity);
    }

    if (newPos.y() < map->boundingRect().top()+100) {
        newPos.setY(map->boundingRect().top()+100);
        velocity.setY(0);
        character->setVelocity(velocity);
    }

    newPos = handleVerticalCollision(character, newPos, velocity);

    character->setPos(newPos);
}

QPointF BattleScene::handleVerticalCollision(Character* character, QPointF newPos, QPointF velocity) {
    QRectF charBounds = character->getCollisionBounds();
    qreal charWidth = charBounds.width();
    qreal charHeight = charBounds.height();

    QRectF newCharBounds(newPos.x() - charWidth/2, newPos.y() - charHeight, charWidth, charHeight);

    qreal groundLevel = map->getFloorHeight();

    Platform* collidingPlatform = findCollidingPlatform(character->pos(), newPos, newCharBounds, velocity);

    if (collidingPlatform) {
        QRectF platformRect = collidingPlatform->getCollisionRect();

        if (velocity.y() > 0) {
            qreal platformTop = platformRect.top();
            newPos.setY(platformTop);
            character->setIsOnGround(true);
            velocity.setY(0);
            character->setVelocity(velocity);


        } else if (velocity.y() < 0) {
            qreal platformBottom = platformRect.bottom();
            newPos.setY(platformBottom + charHeight);
            velocity.setY(0);
            character->setVelocity(velocity);

        }
    } else if (newPos.y() >= groundLevel) {
        newPos.setY(groundLevel);
        character->setIsOnGround(true);
        velocity.setY(0);
        character->setVelocity(velocity);

    } else {
        character->setIsOnGround(false);
    }

    return newPos;
}


Platform* BattleScene::findCollidingPlatform(const QPointF& currentPos, const QPointF& newPos, const QRectF& charBounds, const QPointF& velocity) {
    for (Platform* platform : platforms) {
        QRectF platformRect = platform->getCollisionRect();

        bool horizontalOverlap = !(charBounds.right() < platformRect.left() ||
                                   charBounds.left() > platformRect.right());

        if (!horizontalOverlap) continue;

        if (velocity.y() > 0) {
            qreal charFootCurrent = currentPos.y();
            qreal charFootNew = newPos.y();
            qreal platformTop = platformRect.top();

            bool wasAbove = charFootCurrent <= platformTop;
            bool willBeBelow = charFootNew >= platformTop;

            if (wasAbove && willBeBelow) {

                return platform;
            }

        } else if (velocity.y() < 0) {
            QRectF currentCharBounds = character->getCollisionBounds();
            qreal charHeadCurrent = currentCharBounds.top();
            qreal charHeadNew = charBounds.top();
            qreal platformBottom = platformRect.bottom();

            qreal distance = platformBottom - charHeadNew;

            if (distance >= 0 && distance <= 50 && velocity.y() < 0) {

                return platform;
            }
        }
    }

    return nullptr;
}

Platform* BattleScene::findSupportingPlatform(const QRectF& characterBounds, qreal velocityY) {
    for (Platform* platform : platforms) {
        QRectF platformRect = platform->getCollisionRect();

        bool horizontalOverlap = !(characterBounds.right() < platformRect.left() ||
                                   characterBounds.left() > platformRect.right());

        if (horizontalOverlap) {
            qreal charBottom = characterBounds.bottom();
            qreal platformTop = platformRect.top();

            if (abs(charBottom - platformTop) <= 2) {
                return platform;
            }
        }
    }

    return nullptr;
}
*/
void BattleScene::processPicking() {
    Scene::processPicking();

        //auto weapon = findNearestUnmountedWeapon(character->pos(), 150);
    auto drug = findNearestUnmountedDrug(myfish->pos(), 200);
        //auto armor = findNearestUnmountedArmor(character->pos(),300);

        if (drug != nullptr){
            myfish->pickUpDrug(drug);

        }
        if (dynamic_cast<MedBag*>(drug)){
            if(lives<MAX_LIVES){
                lives++;
                updateHealthBars();
            }
        }


}

Weapon *BattleScene::findNearestUnmountedWeapon(const QPointF &pos, qreal distance_threshold) {
    Weapon *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto weapon = dynamic_cast<Weapon *>(item)) {
            if (!weapon->getIsPicked()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = weapon;
                }
            }
        }
    }

    return nearest;
}


Drug *BattleScene::findNearestUnmountedDrug(const QPointF &pos, qreal distance_threshold) {
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

Armor *BattleScene::findNearestUnmountedArmor(const QPointF &pos, qreal distance_threshold) {
    Armor *nearest = nullptr;
    qreal minDistance = distance_threshold;

    for (QGraphicsItem *item: items()) {
        if (auto weapon = dynamic_cast<Armor *>(item)) {
            if (!weapon->getIsPicked()) {
                qreal distance = QLineF(pos, item->pos()).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    nearest = weapon;
                }
            }
        }
    }

    return nearest;
}


void BattleScene::setupHealthBars() {
    /*
    player1HealthBar = new HealthBar("Player 1");
    player1HealthBar->setMaxWidth(250);
    player1HealthBar->setPosition(20, 50, false);
    addItem(player1HealthBar);

    player2HealthBar = new HealthBar("Player 2");
    player2HealthBar->setMaxWidth(250);
    player2HealthBar->setPosition(sceneRect().width() - 20, 50, true);
    addItem(player2HealthBar);

    updateHealthBars();
    */
    
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

void BattleScene::updateHealthBars() {
    /*
    if (player1HealthBar && character) {
        player1HealthBar->setHealthPercentage(character->getHealthPercentage());
    }
    if (player2HealthBar && character2) {
        player2HealthBar->setHealthPercentage(character2->getHealthPercentage());
    }
    */
    
    if (myFishHealthBar) {
        qreal healthPercentage = static_cast<qreal>(lives) / static_cast<qreal>(MAX_LIVES);
         myFishHealthBar->setHealthValues(lives, MAX_LIVES);
    }
}

void BattleScene::updateFishCounter() {
    if (fishCounterText) {
        fishCounterText->setPlainText(QString("Fish: %1/%2").arg(fishEaten).arg(WIN_FISH_COUNT));
    }
}

void BattleScene::processCombat() {
    /*
    if (!character || !character2) return;

    if (character->getAttack() || character2->getAttack()) {
        qreal distance = QLineF(character->pos(), character2->pos()).length();

    }

    if (character->canDealDamage()) {
        checkAttackCollision(character, character2);

    }

    if (character2->canDealDamage()) {
        checkAttackCollision(character2, character);
    }
    */
}

void BattleScene::checkAttackCollision(Character* attacker, Character* target) {
    /*
    if (!attacker || !target || !attacker->isAttacking() || !target->isAlive()) {

        return;
    }

    QRectF attackBounds = attacker->getAttackBounds();

    QRectF targetBounds = target->getCollisionBounds();


    if (attackBounds.intersects(targetBounds)) {
        if(auto hasNormArmor=dynamic_cast<NormalArmor*>(target->getArmor())){

        }
        else{
        target->takeDamage(2);
        }
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        attacker->setLastAttackTime(currentTime);




        if (!target->isAlive()) {


        }
    }
    */
}

Mountable * pickup(Character *character, Mountable *mountable){
    //out of date
    return nullptr;
}

void BattleScene::processBullets() {
    /*
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Bullet*>(item)) {
            QPointF currentPos = bullet->pos();
            //bullet->applyGravity();
            QPointF velocity = bullet->getVelocity();
            currentPos += velocity * deltaTime;
            bullet->setPos(currentPos);

            if (!sceneRect().contains(currentPos)) {
                removeItem(bullet);
                bullet->deleteLater();
                continue;
            }

            checkBulletCollision(bullet);
        }
    }
    */
}

void BattleScene::checkBulletCollision(Bullet* bullet) {
    /*
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();

    if (character && character->isAlive()) {
        QRectF char1Bounds = character->getCollisionBounds();
        if (bulletBounds.intersects(char1Bounds)&&bullet->sourceID==character2->id) {
            if(auto hasBulletArmor = dynamic_cast<BulletArmor*>(character->getArmor())){
                character->takeDamage(bullet->hurt*hasBulletArmor->COEFFICIENT);
                if(!hasBulletArmor->validate()) {
                    hasBulletArmor->deleteLater();
                    character->setArmor(nullptr);
                }
            }
            else{
            character->takeDamage(bullet->hurt);
            }
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }

    if (character2 && character2->isAlive()) {
        QRectF char2Bounds = character2->getCollisionBounds();
        if (bulletBounds.intersects(char2Bounds)&&bullet->sourceID==character->id) {
            if(auto hasBulletArmor = dynamic_cast<BulletArmor*>(character2->getArmor())){
                character2->takeDamage(bullet->hurt*hasBulletArmor->COEFFICIENT);
                if(!hasBulletArmor->validate()) {
                    hasBulletArmor->deleteLater();
                    character2->setArmor(nullptr);
                }
            }
            else{
                character2->takeDamage(bullet->hurt);
            }
            //character2->takeDamage(bullet->hurt);
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }
    */
}


void BattleScene::processBalls() {
    /*
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Ball*>(item)) {
            QPointF currentPos = bullet->pos();
            bullet->applyGravity();
            QPointF velocity = bullet->getVelocity();
            currentPos += velocity * deltaTime;
            bullet->setPos(currentPos);

            if (!sceneRect().contains(currentPos)) {
                removeItem(bullet);
                bullet->deleteLater();
                continue;
            }

            checkBallCollision(bullet);
        }
    }
    */
}

void BattleScene::checkBallCollision(Ball* bullet) {
    /*
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();

    if (character && character->isAlive()) {
        QRectF char1Bounds = character->getCollisionBounds();
        if (bulletBounds.intersects(char1Bounds)&&bullet->sourceID==character2->id) {
            character->takeDamage(bullet->hurt);
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }

    if (character2 && character2->isAlive()) {
        QRectF char2Bounds = character2->getCollisionBounds();
        if (bulletBounds.intersects(char2Bounds)&&bullet->sourceID==character->id) {
            character2->takeDamage(bullet->hurt);
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }
    */
}





void BattleScene::processKnife() {
    /*
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Knife*>(item)) {
            checkKnifeCollision(bullet);
        }
    }
    */
}

void BattleScene::checkKnifeCollision(Knife* bullet) {
    /*
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();
    Character* attacker = dynamic_cast<Character*>(bullet->parentItem());
    if (!attacker) return;

    if (!attacker->isAttacking() || !attacker->canDealDamage()) {
        return;
    }
    if (character && character->isAlive()) {
        QRectF char1Bounds = character->getCollisionBounds();
        if (bulletBounds.intersects(char1Bounds)&&bullet->sourceID==character2->id) {
            if(auto hasNormArmor=dynamic_cast<NormalArmor*>(character->getArmor())){
                character->takeDamage(bullet->HURT-2);
            }
            else{
            character->takeDamage(bullet->HURT);
            }
            attacker->setLastAttackTime(QDateTime::currentMSecsSinceEpoch());

            return;
        }
    }

    if (character2 && character2->isAlive()) {
        QRectF char2Bounds = character2->getCollisionBounds();
        if (bulletBounds.intersects(char2Bounds)&&bullet->sourceID==character->id) {
            character2->takeDamage(bullet->HURT);
            attacker->setLastAttackTime(QDateTime::currentMSecsSinceEpoch());

            return;
        }
    }
    */
}

void BattleScene::createPlatforms() {
    /*
    for (Platform* platform : platforms) {
        removeItem(platform);
        delete platform;
    }
    platforms.clear();


    Platform* centerMidPlatform = new Platform(500, 450, 180, 30);
    platforms.append(centerMidPlatform);
    addItem(centerMidPlatform);

    Platform* rightHighPlatform = new Platform(900, 250, 200, 30);
    platforms.append(rightHighPlatform);
    addItem(rightHighPlatform);

    Platform* leftTopPlatform = new Platform(100, 300, 150, 25);
    platforms.append(leftTopPlatform);
    addItem(leftTopPlatform);



    for (Platform* platform : platforms) {
        platform->setPlatformStyle(
            QBrush(QColor(101, 67, 33)),
            QPen(QColor(139, 69, 19), 3)
            );
    }
    */
}



qreal BattleScene::findGroundLevel(const QPointF& position, qreal defaultGround) {
    /*
    qreal groundLevel = defaultGround;

    QRectF testBounds(position.x() - 25, position.y(), 50, 10);

    for (Platform* platform : platforms) {
        if (platform->canStandOn(testBounds)) {
            qreal platformTop = platform->getTopY();
            if (platformTop < groundLevel) {
                groundLevel = platformTop;
            }
        }
    }

    return groundLevel;
    */
    return defaultGround;
}

void BattleScene::checkGameOver() {
    /*
    if (gameEnded) return;

    QString winner = "";

    if (character && !character->isAlive()) {
        winner = "Player 2";
    } else if (character2 && !character2->isAlive()) {
        winner = "Player 1";
    }

    if (!winner.isEmpty()) {
        gameEnded = true;


        Scene::stopAllTimers();
        if (fallingController) {
            fallingController->stopAllTimers();

        }
        QTimer::singleShot(100, [this, winner]() {
            emit gameOver(winner);
        });
    }
    */
    
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
            emit gameOver("Game Over");
        });
    }
}

void BattleScene::checkVictory() {
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
            emit gameOver("Victory");
        });
    }
}

void BattleScene::takeDamage() {
    if (gameEnded) return;
    
    lives--;
    updateHealthBars();
    isInvincible = true;
    lastDamageTime = QDateTime::currentMSecsSinceEpoch();
    if (lives <= 0) {
        checkGameOver();
    }
}

void BattleScene::checkGrassVisibility() {
    /*
    if (character) {
        bool inGrass = isCharacterInAnyGrass(character);
        character->setInStealth(inGrass);
    }

    if (character2) {
        bool inGrass = isCharacterInAnyGrass(character2);
        character2->setInStealth(inGrass);
    }
    */
}

bool BattleScene::isCharacterInAnyGrass(Character* character) {
    /*
    if (!character) return false;

    QPointF charPos = character->pos();
    for (Grass* grass : grassList) {
        if (grass->isCharacterInGrass(character)) {
            return true;
        }
    }
    */
    return false;
}


void BattleScene::checkIceSpeedUp() {
    /*
    if (character) {
        bool inIce = isCharacterInAnyIce(character);
        character->setInIce(inIce);
    }

    if (character2) {
        bool inIce = isCharacterInAnyIce(character2);
        character2->setInIce(inIce);
    }
    */
}

bool BattleScene::isCharacterInAnyIce(Character* character) {
    /*
    if (!character) return false;

    QPointF charPos = character->pos();
    for (Ice* ice : iceList) {
        if (ice->isCharacterInIce(character)) {
            return true;
        }
    }
    */
    return false;
}

void BattleScene::checkFishCollision() {
    //if (gameEnded) return;
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

    if (!myfish || !fishGenerator) return;

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
            qDebug() << "MyFish was eaten by Shark!";
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
            qDebug() << "MyFish was eaten by Midfish!";
            takeDamage();
            playEatSound();


            showScorePopup(midfishEnemy->pos(), "-1");
            return;
        }
    }
}

void BattleScene::processCharacterMovement(Character* character) {
}

QPointF BattleScene::handleVerticalCollision(Character* character, QPointF newPos, QPointF velocity) {
    return newPos;
}

Platform* BattleScene::findCollidingPlatform(const QPointF& currentPos, const QPointF& newPos, const QRectF& charBounds, const QPointF& velocity) {
    return nullptr;
}

Platform* BattleScene::findSupportingPlatform(const QRectF& characterBounds, qreal velocityY) {
    return nullptr;
}
void BattleScene::playEatSound() {
    // 停止当前播放（如果正在播放）
    if (eatSoundPlayer->playbackState() == QMediaPlayer::PlayingState) {
        eatSoundPlayer->stop();
    }

    // 重置到开始位置
    eatSoundPlayer->setPosition(0);

    // 播放音效
    eatSoundPlayer->play();
}
void BattleScene::showScorePopup(const QPointF& position, QString score) {
    // 创建文字项
    QGraphicsTextItem* scoreText = new QGraphicsTextItem();
    scoreText->setPlainText(score);

    // 设置字体样式
    QFont font("Arial", 24, QFont::Bold);
    scoreText->setFont(font);
    scoreText->setDefaultTextColor(QColor(255, 50, 50)); // 红色

    // 添加文字描边效果（可选）
    scoreText->setHtml(QString(
                           "<span style='color: #ff3232; "
                           "text-shadow: -2px -2px 0 #000, 2px -2px 0 #000, "
                           "-2px 2px 0 #000, 2px 2px 0 #000;'>"
                           "%1</span>"
                           ).arg(score));

    // 设置初始位置（在鱼的位置上方一点）
    scoreText->setPos(position.x(), position.y() - 50);
    scoreText->setZValue(1000); // 确保在最上层显示

    // 添加到场景
    addItem(scoreText);

    //  创建向上漂浮的动画
    QPropertyAnimation* moveAnimation = new QPropertyAnimation(scoreText, "pos");
    moveAnimation->setDuration(1000); // 1秒
    moveAnimation->setStartValue(scoreText->pos());
    moveAnimation->setEndValue(scoreText->pos() + QPointF(0, -80)); // 向上移动80像素
    moveAnimation->setEasingCurve(QEasingCurve::OutQuad);

    //  创建淡出动画
    QPropertyAnimation* fadeAnimation = new QPropertyAnimation(scoreText, "opacity");
    fadeAnimation->setDuration(1000);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    fadeAnimation->setEasingCurve(QEasingCurve::InQuad);

    //动画结束后删除文字
    connect(moveAnimation, &QPropertyAnimation::finished, [this, scoreText]() {
        removeItem(scoreText);
        delete scoreText;
    });

    // 启动动画
    moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
