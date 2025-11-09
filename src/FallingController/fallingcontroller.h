#ifndef FALLINGCONTROLLER_H
#define FALLINGCONTROLLER_H
#include "../Items/Weapons/Rifle.h"
#include "../Items/Drugs/drug.h"
#include <QObject>
#include <QTimer>
#include <vector>
class FallingController : public QObject
{
    Q_OBJECT
public:
    explicit FallingController(QObject *parent = nullptr);

    void setFallingParams(int fallingInterval);

    void setVanishParams(int VanishInterval);

    void startLoop();

    void processMovement();

    void setFallingRange(QRectF range);

    void setScene(QGraphicsScene* scene);

    void setWeaponLifetime(int lifetime) { weaponLifetime = lifetime; }

    void setGroundLevel(qreal level) { groundLevel = level; }

    void stopAllTimers();
protected:
    std::vector<Weapon*> createdWeapon;
    std::vector<Drug*> createdDrug;
    std::vector<Armor*> createdArmor;
    QGraphicsScene* parentScene;
    qreal groundLevel{600};
protected slots:
    void fall();
    void vanish();
signals:
private:
    QTimer *fallingTimer;
    QTimer *vanishTimer;
    int fallingInterval;
    int vanishInterval;
    QRectF fallingRange;
    int weaponLifetime{4700};
};

#endif // FALLINGCONTROLLER_H
