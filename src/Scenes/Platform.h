//
// Platform.h - 平台系统头文件
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QRectF>
#include <QBrush>
#include <QPen>

class Platform : public QGraphicsRectItem {
public:
    explicit Platform(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

    // 获取平台的碰撞矩形
    QRectF getCollisionRect() const;

    // 检查点是否在平台上方
    bool isPointAbove(const QPointF& point, qreal tolerance = 5.0) const;

    // 获取平台顶部Y坐标
    qreal getTopY() const;

    // 获取平台底部Y坐标
    qreal getBottomY() const;

    // 检查角色是否可以站在此平台上
    bool canStandOn(const QRectF& characterBounds) const;

    // 设置平台样式
    void setPlatformStyle(const QBrush& brush, const QPen& pen);

private:
    QRectF collisionRect;
};

#endif // PLATFORM_H
