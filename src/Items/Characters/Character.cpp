//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include "Character.h"
#include <QDateTime>
#include <algorithm>
#include <QGraphicsScene>
Character::Character(QGraphicsItem *parent) : Item(parent, ""), health(100)
    , maxHealth(100)
    , lastAttackTime(0) {
//    ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
//    // Optionally, set some properties of the ellipse
//    ellipseItem->setBrush(Qt::green);          // Fill color
//    ellipseItem->setZValue(1);
}

bool Character::getAttack() const {
    return isAttack;
}

void Character::setAttack(bool isattack) {
    Character::isAttack=isattack;
}
bool Character::getDirection() const {
    return facingRight;
}

void Character::setDirection(bool direction) {
    Character::facingRight=direction;
}

bool Character::isKneeDown() const {
    return kneeDown;
}

void Character::setKneeDown(bool kneeDown) {
    Character::kneeDown = kneeDown;
}

bool Character::isLeftDown() const {
    return leftDown;
}

void Character::setLeftDown(bool leftDown) {
    Character::leftDown = leftDown;
}

bool Character::isRightDown() const {
    return rightDown;
}

void Character::setRightDown(bool rightDown) {
    Character::rightDown = rightDown;
}

bool Character::isPickDown() const {
    return pickDown;
}

void Character::setPickDown(bool pickDown) {
    Character::pickDown = pickDown;
}

const QPointF &Character::getVelocity() const {
    return velocity;
}

void Character::setVelocity(const QPointF &velocity) {
    Character::velocity = velocity;
}

bool Character::isJumpDown() const {
    return jumpDown;
}

void Character::setJumpDown(bool jumpDown) {
    Character::jumpDown = jumpDown;
}

bool Character::getIsJumping() const {
    return isJumping;
}

bool Character::getIsOnGround() const {
    return isOnGround;
}

void Character::setIsOnGround(bool onGround) {
    Character::isOnGround = onGround;
    if (onGround) {
        isJumping = false;
    }
}

void Character::processJump() {
    // 检测跳跃按键按下（边缘检测）
    if ( jumpDown && isOnGround) {
        // 开始跳跃
        isJumping = true;
        isOnGround = false;
        auto currentVelocity = getVelocity();
        currentVelocity.setY(JUMP_SPEED);  // 设置向上的初始速度
        setVelocity(currentVelocity);
    }
    lastJumpDown = jumpDown;
}

void Character::applyGravity() {
    if (!isOnGround) {
        auto currentVelocity = getVelocity();
        // 应用重力
        currentVelocity.setY(currentVelocity.y() + GRAVITY);

        // 限制最大下落速度
        if (currentVelocity.y() > TERMINAL_VELOCITY) {
            currentVelocity.setY(TERMINAL_VELOCITY);
        }

        setVelocity(currentVelocity);
    }
}
//no use
void Character::processAttack(){
    if(isAttack){
    if(weapon==nullptr){
        figure->setVisible(false);
        figureKneeDown->setVisible(false);
        figureAttack->setVisible(true);
    }
    }
    else{
        if(weapon==nullptr){
            figure->setVisible(true);
            figureKneeDown->setVisible(false);
            figureAttack->setVisible(false);
        }
    }
}
void Character::processInput() {
    if (getDirection() != lastDirection) {
        processDirection();
        lastDirection = getDirection();
    }

    if(isKneeDown()){
        setJumpDown(false);
    }
    //processAttack();
    processJump();
    applyGravity();
    auto velocity = getVelocity();
    double moveSpeed = 0.3;
    if(getIsPill()){
        qint64 currentTime=QDateTime::currentMSecsSinceEpoch();
        if((currentTime-lastPillTime)>PILL_TIME){
            setPill(false);
            lastPillTime=currentTime;
        }
        else{
            moveSpeed=(double)PILL_SPEED;
            if((currentTime-lastPillCureTime)>500){
                lastPillCureTime=currentTime;
                takeDamage(-1);
            }

        }
    }
    if(isInIce()){
        moveSpeed+=ICE_SPEEDUP;
    }
    velocity.setX(0);
    if (isLeftDown()) {
        velocity.setX(- moveSpeed);

    }

    if (isRightDown()) {
        velocity.setX(+ moveSpeed);
    }



    if (!lastPickDown && pickDown) { // first time pickDown
        picking = true;
    } else {
        picking = false;
    }
    lastPickDown = pickDown;
    //handle it at last
    /*
    if (isAttack && weapon == nullptr) {
        figure->setVisible(false);
        figureKneeDown->setVisible(false);
        figureAttack->setVisible(true);
        // 攻击时可能不允许移动
        // velocity.setX(0);
    }
    else if(isAttack && weapon != nullptr){
        weapon->attack();
    }
    // 然后处理蹲下状态
    else if (isKneeDown()) {
        figure->setVisible(false);
        figureKneeDown->setVisible(true);
        figureAttack->setVisible(false);
        velocity.setX(0);
    }
    // 最后是正常状态
    else {
        figure->setVisible(true);
        figureKneeDown->setVisible(false);
        figureAttack->setVisible(false);
        if (weapon!=nullptr){
            weapon->reset();
        }
    }
    */
    setVelocity(velocity);
    updateVisibility();

}

