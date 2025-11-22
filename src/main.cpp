#include <QApplication>
#include "MyGame.h"
#include "Intro.h"
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGame myGame;
    myGame.show();
    return QApplication::exec();
}
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Intro intro;
    intro.show();
    return QApplication::exec();
}
