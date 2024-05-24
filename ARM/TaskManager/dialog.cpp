#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    static int numer = 0;
    numer++;

    QString Num = QString::number(numer) + "№" + "Task";
    QFile file(Num);

    file.open(QIODevice::WriteOnly | QIODevice::Append);

    QString TaskName = ui->TaskName->text();
    QString TaskDesc = ui->TaskDesc->text();

    QTextStream out(&file);
    out << TaskName << "\n";
    out << TaskDesc << "\n";

    file.close();
    emit signalDialog(TaskName, numer);

    QWidget::close();
}
