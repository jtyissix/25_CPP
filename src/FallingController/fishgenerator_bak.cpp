#include "fishgenerator.h"
#include <QRandomGenerator>
#include <QDebug>

FishGenerator::FishGenerator(QObject *parent)
    : QObject(parent)
    , scene(nullptr)
    , spawnRange(0, 0, 1180, 640)
    , leftSpawnRate(1.0)
    , rightSpawnRate(1.0)
    , minVelocityX(0.1)
    , maxVelocityX(0.3)
    , minVelocityY(-0.1)
    , maxVelocityY(0.1)
    , leftSpawnAccumulator(0.0)
    , rightSpawnAccumulator(0.0)
    ,sharkSpawnRate(0.5)         // 默认每10秒生成1条Shark
    , sharkSpawnAccumulator(0.0)
{
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &FishGenerator::onSpawnTimerTimeout);
}

FishGenerator::~FishGenerator()
{
    //clearAllFish();
    /*
    for (shark* s : activeSharks) {
        if (s && scene) {
            scene->removeItem(s);
            delete s;
        }
    }
    activeSharks.clear();
*/
}

void FishGenerator::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void FishGenerator::setSpawnRange(const QRectF& range)
{
    this->spawnRange = range;
}

void FishGenerator::setSpawnRate(qreal leftRate, qreal rightRate)
{
    this->leftSpawnRate = leftRate;
    this->rightSpawnRate = rightRate;
}

void FishGenerator::setVelocityRange(qreal minX, qreal maxX, qreal minY, qreal maxY)
{
    this->minVelocityX = minX;
    this->maxVelocityX = maxX;
    this->minVelocityY = minY;
    this->maxVelocityY = maxY;
}

void FishGenerator::startGeneration()
{
    if (!scene) {
        qDebug() << "Error: Scene not set in FishGenerator";
        return;
    }

    leftSpawnAccumulator = 0.0;
    rightSpawnAccumulator = 0.0;
    spawnTimer->start(SPAWN_TIMER_INTERVAL);
    qDebug() << "FishGenerator started";
}

void FishGenerator::stopGeneration()
{
    spawnTimer->stop();
    qDebug() << "FishGenerator stopped";
}

void FishGenerator::onSpawnTimerTimeout()
{
    // 计算本次应该生成的鱼数量
    qreal interval = SPAWN_TIMER_INTERVAL / 1000.0; // 转换为秒

    // 左侧生成
    leftSpawnAccumulator += leftSpawnRate * interval;
    while (leftSpawnAccumulator >= 1.0) {
        Character* fish = spawnFish(SpawnSide::Left);
        if (fish) {
            leftSpawnAccumulator -= 1.0;
        } else {
            break;
        }
    }

    // 右侧生成
    rightSpawnAccumulator += rightSpawnRate * interval;
    while (rightSpawnAccumulator >= 1.0) {
        Character* fish = spawnFish(SpawnSide::Right);
        if (fish) {
            rightSpawnAccumulator -= 1.0;
        } else {
            break;
        }
    }
    // Shark 生成
    sharkSpawnAccumulator += sharkSpawnRate * interval;
    if (sharkSpawnAccumulator >= 1.0) {
        spawnShark();
        sharkSpawnAccumulator -= 1.0;
    }
}

Character* FishGenerator::spawnFish(SpawnSide side)
{
    if (!scene) return nullptr;

    // 创建小鱼
    smallfish* fish = new smallfish();

    // 设置位置
    qreal x, y;
    y = generateRandomY();

    if (side == SpawnSide::Left) {
        // 左侧生成，在左边界外
        x = spawnRange.left() - 50;
        fish->setDirection(true);
        fish->processDirection();        // 朝右
    } else {
        // 右侧生成，在右边界外
        x = spawnRange.right() + 50;
        fish->setDirection(false);
        fish->processDirection();// 朝左
    }

    fish->setPos(x, y);

    // 生成随机速度
    QPointF velocity = generateRandomVelocity(side);
    fish->setVelocity(velocity);

    // 添加到场景
    scene->addItem(fish);

    // 添加到活跃列表
    activeFish.append(FishInfo(fish, side, velocity));

    qDebug() << "Spawned fish at" << fish->pos() << "with velocity" << velocity;

    return fish;
}

QPointF FishGenerator::generateRandomVelocity(SpawnSide side)
{
    QRandomGenerator* rng = QRandomGenerator::global();

    // 水平速度
    qreal vx = minVelocityX + (maxVelocityX - minVelocityX) * (rng->bounded(1000) / 1000.0);

    // 垂直速度（上下随机）
    qreal vy = minVelocityY + (maxVelocityY - minVelocityY) * (rng->bounded(1000) / 1000.0);

    // 左侧生成的鱼向右游（正速度）
    if (side == SpawnSide::Left) {
        return QPointF(vx, vy);
    }
    // 右侧生成的鱼向左游（负速度）
    else {
        return QPointF(-vx, vy);
    }
}

