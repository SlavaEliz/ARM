#include "mainwindow.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Авторизация");
    w.show();
    return a.exec();
}
