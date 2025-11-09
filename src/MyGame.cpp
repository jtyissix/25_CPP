//
// Created by gerw on 8/20/24.
//
#include "MyGame.h"
#include "Scenes/BattleScene.h"
MyGame* MyGame::currentInstance = nullptr;
MyGame::MyGame(QWidget *parent)
    : QMainWindow(parent)
{
    currentInstance = this;
    battleScene = new BattleScene(this);
    view = new QGraphicsView(this);
    view->setScene(battleScene);
    // Set the view's window size to 1280x720
    view->setFixedSize((int) view->scene()->width(), (int) view->scene()->height());
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(battleScene, &BattleScene::gameOver, this, &MyGame::onGameOver);
    setCentralWidget(view);
    // Adjust the QMainWindow size to tightly wrap the QGraphicsView
    setFixedSize(view->sizeHint());
    battleScene->startLoop();
}

void MyGame::onGameOver(const QString& winner) {
    // 暂停游戏
    if (battleScene) {
        if (battleScene) {
            disconnect(battleScene, nullptr, this, nullptr);
        }
    }

    // 显示游戏结束对话框
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(QString("%1 获胜！ ").arg(winner));
    msgBox.setInformativeText("是否重新开始游戏？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    // 设置按钮文本
    msgBox.setButtonText(QMessageBox::Yes, "🔄 重新开始");
    msgBox.setButtonText(QMessageBox::No, "❌ 退出游戏");

    // 设置样式
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #f0f0f0;"
        "    font-size: 14px;"
        "}"
        "QMessageBox QLabel {"
        "    color: #333;"
        "    font-weight: bold;"
        "}"
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        );

    int result = msgBox.exec();

    if (result == QMessageBox::Yes) {
        // 延迟重启
        QTimer::singleShot(100, []() {
            MyGame::restartGame();
        });
    } else {
        QApplication::quit();
    }
}

// 重启游戏（创建新窗口）
void MyGame::restartGame() {
    qDebug() << "开始重启游戏...";

    MyGame* oldInstance = currentInstance;

    // 创建新窗口
    MyGame* newGame = new MyGame();
    newGame->show();

    // 延迟删除旧窗口 bug:deletelater ram leak
    if (oldInstance) {
        QTimer::singleShot(500, [oldInstance]() {
            oldInstance->close();
            //oldInstance->deleteLater();
        });
    }
}
