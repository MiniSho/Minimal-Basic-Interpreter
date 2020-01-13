#include <QApplication>
#include "basicwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    BasicWindow w;
    w.show();

    return a.exec();
}