bool Character::isPicking() const {
    return picking;
}



// 生命值系统实现
int Character::getHealth() const {
    return health;
}

int Character::getMaxHealth() const {
    return maxHealth;
}

qreal Character::getHealthPercentage() const {
    return static_cast<qreal>(health) / static_cast<qreal>(maxHealth);
}

void Character::setHealth(int health) {
    this->health = std::clamp(health, 0, maxHealth);
}

void Character::setMaxHealth(int maxHealth) {
    this->maxHealth = std::max(1, maxHealth);
    this->health = std::min(this->health, this->maxHealth);
}

void Character::takeDamage(int damage) {
    int oldHealth = health;
    health = std::max(0, health - damage);

}

void Character::heal(int amount) {
    health = std::min(maxHealth, health + amount);
}

bool Character::isAlive() const {
    return health > 0;
}

// 攻击系统实现
QRectF Character::getAttackBounds() const {
    if (!isAttacking()) {
        return QRectF();
    }

    // 计算攻击范围矩形
    QRectF bounds = boundingRect();
    qreal attackWidth = ATTACK_RANGE;
    qreal attackHeight = this->figure->boundingRect().height() * 0.9; // 攻击高度为角色高度的60%

    QRectF attackRect;
    if (getDirection()) { // 面向右
        attackRect = QRectF(bounds.right(), bounds.center().y() - attackHeight/2,
                            attackWidth, attackHeight);
    } else { // 面向左
        attackRect = QRectF(bounds.left() - attackWidth, bounds.center().y() - attackHeight/2,
                            attackWidth, attackHeight);
    }

    // 转换到场景坐标
    return mapRectToScene(attackRect);
}

bool Character::isAttacking() const {
    bool result = isAttack && figureAttack && figureAttack->isVisible();
    if (isAttack) {

    }
    return isAttack && figureAttack && figureAttack->isVisible();
}

bool Character::canDealDamage() const {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    return isAttacking() && (currentTime - lastAttackTime) >= ATTACK_COOLDOWN;
}

void Character::setLastAttackTime(qint64 time) {
    lastAttackTime = time;
}

qint64 Character::getLastAttackTime() const {
    return lastAttackTime;
}

QRectF Character::getCollisionBounds() const {
    return this->figure->sceneBoundingRect();
}

void Character::pickUpWeapon(Weapon* weaponToPick){

}

void Character::pickUpDrug(Drug* drugToPick){

    drugToPick->setParentItem(this);
    drugToPick->effect();
    drugToPick->setVisible(false);
    this->scene()->removeItem(drugToPick);
    // let falling controller to manage it
}
void Character::updateVisibility() {
    if (inStealth && isKneeDown()) {
        if(weapon!=nullptr){
            weapon->setVisible(false);
        }
        if(armor!=nullptr){
            armor->setVisible(false);
        }
        // 隐身状态：角色不可见但保持其他功能
        if (isAttack && weapon == nullptr) {
            figure->setVisible(false);
            figureKneeDown->setVisible(false);  // 隐身时蹲下形象也不可见
            figureAttack->setVisible(false);    // 攻击动作也不可见，但攻击判定仍然有效
        } else if (isAttack && weapon != nullptr) {
            figure->setVisible(false);
            figureKneeDown->setVisible(false);
            figureAttack->setVisible(false);
            weapon->attack();  // 武器攻击仍然有效
        } else if (isKneeDown()) {
            figure->setVisible(false);
            figureKneeDown->setVisible(false);  // 隐身
            figureAttack->setVisible(false);
        } else {
            // 不在蹲下状态，取消隐身
            figure->setVisible(true);
            figureKneeDown->setVisible(false);
            figureAttack->setVisible(false);
            if (weapon != nullptr) {
                weapon->reset();
            }
        }
    } else {
        if(weapon!=nullptr){
            weapon->setVisible(true);
        }
        if(armor!=nullptr){
            armor->setVisible(true);
        }
        if (isAttack && weapon == nullptr) {
            figure->setVisible(false);
            figureKneeDown->setVisible(false);
            figureAttack->setVisible(true);
            // 攻击时可能不允许移动
            // velocity.setX(0);
        }
        else if(isAttack && weapon != nullptr){
            weapon->attack();
        }
        // 然后处理蹲下状态
        else if (isKneeDown()) {
            figure->setVisible(false);
            figureKneeDown->setVisible(true);
            figureAttack->setVisible(false);
            velocity.setX(0);
        }
        // 最后是正常状态
        else {
            figure->setVisible(true);
            figureKneeDown->setVisible(false);
            figureAttack->setVisible(false);
            if (weapon!=nullptr){
                weapon->reset();
            }
        }
    }
}
