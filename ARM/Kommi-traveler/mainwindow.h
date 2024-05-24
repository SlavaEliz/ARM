#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Position.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    int verticesCount;
    int startVerticesCount = 3;
    int deletedVerticesCount = 1;
    int rangeToFillAdjacencyMatrix = 3;
    int bestWayList[9];
    int currentStage = 0;
    double circusGrad = 360;
    int roadsForComputing[9][9];
    int roadsForDrawing[9][9];
    int vertexToDelete = 1;
    int deletedVertices1 = 12;
    int deletedVertices2 = 12;
    int roadsList[100];
    int smallesVerticesInRow[9];
    int smallesVerticesInColumn[9];
    int smallesVertices[9][2];
    Position *coord;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void writeVertexNumbers();
    void hideAdjacencyMatrix();
    void showAdjacencyMatrix();
    void findStartVertex();
    void initializeMinArrays();
    void subtract_row_minima();
    void subtract_column_minima();
    void findBestWay(int num);
    void paintEvent(QPaintEvent* event);
private slots:
    void on_create_clicked();
    void on_build_clicked();
    void on_calc_clicked();
    void on_clear_clicked();
    void on_fill_clicked();
    void on_del_clicked();
};

#endif // MAINWINDOW_H
