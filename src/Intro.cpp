#include "Intro.h"
#include "MyGame.h"
#include "GameProgress.h"
#include <QPainter>
#include <QMessageBox>
#include <QFont>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDialog>

Intro::Intro(QWidget *parent)
    : QMainWindow(parent)
    , titleLabel(nullptr)
    , level1Button(nullptr)
    , level2Button(nullptr)
    , level3Button(nullptr)
    , helpButton(nullptr)
{
    setWindowTitle("吞食鱼");
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowIcon(QIcon(":/Items/a5_icon.png"));
    // 加载背景图片
    // backgroundImage = QPixmap(":/path/to/background.png");
    backgroundImage = QPixmap(":/Items/background/bg2.jpg");
    bgmPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    bgmPlayer->setAudioOutput(audioOutput);
    bgmPlayer->setSource(QUrl("qrc:/Items/music/SkyHigh.mp3"));
    audioOutput->setVolume(0.5);  // 音量 0.0-1.0
    bgmPlayer->setLoops(QMediaPlayer::Infinite);  // 循环播放
    bgmPlayer->play();
    setupUI();
}

Intro::~Intro() {
}

void Intro::setupUI() {
    // 创建中央窗口部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建标题标签
    titleLabel = new QLabel("吞食鱼", centralWidget);
    QFont titleFont("SimHei", 72, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #FF6B35;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry((WINDOW_WIDTH - 400) / 2, 100, 400, 120);
    
    // 创建关卡按钮
    int buttonSize = 120;
    int buttonY = WINDOW_HEIGHT - 200;
    int spacing = 80;
    int totalWidth = 3 * buttonSize + 2 * spacing;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;
    
    // 关卡1按钮
    level1Button = new QPushButton("关卡1", centralWidget);
    level1Button->setGeometry(startX, buttonY, buttonSize, buttonSize);
    createLevelButton(level1Button, "关卡1", 1);
    connect(level1Button, &QPushButton::clicked, this, &Intro::onLevel1Clicked);
    // 关卡1任务按钮
    task1Button = new QPushButton("任务提示", centralWidget);
    task1Button->setGeometry(startX, buttonY + buttonSize + 15, buttonSize, 35);
    createTaskButton(task1Button, 1);
    connect(task1Button, &QPushButton::clicked, this, &Intro::onTask1Clicked);
    // 关卡2按钮
    level2Button = new QPushButton("关卡2", centralWidget);
    level2Button->setGeometry(startX + buttonSize + spacing, buttonY, buttonSize, buttonSize);
    createLevelButton(level2Button, "关卡2", 2);
    connect(level2Button, &QPushButton::clicked, this, &Intro::onLevel2Clicked);
    // 关卡2任务按钮
    task2Button = new QPushButton("任务提示", centralWidget);
    task2Button->setGeometry(startX + buttonSize + spacing, buttonY + buttonSize + 15, buttonSize, 35);
    createTaskButton(task2Button, 2);
    connect(task2Button, &QPushButton::clicked, this, &Intro::onTask2Clicked);
    // 关卡3按钮
    level3Button = new QPushButton("关卡3", centralWidget);
    level3Button->setGeometry(startX + 2 * (buttonSize + spacing), buttonY, buttonSize, buttonSize);
    createLevelButton(level3Button, "关卡3", 3);
    connect(level3Button, &QPushButton::clicked, this, &Intro::onLevel3Clicked);
    // 关卡3任务按钮
    task3Button = new QPushButton("任务提示", centralWidget);
    task3Button->setGeometry(startX + 2 * (buttonSize + spacing), buttonY + buttonSize + 15, buttonSize, 35);
    createTaskButton(task3Button, 3);
    connect(task3Button, &QPushButton::clicked, this, &Intro::onTask3Clicked);
    // 帮助按钮（右上角）
    helpButton = new QPushButton("帮助", centralWidget);
    helpButton->setGeometry(WINDOW_WIDTH - 120, 20, 100, 40);
    helpButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4ECDC4;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 20px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45B7AF;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3AA39C;"
        "}"
    );
    connect(helpButton, &QPushButton::clicked, this, &Intro::onHelpClicked);
}

void Intro::createLevelButton(QPushButton*& button, const QString& text, int level) {
    // 设置圆形按钮样式
    QString styleSheet;
    bool isUnlocked = GameProgress::instance().isLevelUnlocked(level);
    
    if (isUnlocked) {
        styleSheet = 
            "QPushButton {"
            "   background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.5, "
            "       fx:0.5, fy:0.5, stop:0 #FFD93D, stop:1 #F6C90E);"
            "   color: white;"
            "   border: 4px solid #F6C90E;"
            "   border-radius: 60px;"
            "   font-size: 20px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.5, "
            "       fx:0.5, fy:0.5, stop:0 #FFE66D, stop:1 #FFD93D);"
            "   border: 4px solid #FFD93D;"
            "}"
            "QPushButton:pressed {"
            "   background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.5, "
            "       fx:0.5, fy:0.5, stop:0 #F6C90E, stop:1 #E3B505);"
            "}";
    } else {
        styleSheet = 
            "QPushButton {"
            "   background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.5, "
            "       fx:0.5, fy:0.5, stop:0 #CCCCCC, stop:1 #999999);"
            "   color: #666666;"
            "   border: 4px solid #999999;"
            "   border-radius: 60px;"
            "   font-size: 20px;"
            "   font-weight: bold;"
            "}";
        button->setEnabled(false);
    }
    
    button->setStyleSheet(styleSheet);
}
void Intro::createTaskButton(QPushButton*& button, int level) {
    QString styleSheet;
    bool isUnlocked = GameProgress::instance().isLevelUnlocked(level);

    if (isUnlocked) {
        styleSheet =
            "QPushButton {"
            "   background-color: #4ECDC4;"
            "   color: white;"
            "   border: none;"
            "   border-radius: 17px;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #45B7AF;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #3AA39C;"
            "}";
    } else {
        styleSheet =
            "QPushButton {"
            "   background-color: #999999;"
            "   color: #666666;"
            "   border: none;"
            "   border-radius: 17px;"
            "   font-size: 14px;"
            "}";
        button->setEnabled(false);
    }

    button->setStyleSheet(styleSheet);
}

