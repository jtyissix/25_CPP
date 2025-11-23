#include <QTimer>
#include <QLabel>
#include "MyGame.h"
#include "Scenes/BattleScene.h"
MyGame* MyGame::currentInstance = nullptr;
MyGame::MyGame(QWidget *parent)
    : QMainWindow(parent)
{
    currentInstance = this;
    if (parent && parent->inherits("Intro")) {
        parentIntro = parent;
    }
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
    createPauseDialog();
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
            //oldInstance->close();
            //oldInstance->deleteLater();
        });
    }
}

void MyGame::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    event->accept();
}

// 创建暂停对话框
void MyGame::createPauseDialog() {
    pauseDialog = new QDialog(this);
    pauseDialog->setWindowTitle("游戏暂停");
    pauseDialog->setModal(true);
    pauseDialog->setFixedSize(300, 300);

    // 设置对话框样式
    pauseDialog->setStyleSheet(
        "QDialog {"
        "    background-color: rgba(30, 144, 255, 180);"
        "    border-radius: 20px;"
        "}"
        );

    QVBoxLayout* layout = new QVBoxLayout(pauseDialog);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 50, 40, 50);

    // 标题标签
    QLabel* titleLabel = new QLabel("游戏暂停", pauseDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    background: transparent;"
        "}"
        );
    layout->addWidget(titleLabel);

    layout->addSpacing(20);

    // 继续游戏按钮
    QPushButton* continueBtn = createCapsuleButton("继续游戏", QColor(76, 175, 80));
    connect(continueBtn, &QPushButton::clicked, pauseDialog, &QDialog::accept);
    layout->addWidget(continueBtn);

    // 重新开始按钮
    QPushButton* restartBtn = createCapsuleButton("重新开始", QColor(255, 152, 0));
    connect(restartBtn, &QPushButton::clicked, this, [this]() {
        pauseDialog->accept();
        hide();
        MyGame::restartGame();
    });
    layout->addWidget(restartBtn);

    // 退出到主界面按钮
    QPushButton* exitBtn = createCapsuleButton("退出到主界面", QColor(244, 67, 54));
    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        pauseDialog->accept();
        close();  // 这会触发 windowClosed 信号
    });
    layout->addWidget(exitBtn);

    layout->addStretch();

    pauseDialog->setLayout(layout);
}

// 创建胶囊形状按钮
QPushButton* MyGame::createCapsuleButton(const QString& text, const QColor& color) {
    QPushButton* button = new QPushButton(text);
    button->setFixedSize(220, 50);
    button->setCursor(Qt::PointingHandCursor);

    QString styleSheet = QString(
                             "QPushButton {"
                             "    background-color: %1;"
                             "    color: white;"
                             "    border: none;"
                             "    border-radius: 25px;"
                             "    font-size: 16px;"
                             "    font-weight: bold;"
                             "    padding: 10px;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: %2;"
                             "}"
                             "QPushButton:pressed {"
                             "    background-color: %3;"
                             "}"
                             ).arg(color.name())
                             .arg(color.lighter(110).name())
                             .arg(color.darker(110).name());

    button->setStyleSheet(styleSheet);
    return button;
}

// 显示暂停菜单
void MyGame::showPauseMenu() {
    if (battleScene) {
        battleScene->stopAllTimers();  // 停止游戏循环
    }

    // 显示对话框（阻塞）
    int result = pauseDialog->exec();

    // 如果点击继续游戏或关闭对话框
    if (result == QDialog::Accepted && battleScene) {
        battleScene->startLoop();  // 恢复游戏循环
    }
}

void MyGame::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        showPauseMenu();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
