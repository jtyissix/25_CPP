#ifndef MIDFISH_H
#define MIDFISH_H

#include "Character.h"
#include <QPixmap>

class midfish : public Character
{
public:
    explicit midfish(QGraphicsItem *parent = nullptr);
    virtual void processDirection() override;
    virtual void pickUpWeapon(Weapon* weaponToPick) override;
    virtual void pickUpArmor(Armor* newArmor) override;
    
    void initializeAsEnemy(const QRectF& sceneRect, const QPointF& targetPos);
    void updateMovement(qint64 deltaTime);
    bool isOutOfBounds(const QRectF& sceneRect) const;
    
    qreal getSpeed() const { return moveSpeed; }
    void setSpeed(qreal speed) { moveSpeed = speed; }
    
private:
    void loadSprites();
    QPixmap extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight);
    
    qreal moveSpeed;
    bool isEnemy;
    QPixmap leftSprite;
    QPixmap rightSprite;
};

#endif
