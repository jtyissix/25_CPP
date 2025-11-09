#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPen>

class HealthBar : public QGraphicsItemGroup {
public:
    explicit HealthBar(const QString& playerName, QGraphicsItem* parent = nullptr);

    // 设置生命值 (0.0 - 1.0)
    void setHealthPercentage(qreal percentage);

    // 设置最大宽度
    void setMaxWidth(qreal width);

    // 设置位置（左上角或右上角对齐）
    void setPosition(qreal x, qreal y, bool rightAlign = false);

    // 获取当前生命值百分比
    qreal getHealthPercentage() const;

private:
    void updateHealthBar();

    QGraphicsRectItem* backgroundRect;
    QGraphicsRectItem* healthRect;
    QGraphicsTextItem* nameText;
    QGraphicsTextItem* healthText;

    qreal maxWidth;
    qreal currentHealth; // 0.0 - 1.0
    bool isRightAligned;

    static constexpr qreal BAR_HEIGHT = 20;
    static constexpr qreal TEXT_HEIGHT = 15;
    static constexpr qreal PADDING = 5;
};

#endif // HEALTHBAR_H
