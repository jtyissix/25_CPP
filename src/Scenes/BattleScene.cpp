//
// Created by gerw on 8/20/24.
//


#include <QDateTime>
#include "BattleScene.h"
#include "../Items/Characters/myfish.h"
#include "../Items/Characters/Link.h"
#include "../Items/Maps/Battlefield.h"
#include "../Items/Characters/link2.h"
#include "../FallingController/fallingcontroller.h"
#include "../Items/Armors/NormalArmor.h"
#include "../Items/Armors/BulletArmor.h"
#include "../Items/Maps/grass.h"
#include "../Items/Maps/ice.h"
#include <QTimer>
BattleScene::BattleScene(QObject *parent) : Scene(parent)
    /*, player1HealthBar(nullptr)
    , player2HealthBar(nullptr)*/
    , gameEnded(false){
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
    //setupHealthBars();
    //createPlatforms();
    //setup falling controller
    fallingController = new FallingController();
    fallingController->setScene(this);
    fallingController->setFallingParams(5000);
    fallingController->setVanishParams(1000);
    fallingController->setFallingRange(map->sceneBoundingRect());
    fallingController->setGroundLevel(map->getFloorHeight()-60);//considering picture height
    fallingController->startLoop();
}
void BattleScene::createGrass(){
    // 清理旧的草地列表
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
    // 清理旧的草地列表
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
        // 处理跳跃逻辑
        //character->processJump();

        // 应用重力
        //character->applyGravity();
    }
    */
}

void BattleScene::update() {
    if (gameEnded) {
        return;
    }
    /*
    if (!character || !character2) {

        return;
    }*/
    Scene::update();
    // 处理战斗逻辑
    /*
    processCombat();
    // 更新血条显示
    updateHealthBars();

    checkGrassVisibility();

    checkIceSpeedUp();

    processBullets();

    processBalls();

    processKnife();

    checkGameOver();
    // 处理物理更新
    //processPhysics();
*/
}

