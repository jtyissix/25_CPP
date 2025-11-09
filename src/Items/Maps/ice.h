#ifndef ICE_H
#define ICE_H



#include "../Item.h"
#include "../Mountable.h"
#include "../Characters/Character.h"
class Ice : public Item, public Mountable
{
public:
    explicit Ice(QGraphicsItem *parent, const QString &pixmapPath=":/Items/icleblock.svg");
    void mountToParent(double x, double y) override;
    bool isCharacterInIce(const Character* character, qreal characterWidth = 50) const;
    QRectF getIceArea() const;
};

#endif // ICE_H
