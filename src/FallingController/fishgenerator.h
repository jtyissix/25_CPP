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
#include "../Items/Characters/midfish.h"

enum class SpawnSide {
    Left,
    Right
};

struct FishInfo {
    Character* fish;
    SpawnSide spawnSide;
    QPointF velocity;

    FishInfo(Character* f, SpawnSide side, QPointF vel)
        : fish(f), spawnSide(side), velocity(vel) {}
};

class FishGenerator : public QObject
{
    Q_OBJECT

public:
    explicit FishGenerator(QObject *parent = nullptr);
    ~FishGenerator();

    void setScene(QGraphicsScene* scene);
    void setSpawnRange(const QRectF& range);
    void setSpawnRate(qreal leftRate, qreal rightRate);
    bool removeShark(shark* sharkToRemove);
    void setVelocityRange(qreal minX, qreal maxX, qreal minY, qreal maxY);
    void setSharkSpawnRate(qreal rate);
    void spawnShark();
    const QList<shark*>& getAllSharks() const { return activeSharks; }
    
    void setMidfishSpawnRate(qreal rate);
    void spawnMidfish(const QPointF& targetPos);
    bool removeMidfish(midfish* midfishToRemove);
    const QList<midfish*>& getAllMidfish() const { return activeMidfish; }
    void setTargetFish(Character* target) { targetFish = target; }
    
    void startGeneration();
    void stopGeneration();
    void updateFishMovement(qint64 deltaTime);
    bool removeFish(Character* fish);
    const QList<FishInfo>& getAllFish() const { return activeFish; }
    void clearAllFish();

signals:
    void fishRemoved(Character* fish);

private slots:
    void onSpawnTimerTimeout();

private:
    Character* spawnFish(SpawnSide side);
    void checkBoundaries();
    QPointF generateRandomVelocity(SpawnSide side);
    qreal generateRandomY();

    QGraphicsScene* scene;
    QRectF spawnRange;
    QTimer* spawnTimer;
    qreal leftSpawnRate;
    qreal rightSpawnRate;
    qreal minVelocityX;
    qreal maxVelocityX;
    qreal minVelocityY;
    qreal maxVelocityY;

    QList<FishInfo> activeFish;
    QList<shark*> activeSharks;
    QList<midfish*> activeMidfish;
    Character* targetFish;
    
    qreal leftSpawnAccumulator;
    qreal rightSpawnAccumulator;
    qreal sharkSpawnRate;
    qreal sharkSpawnAccumulator;
    qreal midfishSpawnRate;
    qreal midfishSpawnAccumulator;
    
    static constexpr int SPAWN_TIMER_INTERVAL = 100;
};

#endif
