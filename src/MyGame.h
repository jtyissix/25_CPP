//
// Created by gerw on 8/20/24.
//

#ifndef QT_PROGRAMMING_2024_MYGAME_H
#define QT_PROGRAMMING_2024_MYGAME_H

#include <QGraphicsView>
#include <QMainWindow>
#include "Scenes/BattleScene.h"
#include <QMessageBox>
#include <QApplication>
class MyGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);
    static void restartGame();
private slots:
    void onGameOver(const QString& winner);
private:
    BattleScene *battleScene;
    QGraphicsView *view;
    static MyGame* currentInstance;
};

#endif //QT_PROGRAMMING_2024_MYGAME_H
