#ifndef GRASS_H
#define GRASS_H

#include "../Item.h"
#include "../Mountable.h"
#include "../Characters/Character.h"
class Grass : public Item, public Mountable
{
public:
    explicit Grass(QGraphicsItem *parent, const QString &pixmapPath=":/Items/bush.svg");
    void mountToParent(double x, double y) override;
    bool isCharacterInGrass(const Character* character, qreal characterWidth = 50) const;
    QRectF getGrassArea() const;
};

#endif // GRASS_H
