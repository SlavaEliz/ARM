#include "mainwindow.h"
#include <QApplication>
#include<ctime>

int main(int argc, char *argv[]) {
    srand(time(0));
    QApplication qApplication(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Коммивояжер");
    mainWindow.show();
    return qApplication.exec();
}
