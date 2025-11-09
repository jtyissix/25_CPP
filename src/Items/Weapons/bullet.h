#ifndef BULLET_H
#define BULLET_H

#include "../Item.h"
#include "../Mountable.h"
class Bullet : public Item,public Mountable
{
public:
    //Bullet();
    explicit Bullet(QGraphicsItem *parent);
    void mountToParent(double x,double y) override;
    void setScene(QGraphicsScene* scene){parentScene=scene;}
    void setVelocity(const QPointF& vel);
    QPointF getVelocity() const;
    QRectF getCollisionBounds() const { return sceneBoundingRect(); }

    int hurt;
    int sourceID;
protected:
    QGraphicsScene* parentScene;
    QPointF velocity;

};

#endif // BULLET_H=
