#ifndef SHARK_H
#define SHARK_H

#include "Character.h"

#include "Character.h"
#include <QPixmap>
class shark : public Character
{
public:
    explicit shark(QGraphicsItem *parent = nullptr);
    virtual void processDirection() override;
    virtual void pickUpWeapon(Weapon* weaponToPick) override;
    virtual void pickUpArmor(Armor* newArmor) override;
    // Shark 特有功能
    void initializeAsEnemy(const QRectF& sceneRect);  // 初始化为敌人
    void updateMovement(qint64 deltaTime);             // 更新移动
    bool isOutOfBounds(const QRectF& sceneRect) const; // 检查是否越界

    qreal getSpeed() const { return moveSpeed; }
    void setSpeed(qreal speed) { moveSpeed = speed; }
private:
    void loadSprites();
    QPixmap extractFrame(const QPixmap& spriteSheet, int row, int col, int frameWidth, int frameHeight);
    qreal moveSpeed;      // 移动速度（水平）
    bool isEnemy;         // 是否为敌人（自动移动）
    QPixmap leftSprite;   // 朝左的图片
    QPixmap rightSprite;  // 朝右的图片
};


#endif // SHARK_H
