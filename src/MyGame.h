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
#include <QKeyEvent>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class MyGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyGame(QWidget *parent = nullptr);
    static void restartGame();
    static void returnToMainMenu();
signals:
    void windowClosed();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onGameOver(const QString& winner);
    void showPauseMenu();

private:
    void createPauseDialog();
    QPushButton* createCapsuleButton(const QString& text, const QColor& color);

    BattleScene *battleScene;
    QGraphicsView *view;
    static MyGame* currentInstance;
    QDialog* pauseDialog = nullptr;
    QWidget* parentIntro = nullptr;
};



#endif //QT_PROGRAMMING_2024_MYGAME_H
