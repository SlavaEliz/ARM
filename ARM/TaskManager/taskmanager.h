#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <dialog.h>
#include <QApplication>


namespace Ui {
class TaskManager;
}

class TaskManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit TaskManager(QWidget *parent = nullptr);
    ~TaskManager();

public slots:
    void slotDialog(QString NameTask, int numer);

private slots:
    void on_add_clicked();

    void on_select_clicked();

    void on_Exit_clicked();

    void on_Destroy_clicked();

private:
    Ui::TaskManager *ui;
    Dialog *Dial;
};

#endif // TASKMANAGER_H
