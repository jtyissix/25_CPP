#ifndef BATTLEFIELD3_H
#define BATTLEFIELD3_H

#include "Map.h"

class Battlefield3: public Map {
public:
    explicit Battlefield3(QGraphicsItem *parent= nullptr);
    qreal getFloorHeight() override;
};

#endif
