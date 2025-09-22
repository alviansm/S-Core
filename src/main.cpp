#include <QApplication>
#include "ui/MainWindow.h"
#include <QWebEngineSettings>

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9222");

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon("qrc:/icons/s-core_app_icon.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