qreal FishGenerator::generateRandomY()
{
    QRandomGenerator* rng = QRandomGenerator::global();

    // 在可生成范围内随机Y坐标
    // 留出上下边距，避免鱼贴边生成
    qreal margin = 50;
    qreal minY = spawnRange.top() + margin;
    qreal maxY = spawnRange.bottom() - margin;

    return minY + (maxY - minY) * (rng->bounded(1000) / 1000.0);
}

void FishGenerator::updateFishMovement(qint64 deltaTime)
{
    // 更新所有鱼的位置
    for (int i = 0; i < activeFish.size(); ++i) {
        FishInfo& info = activeFish[i];
        Character* fish = info.fish;

        if (!fish) {
            activeFish.removeAt(i);
            --i;
            continue;
        }

        // 更新位置
        QPointF currentPos = fish->pos();
        QPointF newPos = currentPos + info.velocity * deltaTime;
        fish->setPos(newPos);

        // 更新方向（确保鱼朝向运动方向）
        if (info.velocity.x() > 0) {
            fish->setDirection(true);
fish->processDirection();            // 向右
        } else if (info.velocity.x() < 0) {
            fish->setDirection(false);
fish->processDirection();            // 向左
        }
    }
    // 更新所有 Shark 的运动
    for (int i = activeSharks.size() - 1; i >= 0; --i) {
        shark* s = activeSharks[i];
        if (!s) {
            activeSharks.removeAt(i);
            continue;
        }

        s->updateMovement(deltaTime);

        // 检查 Shark 是否越界，越界则删除
        if (s->isOutOfBounds(spawnRange)) {
            scene->removeItem(s);
            delete s;
            activeSharks.removeAt(i);
            qDebug() << "Shark removed (out of bounds)";
        }
    }
    // 检查边界
    checkBoundaries();
}

void FishGenerator::checkBoundaries()
{
    for (int i = activeFish.size() - 1; i >= 0; --i) {
        FishInfo& info = activeFish[i];
        Character* fish = info.fish;

        if (!fish) {
            activeFish.removeAt(i);
            continue;
        }

        QRectF fishBounds = fish->sceneBoundingRect();
        bool shouldRemove = false;

        // 左侧生成的鱼：检查是否超出右边界
        if (info.spawnSide == SpawnSide::Left) {
            if (fishBounds.left() > spawnRange.right()) {
                shouldRemove = true;
                qDebug() << "Left-spawned fish exceeded right boundary";
            }
        }
        // 右侧生成的鱼：检查是否超出左边界
        else if (info.spawnSide == SpawnSide::Right) {
            if (fishBounds.right() < spawnRange.left()) {
                shouldRemove = true;
                qDebug() << "Right-spawned fish exceeded left boundary";
            }
        }

        // 移除越界的鱼
        if (shouldRemove) {
            emit fishRemoved(fish);
            scene->removeItem(fish);
            delete fish;
            activeFish.removeAt(i);
        }
    }
}

bool FishGenerator::removeFish(Character* fish)
{
    for (int i = 0; i < activeFish.size(); ++i) {
        if (activeFish[i].fish == fish) {
            emit fishRemoved(fish);
            scene->removeItem(fish);
            delete fish;
            activeFish.removeAt(i);
            qDebug() << "Fish removed (eaten or killed)";
            return true;
        }
    }
    return false;
}

void FishGenerator::clearAllFish()
{
    for (const FishInfo& info : activeFish) {
        if (info.fish && scene) {
            scene->removeItem(info.fish);
            delete info.fish;
        }
    }
    activeFish.clear();
    qDebug() << "All fish cleared";
}
void FishGenerator::setSharkSpawnRate(qreal rate) {
    sharkSpawnRate = rate;
}

void FishGenerator::spawnShark() {
    if (!scene) return;

    // 创建 Shark
    shark* newShark = new shark();

    // 初始化为敌人（随机位置和方向）
    newShark->initializeAsEnemy(spawnRange);

    // 设置速度（比小鱼快）
    newShark->setSpeed(0.5);  // 可以调整速度

    // 添加到场景
    scene->addItem(newShark);

    // 添加到活跃列表
    activeSharks.append(newShark);

    qDebug() << "Spawned a shark! Total sharks:" << activeSharks.size();
}

bool FishGenerator::removeShark(shark* sharkToRemove) {
    for (int i = 0; i < activeSharks.size(); ++i) {
        if (activeSharks[i] == sharkToRemove) {
            scene->removeItem(sharkToRemove);
            delete sharkToRemove;
            activeSharks.removeAt(i);
            qDebug() << "Shark removed";
            return true;
        }
    }
    return false;
}
