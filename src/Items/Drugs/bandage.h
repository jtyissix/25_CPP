#ifndef BANDAGE_H
#define BANDAGE_H

#include "drug.h"
#include<vector>
#include "../Characters/Character.h"

class Bandage: public Drug {
public:
    explicit Bandage(QGraphicsItem *parent=nullptr);
    void mountToParent(double x,double y) override;
    virtual void effect() override;
private:
    static constexpr int SHIELD_COUNT = 1;
};

#endif
