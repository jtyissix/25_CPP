#ifndef FISHGENERATOR_H
#define FISHGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <QPointF>
#include <QRectF>
#include "../Items/Characters/smallfish.h"
#include "../Items/Characters/shark.h"
// 鱼的生成位置
enum class SpawnSide {
    Left,   // 左侧生成，向右游
    Right   // 右侧生成，向左游
};

// 鱼的信息结构
struct FishInfo {
    Character* fish;           // 鱼的指针
    SpawnSide spawnSide;       // 生成侧
    QPointF velocity;          // 速度

    FishInfo(Character* f, SpawnSide side, QPointF vel)
        : fish(f), spawnSide(side), velocity(vel) {}
};

class FishGenerator : public QObject
{
    Q_OBJECT

public:
    explicit FishGenerator(QObject *parent = nullptr);
    ~FishGenerator();

    // 设置场景
    void setScene(QGraphicsScene* scene);

    // 设置生成范围
    void setSpawnRange(const QRectF& range);

    // 设置生成速率（每秒生成数量）
    void setSpawnRate(qreal leftRate, qreal rightRate);
    //void spawnShark();
    bool removeShark(shark* sharkToRemove);  // 移除特定的 Shark    // 生成一条 Shark
    // 设置速度范围
    void setVelocityRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    // Shark 相关
    void setSharkSpawnRate(qreal rate);  // 设置 Shark 生成速率
    void spawnShark();                    // 生成一条 Shark
    const QList<shark*>& getAllSharks() const { return activeSharks; }
    // 启动生成器
    void startGeneration();

    // 停止生成器
    void stopGeneration();

    // 更新所有鱼的运动（每帧调用）
    void updateFishMovement(qint64 deltaTime);

    // 删除特定的鱼（大鱼吃小鱼时调用）
    bool removeFish(Character* fish);

    // 获取所有活跃的鱼
    const QList<FishInfo>& getAllFish() const { return activeFish; }

    // 清空所有鱼
    void clearAllFish();

signals:
    void fishRemoved(Character* fish);  // 鱼被移除时发出信号

private slots:
    void onSpawnTimerTimeout();

private:
    // 生成一条鱼
    Character* spawnFish(SpawnSide side);

    // 检查边界并移除越界的鱼
    void checkBoundaries();

    // 随机生成速度
    QPointF generateRandomVelocity(SpawnSide side);

    // 随机生成Y坐标
    qreal generateRandomY();

    QGraphicsScene* scene;
    QRectF spawnRange;           // 生成范围（场景边界）

    QTimer* spawnTimer;          // 生成定时器
    qreal leftSpawnRate;         // 左侧每秒生成数量
    qreal rightSpawnRate;        // 右侧每秒生成数量

    qreal minVelocityX;          // 最小水平速度
    qreal maxVelocityX;          // 最大水平速度
    qreal minVelocityY;          // 最小垂直速度
    qreal maxVelocityY;          // 最大垂直速度

    QList<FishInfo> activeFish;  // 所有活跃的鱼
    QList<shark*> activeSharks;  // 所有活跃的 Shark
    qreal leftSpawnAccumulator;  // 左侧生成累积器
    qreal rightSpawnAccumulator; // 右侧生成累积器
    qreal sharkSpawnRate;        // Shark 生成速率
    qreal sharkSpawnAccumulator; // Shark 生成累积器
    static constexpr int SPAWN_TIMER_INTERVAL = 100; // 生成定时器间隔（毫秒）
};

#endif // FISHGENERATOR_H
