//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_Figure_H
#define QT_PROGRAMMING_2024_Figure_H

#include "../Item.h"
#include "../Mountable.h"

class Figure : public Item, public Mountable {
public:
    explicit Figure(QGraphicsItem *parent, const QString &pixmapPath);

    void mountToParent(double x, double y) override;

    void unmount() override;
};


#endif //QT_PROGRAMMING_2024_Figure_H
