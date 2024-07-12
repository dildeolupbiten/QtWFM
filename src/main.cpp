#include "../include/widget.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    init_COLORS();
    QFile file(":/style.qss");
    if (!file.open(QFile::ReadOnly)) {
        qWarning("Couldn't open stylesheet file.");
        return -1;
    }
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QString baseStylesheet = QLatin1String(file.readAll());
    a.setStyleSheet(baseStylesheet);
    Widget w;
    w.show();
    return a.exec();
}