void BattleScene::processMovement() {
    Scene::processMovement();
    if (myfish != nullptr) {
        QPointF oldPos = myfish->pos();

        // 应用速度
        QPointF newPos = myfish->pos() + myfish->getVelocity() * (double) deltaTime;
        // 边界检测 - 防止角色移出场景
        if (newPos.x() < map->boundingRect().left()) {
            newPos.setX( map->boundingRect().left());
            // 停止水平移动
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        } else if (newPos.x()+64 > map->boundingRect().right()) {
            newPos.setX(map->boundingRect().right()-64);
            // 停止水平移动
            QPointF vel = myfish->getVelocity();
            vel.setX(0);
            myfish->setVelocity(vel);
        }

        // 地面检测
        int fishHeight=64;
        if (newPos.y() < map->boundingRect().top()) {
            newPos.setY(map->boundingRect().top());
            QPointF vel = myfish->getVelocity();
            vel.setY(0);
            myfish->setVelocity(vel);
        }
        // 下边界检测 - 让鱼的底部贴边，而不是锚点贴边
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

        // 应用速度
        QPointF newPos = character2->pos() + character2->getVelocity() * (double) deltaTime;
        // 边界检测 - 防止角色移出场景
        if (newPos.x() < map->boundingRect().left()) {
            newPos.setX( map->boundingRect().left());
            // 停止水平移动
            QPointF vel = character->getVelocity();
            vel.setX(0);
            character2->setVelocity(vel);
        } else if (newPos.x() > map->boundingRect().right()) {
            newPos.setX(map->boundingRect().right());
            // 停止水平移动
            QPointF vel = character->getVelocity();
            vel.setX(0);
            character2->setVelocity(vel);
        }

        // 地面检测

        qreal groundLevel = map->getFloorHeight();

        if (newPos.y() >= groundLevel) {
            newPos.setY(groundLevel);
            character2->setIsOnGround(true);
            // 如果着地，停止垂直移动
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
// 角色移动处理
void BattleScene::processCharacterMovement(Character* character) {
    if (!character) return;

    QPointF currentPos = character->pos();
    QPointF velocity = character->getVelocity();

    // 计算预期的新位置
    QPointF newPos = currentPos + velocity * (double) deltaTime;

    // 边界检测 - 防止角色移出场景
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

    // 垂直碰撞检测和位置调整
    newPos = handleVerticalCollision(character, newPos, velocity);

    // 设置最终位置
    character->setPos(newPos);
}

// 处理垂直方向的碰撞检测（平台和地面）
QPointF BattleScene::handleVerticalCollision(Character* character, QPointF newPos, QPointF velocity) {
    // 获取角色的完整碰撞框
    QRectF charBounds = character->getCollisionBounds();
    qreal charWidth = charBounds.width();
    qreal charHeight = charBounds.height();

    // 创建基于新位置的角色边界框
    QRectF newCharBounds(newPos.x() - charWidth/2, newPos.y() - charHeight, charWidth, charHeight);

    // 地面高度
    qreal groundLevel = map->getFloorHeight();

    // 检查平台碰撞
    Platform* collidingPlatform = findCollidingPlatform(character->pos(), newPos, newCharBounds, velocity);

    if (collidingPlatform) {
        QRectF platformRect = collidingPlatform->getCollisionRect();

        if (velocity.y() > 0) {
            // 向下移动，脚部撞到平台顶部 - 站在平台上
            qreal platformTop = platformRect.top();
            newPos.setY(platformTop); // 角色脚部对齐到平台顶部
            character->setIsOnGround(true);
            velocity.setY(0);
            character->setVelocity(velocity);


        } else if (velocity.y() < 0) {
            // 向上移动，头部撞到平台底部 - 停止向上移动
            qreal platformBottom = platformRect.bottom();
            // 关键修复：角色头部（顶部）撞到平台底部
            newPos.setY(platformBottom + charHeight); // 角色头部紧贴平台底部
            velocity.setY(0);
            character->setVelocity(velocity);

        }
    } else if (newPos.y() >= groundLevel) {
        // 没有平台碰撞，检查地面
        newPos.setY(groundLevel);
        character->setIsOnGround(true);
        velocity.setY(0);
        character->setVelocity(velocity);

    } else {
        // 在空中
        character->setIsOnGround(false);
    }

    return newPos;
}


Platform* BattleScene::findCollidingPlatform(const QPointF& currentPos, const QPointF& newPos, const QRectF& charBounds, const QPointF& velocity) {
    for (Platform* platform : platforms) {
        QRectF platformRect = platform->getCollisionRect();

        // 检查水平重叠
        bool horizontalOverlap = !(charBounds.right() < platformRect.left() ||
                                   charBounds.left() > platformRect.right());

        if (!horizontalOverlap) continue;

        if (velocity.y() > 0) {
            // 向下移动：检查脚部是否撞到平台顶部
            qreal charFootCurrent = currentPos.y();  // 角色当前脚部位置
            qreal charFootNew = newPos.y();          // 角色新的脚部位置
            qreal platformTop = platformRect.top();

            // 检查是否从平台上方穿过平台顶部
            bool wasAbove = charFootCurrent <= platformTop;
            bool willBeBelow = charFootNew >= platformTop;

            if (wasAbove && willBeBelow) {

                return platform;
            }

        } else if (velocity.y() < 0) {
            // 向上移动：检查头部是否撞到平台底部
            QRectF currentCharBounds = character->getCollisionBounds();
            qreal charHeadCurrent = currentCharBounds.top();    // 角色当前头部位置
            qreal charHeadNew = charBounds.top();               // 角色新的头部位置
            qreal platformBottom = platformRect.bottom();

            qreal distance = platformBottom - charHeadNew;

            // 只有距离很近(比如5像素内)才算碰撞
            if (distance >= 0 && distance <= 50 && velocity.y() < 0) {

                return platform;
            }
        }
    }

    return nullptr;
}

// 修复：更准确的支撑平台查找
Platform* BattleScene::findSupportingPlatform(const QRectF& characterBounds, qreal velocityY) {
    for (Platform* platform : platforms) {
        QRectF platformRect = platform->getCollisionRect();

        // 检查水平重叠
        bool horizontalOverlap = !(characterBounds.right() < platformRect.left() ||
                                   characterBounds.left() > platformRect.right());

        if (horizontalOverlap) {
            // 检查垂直位置关系
            qreal charBottom = characterBounds.bottom();
            qreal platformTop = platformRect.top();

            // 角色脚部在平台顶部附近（容差5像素）
            if (abs(charBottom - platformTop) <= 2) {
                return platform;
            }
        }
    }

    return nullptr;
}
void BattleScene::processPicking() {
    Scene::processPicking();
    if (character->isPicking()) {
        auto weapon = findNearestUnmountedWeapon(character->pos(), 150);
        auto drug = findNearestUnmountedDrug(character->pos(), 500);
        auto armor = findNearestUnmountedArmor(character->pos(),300);
        if (weapon != nullptr) {
            character->pickUpWeapon(weapon);//spareArmor = dynamic_cast<Armor *>(pickup(character, mountable));
        }
        if (drug != nullptr){
            character->pickUpDrug(drug);

        }
        if (armor != nullptr){
            character->pickUpArmor(armor);
        }
    }
    if (character2->isPicking()) {
        auto weapon = findNearestUnmountedWeapon(character2->pos(), 150);
        auto drug = findNearestUnmountedDrug(character2->pos(), 500);
        auto armor = findNearestUnmountedArmor(character2->pos(),300);
        if (weapon != nullptr) {
            character2->pickUpWeapon(weapon);//spareArmor = dynamic_cast<Armor *>(pickup(character, mountable));
        }
        if (drug != nullptr){
            character2->pickUpDrug(drug);
        }
        if (armor != nullptr){
            character2->pickUpArmor(armor);
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
    // 创建玩家1血条（左上角）
    player1HealthBar = new HealthBar("Player 1");
    player1HealthBar->setMaxWidth(250);
    player1HealthBar->setPosition(20, 50, false); // 左对齐
    addItem(player1HealthBar);

    // 创建玩家2血条（右上角）
    player2HealthBar = new HealthBar("Player 2");
    player2HealthBar->setMaxWidth(250);
    player2HealthBar->setPosition(sceneRect().width() - 20, 50, true); // 右对齐
    addItem(player2HealthBar);

    // 初始化血条显示
    updateHealthBars();
}

void BattleScene::updateHealthBars() {
    if (player1HealthBar && character) {
        player1HealthBar->setHealthPercentage(character->getHealthPercentage());
    }
    if (player2HealthBar && character2) {
        player2HealthBar->setHealthPercentage(character2->getHealthPercentage());
    }
}

void BattleScene::processCombat() {
    if (!character || !character2) return;

    if (character->getAttack() || character2->getAttack()) {
        qreal distance = QLineF(character->pos(), character2->pos()).length();

    }

    // 检查角色1攻击角色2
    if (character->canDealDamage()) {
        checkAttackCollision(character, character2);

    }

    // 检查角色2攻击角色1
    if (character2->canDealDamage()) {
        checkAttackCollision(character2, character);
    }
}

void BattleScene::checkAttackCollision(Character* attacker, Character* target) {
    if (!attacker || !target || !attacker->isAttacking() || !target->isAlive()) {

        return;
    }

    // 获取攻击者的攻击范围
    QRectF attackBounds = attacker->getAttackBounds();

    // 获取目标的碰撞框
    QRectF targetBounds = target->getCollisionBounds();


    // 检查碰撞
    if (attackBounds.intersects(targetBounds)) {
        // 造成伤害
        if(auto hasNormArmor=dynamic_cast<NormalArmor*>(target->getArmor())){

        }
        else{
        target->takeDamage(2); // 拳头伤害为2
        }
        // 更新攻击者的最后攻击时间，防止连续伤害
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        attacker->setLastAttackTime(currentTime);




        // 如果目标死亡
        if (!target->isAlive()) {


        }
    }
}

Mountable * pickup(Character *character, Mountable *mountable){
    //out of date
}

// 添加子弹处理方法
void BattleScene::processBullets() {
    // 获取场景中所有的子弹并更新它们
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Bullet*>(item)) {
            // 更新子弹位置
            QPointF currentPos = bullet->pos();
            //bullet->applyGravity();
            QPointF velocity = bullet->getVelocity();
            currentPos += velocity * deltaTime;
            bullet->setPos(currentPos);

            // 检查是否超出屏幕边界
            if (!sceneRect().contains(currentPos)) {
                removeItem(bullet);
                bullet->deleteLater();
                continue;
            }

            // 检查子弹与角色的碰撞
            checkBulletCollision(bullet);
        }
    }
}

void BattleScene::checkBulletCollision(Bullet* bullet) {
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();

    // 检查与角色1的碰撞
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
            }                // 子弹伤害
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }

    // 检查与角色2的碰撞
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
            //character2->takeDamage(bullet->hurt); // 子弹伤害
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }
}


