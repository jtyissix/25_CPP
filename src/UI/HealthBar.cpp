#include "HealthBar.h"
#include <QFont>
#include <QColor>
#include <algorithm>

HealthBar::HealthBar(const QString& playerName, QGraphicsItem* parent)
    : QGraphicsItemGroup(parent)
    , maxWidth(200)
    , currentHealth(1.0)
    , isRightAligned(false)
{
    // 创建背景矩形
    backgroundRect = new QGraphicsRectItem(this);
    backgroundRect->setBrush(QBrush(QColor(100, 100, 100, 180)));
    backgroundRect->setPen(QPen(QColor(255, 255, 255), 2));

    // 创建血条矩形
    healthRect = new QGraphicsRectItem(this);
    healthRect->setBrush(QBrush(QColor(220, 50, 50)));
    healthRect->setPen(QPen(Qt::NoPen));

    // 创建玩家名称文本
    nameText = new QGraphicsTextItem(this);
    nameText->setPlainText(playerName);
    QFont nameFont("Arial", 12, QFont::Bold);
    nameText->setFont(nameFont);
    nameText->setDefaultTextColor(QColor(255, 255, 255));

    // 创建血量文本
    healthText = new QGraphicsTextItem(this);
    QFont healthFont("Arial", 10);
    healthText->setFont(healthFont);
    healthText->setDefaultTextColor(QColor(255, 255, 255));

    updateHealthBar();
}

void HealthBar::setHealthPercentage(qreal percentage) {
    currentHealth = std::clamp(percentage, 0.0, 1.0);
    updateHealthBar();
}

void HealthBar::setMaxWidth(qreal width) {
    maxWidth = width;
    updateHealthBar();
}

void HealthBar::setPosition(qreal x, qreal y, bool rightAlign) {
    isRightAligned = rightAlign;
    setPos(x, y);
    updateHealthBar();
}

qreal HealthBar::getHealthPercentage() const {
    return currentHealth;
}

void HealthBar::updateHealthBar() {
    // 更新血量文本
    int healthPoints = static_cast<int>(currentHealth * 100);
    healthText->setPlainText(QString("%1/100").arg(healthPoints));

    // 计算布局
    qreal totalHeight = TEXT_HEIGHT + BAR_HEIGHT + PADDING;

    if (isRightAligned) {
        // 右对齐布局
        nameText->setPos(-nameText->boundingRect().width(), -totalHeight);
        healthText->setPos(-healthText->boundingRect().width(), -totalHeight + TEXT_HEIGHT + PADDING/2);

        backgroundRect->setRect(-maxWidth, -totalHeight + TEXT_HEIGHT + PADDING, maxWidth, BAR_HEIGHT);

        qreal healthWidth = maxWidth * currentHealth;
        healthRect->setRect(-maxWidth, -totalHeight + TEXT_HEIGHT + PADDING, healthWidth, BAR_HEIGHT);
    } else {
        // 左对齐布局
        nameText->setPos(0, -totalHeight);
        healthText->setPos(0, -totalHeight + TEXT_HEIGHT + PADDING/2);

        backgroundRect->setRect(0, -totalHeight + TEXT_HEIGHT + PADDING, maxWidth, BAR_HEIGHT);

        qreal healthWidth = maxWidth * currentHealth;
        healthRect->setRect(0, -totalHeight + TEXT_HEIGHT + PADDING, healthWidth, BAR_HEIGHT);
    }

    // 根据血量改变颜色
    QColor healthColor;
    if (currentHealth > 0.6) {
        healthColor = QColor(50, 220, 50); // 绿色
    } else if (currentHealth > 0.3) {
        healthColor = QColor(220, 220, 50); // 黄色
    } else {
        healthColor = QColor(220, 50, 50); // 红色
    }

    healthRect->setBrush(QBrush(healthColor));
}
