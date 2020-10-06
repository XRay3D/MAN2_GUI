#include "mainwindow.h"
#include "settings.h"
#include <QApplication>
#include <QFontDatabase>
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
    QCoreApplication::setApplicationName("Электросила");

    //    MySettings settings;
    //    w.setFont(QFont(QFontDatabase::applicationFontFamilies(f_id).first()));
    //    int f_id = QFontDatabase::addApplicationFont(":/ubuntu-font-family-0.83/Ubuntu-R.ttf");
    //    QFont f(QFontDatabase::applicationFontFamilies(f_id).first());

    //    QFont f;
    //    f.setPointSize(12);
    //    a.setFont(f);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, a.applicationDirPath());

    mi mi_;
    Q_UNUSED(mi_)

    MainWindow w;

    w.show();
    return a.exec();
}
