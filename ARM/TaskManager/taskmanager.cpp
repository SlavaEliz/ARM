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

void TaskManager::slotDialog(QString NameTask, int numer)
{
    QString Add = QString::number(numer) + ". " + NameTask;
    ui->listWidget->addItem(Add);
}

void TaskManager::on_add_clicked()
{
    Dial = new Dialog(this);
    Dial->setWindowTitle("Добавление задания");
    Dial->show();

    static int numer = 0;
    numer++;
    connect(Dial, &Dialog::signalDialog, this, &TaskManager::slotDialog);
}

void TaskManager::on_select_clicked()
{
    QString Select = ui->listWidget->currentItem()->text().mid(0, 1) + "№Task";
    QFile file(Select);
    file.open(QIODevice::ReadOnly);
    QString Text = file.readAll();
    QMessageBox::about(this, "Задание", Text);
}

void TaskManager::on_Destroy_clicked()
{
    QString Select = ui->listWidget->currentItem()->text().mid(0, 1) + "№Task";
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
    QFile file(Select);
    file.remove();
}

void TaskManager::on_Exit_clicked()
{
    QApplication::quit();
}
