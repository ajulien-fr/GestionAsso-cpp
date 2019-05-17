#include <QApplication>
#include "main-window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logo.png"));

    QCoreApplication::setOrganizationName("ajulien");
    QCoreApplication::setOrganizationDomain("ajulien.fr");
    QCoreApplication::setApplicationName("GestionAsso");

    QFontDatabase::addApplicationFont(":/fonts/PermanentMarker-Regular.ttf");

    QFile file(":/styles.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet(file.readAll());
    qApp->setStyleSheet(styleSheet);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
