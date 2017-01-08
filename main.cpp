#include "gamman3d.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gamman3d w;
    if(!w.initialize())
        return 1;
    w.show();

    return a.exec();
}
