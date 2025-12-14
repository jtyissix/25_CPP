#ifndef BATTLEFIELD2_H
#define BATTLEFIELD2_H

#include "Map.h"

class Battlefield2: public Map {
public:
    explicit Battlefield2(QGraphicsItem *parent= nullptr);
    qreal getFloorHeight() override;
};

#endif
