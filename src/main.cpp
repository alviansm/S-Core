#include <QApplication>
#include "ui/MainWindow.h"
#include <QWebEngineSettings>
#include <QFile>

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9222");

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon("qrc:/icons/s-core_app_icon.png"));

    QFile StyleSheetFile(":/app.css");
    if (StyleSheetFile.open(QIODevice::ReadOnly)) {
        QTextStream StyleSheetStream(&StyleSheetFile);
        a.setStyleSheet(StyleSheetStream.readAll());
        StyleSheetFile.close();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
