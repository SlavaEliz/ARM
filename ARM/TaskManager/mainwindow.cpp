#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    if((login == "slava" && password == "123")|(login == "andrey" && password == "321"))
    {
        ui->statusbar->showMessage("Успешно");
        QMessageBox::information(this, "Авторизация", "Авторизация прошла успешно!");
        window = new TaskManager(this);
        window->setWindowTitle("Менеджер заданий");
        window->show();
        QWidget::hide();
    } else
    {     
        ui->statusbar->showMessage("Не угадал");
        QMessageBox::warning(this, "Ошибка", "Не правильный логин или пароль!");
    }
}
