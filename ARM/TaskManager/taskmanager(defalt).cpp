#include "taskmanager.h"
#include "ui_taskmanager.h"
#include <QMessageBox>


TaskManager::TaskManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TaskManager)
{
    ui->setupUi(this);

}

TaskManager::~TaskManager()
{
    delete ui;
}


void TaskManager::on_add_clicked()
{
    QString Add = ui->Text->text();
    ui->listWidget->addItem(Add);
}


void TaskManager::on_delete_1_clicked()
    {
        ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
    }




void TaskManager::on_select_clicked()
{
    QString select = ui->listWidget->currentItem()->text();

}
