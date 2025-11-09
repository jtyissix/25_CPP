#ifndef DRUG_H
#define DRUG_H

#include "../Item.h"
#include "../Mountable.h"
#include "../Fallable.h"

class Drug: public Item, public Mountable {
    //Q_OBJECT
public:
    explicit Drug(QGraphicsItem *parent, const QString &pixmapPath);

    void mountToParent(double x,double y) override;

    virtual void effect();

    [[nodiscard]] bool getIsFalling();

    void setIsFalling(bool isFalling);

    [[nodiscard]] bool getIsOnGround();

    void setIsOnGround(bool isOnGround);

    [[nodiscard]] bool getIsPicked();

    void setIsPicked(bool isPicked);

    void applyGravity();

    [[nodiscard]] qint64 getCreateTime() const { return createTime; }

    void setCreateTime(qint64 time) { createTime = time; }

    QPointF getFallingVelocity() const { return fallingVelocity; }

    void setFallingVelocity(const QPointF& velocity) { fallingVelocity = velocity; }

    void setScene(QGraphicsScene* scene){parentScene=scene;}

    virtual void setBullet();


    virtual void setBulletCharacter(){}

    virtual void reset(){}
protected:
    QGraphicsScene* parentScene;
private:

    QPointF fallingVelocity{QPointF(0, 0)};

    bool isFalling{true};

    bool isOnGround{false};

    bool isPicked{false};

    qint64 createTime{0}; // 添加创建时间成员变量

    static constexpr qreal DRUG_GRAVITY=1;
};
#endif // DRUG_H
