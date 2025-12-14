#include <QTimer>
#include <QLabel>
#include "MyGame2.h"
#include "Scenes/BattleScene2.h"

MyGame2* MyGame2::currentInstance = nullptr;

MyGame2::MyGame2(QWidget *parent) : QMainWindow(parent) {
    currentInstance = this;
    if (parent && parent->inherits("Intro")) {
        parentIntro = parent;
    }
    battleScene = new BattleScene2(this);
    view = new QGraphicsView(this);
    view->setScene(battleScene);
    view->setFixedSize((int) view->scene()->width(), (int) view->scene()->height());
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(battleScene, &BattleScene2::gameOver, this, &MyGame2::onGameOver);
    setCentralWidget(view);
    setFixedSize(view->sizeHint());
    battleScene->startLoop();
    createPauseDialog();
}

void MyGame2::onGameOver(const QString& winner) {
    if (battleScene) {
        disconnect(battleScene, nullptr, this, nullptr);
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("游戏结束");
    msgBox.setText(QString("%1").arg(winner));
    msgBox.setInformativeText("是否重新开始游戏？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "🔄 重新开始");
    msgBox.setButtonText(QMessageBox::No, "❌ 退出游戏");
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #f0f0f0; font-size: 14px; }"
        "QMessageBox QLabel { color: #333; font-size: 15px; font-weight: bold; }"
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "padding: 8px 16px; border-radius: 4px; font-weight: bold; min-width: 80px; }"
        "QPushButton:hover { background-color: #45a049; }"
    );

    int result = msgBox.exec();
    if (result == QMessageBox::Yes) {
        QTimer::singleShot(100, []() { MyGame2::restartGame(); });
    } else {
        close();
    }
}

void MyGame2::restartGame() {
    qDebug() << "开始重启游戏...";
    MyGame2* oldInstance = currentInstance;
    MyGame2* newGame = new MyGame2();
    newGame->show();
    if (oldInstance) {
        QTimer::singleShot(500, [oldInstance]() {});
    }
}

void MyGame2::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    event->accept();
}

void MyGame2::createPauseDialog() {
    pauseDialog = new QDialog(this);
    pauseDialog->setWindowTitle("游戏暂停");
    pauseDialog->setModal(true);
    pauseDialog->setFixedSize(300, 300);
    pauseDialog->setStyleSheet(
        "QDialog { background-color: rgba(30, 144, 255, 180); border-radius: 20px; }"
    );

    QVBoxLayout* layout = new QVBoxLayout(pauseDialog);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 50, 40, 50);

    QLabel* titleLabel = new QLabel("游戏暂停", pauseDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { color: white; font-size: 24px; font-weight: bold; background: transparent; }"
    );
    layout->addWidget(titleLabel);
    layout->addSpacing(20);

    QPushButton* continueBtn = createCapsuleButton("继续游戏", QColor(76, 175, 80));
    connect(continueBtn, &QPushButton::clicked, pauseDialog, &QDialog::accept);
    layout->addWidget(continueBtn);

    QPushButton* restartBtn = createCapsuleButton("重新开始", QColor(255, 152, 0));
    connect(restartBtn, &QPushButton::clicked, this, [this]() {
        pauseDialog->accept();
        hide();
        MyGame2::restartGame();
    });
    layout->addWidget(restartBtn);

    QPushButton* exitBtn = createCapsuleButton("退出到主界面", QColor(244, 67, 54));
    connect(exitBtn, &QPushButton::clicked, this, [this]() {
        pauseDialog->accept();
        close();
    });
    layout->addWidget(exitBtn);
    layout->addStretch();
    pauseDialog->setLayout(layout);
}

QPushButton* MyGame2::createCapsuleButton(const QString& text, const QColor& color) {
    QPushButton* button = new QPushButton(text);
    button->setFixedSize(220, 50);
    button->setCursor(Qt::PointingHandCursor);
    QString styleSheet = QString(
        "QPushButton { background-color: %1; color: white; border: none; border-radius: 25px; "
        "font-size: 16px; font-weight: bold; padding: 10px; }"
        "QPushButton:hover { background-color: %2; }"
        "QPushButton:pressed { background-color: %3; }"
    ).arg(color.name()).arg(color.lighter(110).name()).arg(color.darker(110).name());
    button->setStyleSheet(styleSheet);
    return button;
}

void MyGame2::showPauseMenu() {
    if (battleScene) {
        battleScene->stopAllTimers();
    }
    int result = pauseDialog->exec();
    if (result == QDialog::Accepted && battleScene) {
        battleScene->startLoop();
    }
}

void MyGame2::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        showPauseMenu();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}