void Intro::showTaskDialog(const QString& title, const QString& taskContent) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(taskContent);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #F5F5F5;"
        "   font-size: 14px;"
        "}"
        "QMessageBox QLabel {"
        "   color: #333;"
        "   font-size: 15px;"
        "}"
        "QPushButton {"
        "   background-color: #4ECDC4;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 20px;"
        "   border-radius: 4px;"
        "   font-weight: bold;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45B7AF;"
        "}"
        );
    msgBox.exec();
}

void Intro::onTask1Clicked() {
    QString task =
        "🎯 关卡1任务目标：\n\n"
        "1. 熟悉游戏操作\n"
        "2. 活下来，吃到100条鱼长大吧！\n"
        "完成后解锁关卡2！";
    showTaskDialog("关卡1 - 任务说明", task);
}

void Intro::onTask2Clicked() {
    QString task =
        "🎯 关卡2任务目标：\n\n"
        "1. 没时间了！你只有30秒！\n"
        "2. 吃到300条鱼搭上进化的末班车\n"
        "完成后解锁关卡3！";
    showTaskDialog("关卡2 - 任务说明", task);
}

void Intro::onTask3Clicked() {
    QString task =
        "🎯 关卡3任务目标：\n\n"
        "1. 艰难险阻！人类抛下地雷\n"
        "2. 躲避地雷，吃到100条鱼\n"
        "这是最终挑战！";
    showTaskDialog("关卡3 - 任务说明", task);
}
void Intro::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    
    // 绘制背景图片
    if (!backgroundImage.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, backgroundImage);
    } else {
        // 如果没有背景图片，绘制渐变背景
        QPainter painter(this);
        QLinearGradient gradient(0, 0, 0, WINDOW_HEIGHT);
        gradient.setColorAt(0, QColor(135, 206, 250));  // 浅蓝色
        gradient.setColorAt(1, QColor(25, 118, 210));   // 深蓝色
        painter.fillRect(rect(), gradient);
    }
}

void Intro::onLevel1Clicked() {
    startLevel(1);
}

void Intro::onLevel2Clicked() {
    startLevel(2);
}

void Intro::onLevel3Clicked() {
    startLevel(3);
}

void Intro::startLevel(int levelNumber) {
    // 隐藏当前窗口
    hide();
    
    // 创建并显示游戏窗口
    MyGame* game = new MyGame(this);
    game->show();
    //game->setAttribute(Qt::WA_DeleteOnClose);
    // 当游戏窗口关闭时，重新显示主界面
    connect(game, &MyGame::windowClosed, this, [this, game]() {
        show();
        //game->deleteLater();
    });
}

void Intro::onHelpClicked() {
    // 创建帮助对话框
    QDialog* helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("游戏帮助");
    helpDialog->setFixedSize(600, 500);
    
    QVBoxLayout* layout = new QVBoxLayout(helpDialog);
    
    // 创建文本显示区域
    QTextEdit* helpText = new QTextEdit(helpDialog);
    helpText->setReadOnly(true);
    helpText->setStyleSheet(
        "QTextEdit {"
        "   background-color: #F5F5F5;"
        "   border: 2px solid #4ECDC4;"
        "   border-radius: 10px;"
        "   padding: 15px;"
        "   font-size: 14px;"
        "}"
    );
    
    // 设置帮助内容
    QString helpContent = 
        "<h2 style='color: #FF6B35; text-align: center;'>吞食鱼 - 游戏教程</h2>"
        "<br>"
        "<h3 style='color: #4ECDC4;'>游戏目标：</h3>"
        "<p>在这个海洋世界中，你将扮演一条小鱼。通过吞食比你小的鱼来成长，"
        "避开比你大的鱼，最终成为海洋霸主！</p>"
        "<br>"
        "<h3 style='color: #4ECDC4;'>操作说明：</h3>"
        "<p><b>控制：</b></p>"
        "<ul>"
        "<li>箭头上键 - 向上移动</li>"
        "<li>箭头下键 - 向下移动</li>"
        "<li>箭头左键 - 向左移动</li>"
        "<li>箭头右键 - 向下移动</li>"
        "</ul>"
        "<br>"
        "<h3 style='color: #4ECDC4;'>游戏提示：</h3>"
        "<ul>"
        "<li>吞食比你小的鱼可以增加体型</li>"
        "<li>小心避开比你大的鱼，你只有3条命</li>"
        "<li>关卡按钮下方有对应任务卡</li>"
        "<li>完成关卡解锁下一关</li>"
        "<li>药箱能重置生命，胶囊加速移动，实心泡泡让所有鱼都吃不到你</li>"
        "</ul>";
    
    helpText->setHtml(helpContent);
    
    layout->addWidget(helpText);
    
    // 创建关闭按钮
    QPushButton* closeButton = new QPushButton("关闭", helpDialog);
    closeButton->setFixedSize(120, 40);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF6B35;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 20px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FF8555;"
        "}"
    );
    connect(closeButton, &QPushButton::clicked, helpDialog, &QDialog::accept);
    
    layout->addWidget(closeButton, 0, Qt::AlignCenter);
    
    helpDialog->setLayout(layout);
    helpDialog->exec();
}