void BattleScene::processBalls() {
    // 获取场景中所有的子弹并更新它们
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Ball*>(item)) {
            // 更新子弹位置
            QPointF currentPos = bullet->pos();
            bullet->applyGravity();
            QPointF velocity = bullet->getVelocity();
            currentPos += velocity * deltaTime;
            bullet->setPos(currentPos);

            // 检查是否超出屏幕边界
            if (!sceneRect().contains(currentPos)) {
                removeItem(bullet);
                bullet->deleteLater();
                continue;
            }

            // 检查子弹与角色的碰撞
            checkBallCollision(bullet);
        }
    }
}

void BattleScene::checkBallCollision(Ball* bullet) {
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();

    // 检查与角色1的碰撞
    if (character && character->isAlive()) {
        QRectF char1Bounds = character->getCollisionBounds();
        if (bulletBounds.intersects(char1Bounds)&&bullet->sourceID==character2->id) {
            character->takeDamage(bullet->hurt); // 子弹伤害
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }

    // 检查与角色2的碰撞
    if (character2 && character2->isAlive()) {
        QRectF char2Bounds = character2->getCollisionBounds();
        if (bulletBounds.intersects(char2Bounds)&&bullet->sourceID==character->id) {
            character2->takeDamage(bullet->hurt); // 子弹伤害
            removeItem(bullet);
            bullet->deleteLater();

            return;
        }
    }
}





void BattleScene::processKnife() {
    // 获取场景中所有的子弹并更新它们
    QList<QGraphicsItem*> itemList = items();
    for (QGraphicsItem* item : itemList) {
        if (auto bullet = dynamic_cast<Knife*>(item)) {
            // 检查与角色的碰撞
            checkKnifeCollision(bullet);
        }
    }
}

void BattleScene::checkKnifeCollision(Knife* bullet) {
    if (!bullet) return;

    QRectF bulletBounds = bullet->sceneBoundingRect();
    Character* attacker = dynamic_cast<Character*>(bullet->parentItem());
    if (!attacker) return;

    // 只有在角色攻击状态且可以造成伤害时才检查碰撞
    if (!attacker->isAttacking() || !attacker->canDealDamage()) {
        return;
    }
    // 检查与角色1的碰撞
    if (character && character->isAlive()) {
        QRectF char1Bounds = character->getCollisionBounds();
        if (bulletBounds.intersects(char1Bounds)&&bullet->sourceID==character2->id) {
            if(auto hasNormArmor=dynamic_cast<NormalArmor*>(character->getArmor())){
                character->takeDamage(bullet->HURT-2);
            }
            else{
            character->takeDamage(bullet->HURT); // 子弹伤害
            }
            attacker->setLastAttackTime(QDateTime::currentMSecsSinceEpoch());

            return;
        }
    }

    // 检查与角色2的碰撞
    if (character2 && character2->isAlive()) {
        QRectF char2Bounds = character2->getCollisionBounds();
        if (bulletBounds.intersects(char2Bounds)&&bullet->sourceID==character->id) {
            character2->takeDamage(bullet->HURT); // 子弹伤害
            attacker->setLastAttackTime(QDateTime::currentMSecsSinceEpoch());

            return;
        }
    }
}

//创建多个平台
void BattleScene::createPlatforms() {
    // 清除已有平台
    for (Platform* platform : platforms) {
        removeItem(platform);
        delete platform;
    }
    platforms.clear();

    // 创建不同高度的平台


    // 中央中等高度平台
    Platform* centerMidPlatform = new Platform(500, 450, 180, 30);
    platforms.append(centerMidPlatform);
    addItem(centerMidPlatform);

    // 右侧高平台
    Platform* rightHighPlatform = new Platform(900, 250, 200, 30);
    platforms.append(rightHighPlatform);
    addItem(rightHighPlatform);

    // 左上角小平台
    Platform* leftTopPlatform = new Platform(100, 300, 150, 25);
    platforms.append(leftTopPlatform);
    addItem(leftTopPlatform);



    // 设置平台样式
    for (Platform* platform : platforms) {
        platform->setPlatformStyle(
            QBrush(QColor(101, 67, 33)),
            QPen(QColor(139, 69, 19), 3)
            );
    }
}



//查找地面高度（包括平台）
qreal BattleScene::findGroundLevel(const QPointF& position, qreal defaultGround) {
    qreal groundLevel = defaultGround;

    // 创建角色边界框用于检测
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
}

void BattleScene::checkGameOver() {
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
        // 延迟发送信号，避免在update循环中直接处理
        QTimer::singleShot(100, [this, winner]() {
            emit gameOver(winner);
        });
    }
}


void BattleScene::checkGrassVisibility() {
    if (character) {
        bool inGrass = isCharacterInAnyGrass(character);
        character->setInStealth(inGrass);
    }

    if (character2) {
        bool inGrass = isCharacterInAnyGrass(character2);
        character2->setInStealth(inGrass);
    }
}

bool BattleScene::isCharacterInAnyGrass(Character* character) {
    if (!character) return false;

    QPointF charPos = character->pos();
    for (Grass* grass : grassList) {
        if (grass->isCharacterInGrass(character)) {
            return true;
        }
    }
    return false;
}


void BattleScene::checkIceSpeedUp() {
    if (character) {
        bool inIce = isCharacterInAnyIce(character);
        character->setInIce(inIce);
    }

    if (character2) {
        bool inIce = isCharacterInAnyIce(character2);
        character2->setInIce(inIce);
    }
}

bool BattleScene::isCharacterInAnyIce(Character* character) {
    if (!character) return false;

    QPointF charPos = character->pos();
    for (Ice* ice : iceList) {
        if (ice->isCharacterInIce(character)) {
            return true;
        }
    }
    return false;
}

*/
