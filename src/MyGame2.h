#ifndef MYGAME2_H
#define MYGAME2_H

#include <QGraphicsView>
#include <QMainWindow>
#include "Scenes/BattleScene2.h"
#include <QMessageBox>
#include <QApplication>
#include <QKeyEvent>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class MyGame2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyGame2(QWidget *parent = nullptr);
    static void restartGame();

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

    BattleScene2 *battleScene;
    QGraphicsView *view;
    static MyGame2* currentInstance;
    QDialog* pauseDialog = nullptr;
    QWidget* parentIntro = nullptr;
};

#endif