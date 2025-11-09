//
// Platform.cpp - 平台系统实现
//

#include "Platform.h"
#include <QBrush>
#include <QPen>

Platform::Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, height, parent) {

    // 设置碰撞矩形
    collisionRect = QRectF(x, y, width, height);

    // 设置默认样式
    setBrush(QBrush(QColor(139, 69, 19))); // 棕色平台
    setPen(QPen(QColor(101, 67, 33), 2));   // 深棕色边框

    // 设置Z值，确保平台在角色下方
    //setZValue(-1);
}

QRectF Platform::getCollisionRect() const {
    return mapRectToScene(rect());
}

bool Platform::isPointAbove(const QPointF& point, qreal tolerance) const {
    QRectF sceneRect = getCollisionRect();
    return (point.x() >= sceneRect.left() &&
            point.x() <= sceneRect.right() &&
            point.y() <= sceneRect.top() + tolerance &&
            point.y() >= sceneRect.top() - tolerance);
}

qreal Platform::getTopY() const {
    return getCollisionRect().top();
}

qreal Platform::getBottomY() const {
    return getCollisionRect().bottom();
}

bool Platform::canStandOn(const QRectF& characterBounds) const {
    QRectF platformRect = getCollisionRect();

    // 检查角色是否在平台水平范围内
    bool horizontalOverlap = !(characterBounds.right() < platformRect.left() ||
                               characterBounds.left() > platformRect.right());

    // 检查角色是否从上方接触平台
    bool verticalContact = characterBounds.bottom() >= platformRect.top() &&
                           characterBounds.bottom() <= platformRect.top() + 10; // 10像素容差

    return horizontalOverlap && verticalContact;
}

void Platform::setPlatformStyle(const QBrush& brush, const QPen& pen) {
    setBrush(brush);
    setPen(pen);
}
