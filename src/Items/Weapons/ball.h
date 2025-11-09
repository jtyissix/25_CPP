#ifndef BALL_H
#define BALL_H
#include "../Item.h"
#include "../Mountable.h"
class Ball : public Item,public Mountable
{
public:
    //Bullet();
    explicit Ball(QGraphicsItem *parent);
    void mountToParent(double x,double y) override;
    void setScene(QGraphicsScene* scene){parentScene=scene;}
    void setVelocity(const QPointF& vel);
    QPointF getVelocity() const;
    QRectF getCollisionBounds() const { return sceneBoundingRect(); }
    void applyGravity();
    int hurt;
    int sourceID;
protected:
    QGraphicsScene* parentScene;
    QPointF velocity;
    static constexpr qreal GRAVITY=0.05;
};
#endif // BALL_H
