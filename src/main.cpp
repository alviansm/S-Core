#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon("qrc:/icons/s-core_app_icon.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
