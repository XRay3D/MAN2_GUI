#include "mainwindow.h"

#include "hw/hwi.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    HWI hw;
    Q_UNUSED(hw)

    MainWindow w;
    w.show();

    return a.exec();
}
