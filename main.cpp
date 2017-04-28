#include "mainwindow.h"
#include <QApplication>
//#include <qt_windows.h>

//void handler_sigsegv(int signum)
//{
//    MessageBox(NULL, L"типа SIGSEGV Error!", L"Так и не понял где ошибка", MB_ICONSTOP);
//    // открепить обработчик и явно завершить приложение
//    signal(signum, SIG_DFL);
//    exit(3);
//}

int main(int argc, char* argv[])
{
    //    signal(SIGSEGV, handler_sigsegv);
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("XrSoft");
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("MAN2");

    MainWindow w;
    int f_id = QFontDatabase::addApplicationFont(":/ubuntu-font-family-0.83/Ubuntu-R.ttf");
    w.setFont(QFont(QFontDatabase::applicationFontFamilies(f_id).first()));
    w.show();
    return a.exec();
}
