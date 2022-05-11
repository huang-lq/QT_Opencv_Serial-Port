#include "QT_OpenCV.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QT_OpenCV w;
    w.show();
    return a.exec();
}
