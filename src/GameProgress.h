#ifndef GAMEPROGRESS_H
#define GAMEPROGRESS_H

#include <QObject>

// 游戏进度管理单例类
class GameProgress : public QObject {
    Q_OBJECT
    
public:
    // 获取单例实例
    static GameProgress& instance() {
        static GameProgress instance;
        return instance;
    }
    
    // 获取当前已完成的关卡数（0表示未完成任何关卡）
    int getCompletedLevel() const { return completedLevel; }
    
    // 设置已完成的关卡数
    void setCompletedLevel(int level) { 
        if (level > completedLevel) {
            completedLevel = level;
        }
    }
    
    // 检查某个关卡是否已解锁
    bool isLevelUnlocked(int level) const {
        return true;
        //return level <= (completedLevel + 1);
    }
    
    // 重置进度
    void resetProgress() {
        completedLevel = 0;
    }
    
private:
    GameProgress() : completedLevel(0) {}
    GameProgress(const GameProgress&) = delete;
    GameProgress& operator=(const GameProgress&) = delete;
    
    int completedLevel; // 已完成的最高关卡数
};

#endif // GAMEPROGRESS_H
