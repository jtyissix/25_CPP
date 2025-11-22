#ifndef INTRO_H
#define INTRO_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QMediaPlayer>
#include <QAudioOutput>
class Intro : public QMainWindow {
    Q_OBJECT

public:
    explicit Intro(QWidget *parent = nullptr);
    ~Intro();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLevel1Clicked();
    void onLevel2Clicked();
    void onLevel3Clicked();
    void onHelpClicked();
    void onTask1Clicked();
    void onTask2Clicked();
    void onTask3Clicked();
private:
    void setupUI();
    void createLevelButton(QPushButton*& button, const QString& text, int level);
    void startLevel(int levelNumber);
    QMediaPlayer* bgmPlayer;
    QAudioOutput* audioOutput;
    // UI组件
    QLabel* titleLabel;
    QPushButton* level1Button;
    QPushButton* level2Button;
    QPushButton* level3Button;
    QPushButton* helpButton;
    QPushButton* task1Button;
    QPushButton* task2Button;
    QPushButton* task3Button;
    // 背景图片
    QPixmap backgroundImage;
    void createTaskButton(QPushButton*& button, int level);
    void showTaskDialog(const QString& title, const QString& taskContent);
    // 窗口尺寸
    static constexpr int WINDOW_WIDTH = 1180;
    static constexpr int WINDOW_HEIGHT = 640;
};

#endif // INTRO_H
