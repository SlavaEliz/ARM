#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_create_clicked() { //Кнопка "Ввести вершины" отвечает за создание матрицы смежности [count*count] и за создание count количества - вершин графа
    verticesCount = ui->verticesCountInputBox->toPlainText().toInt();
    showAdjacencyMatrix();
    coord = new Position[verticesCount];
    currentStage = 1;
    this->repaint();

    rangeToFillAdjacencyMatrix = ui->verticesCountInputBox->toPlainText().toInt();
    QString message;
    int randomValue;
    for (int i = 1; i <= verticesCount; i++) {
        for (int j = i + 1; j <= verticesCount; j++) {
            randomValue = rand() % rangeToFillAdjacencyMatrix;
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText(QString::number(randomValue));
        }
        for (int j = 1; j < i; j++) {
            randomValue = rand() % rangeToFillAdjacencyMatrix;
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText(QString::number(randomValue));
        }
    }
}

void MainWindow::on_build_clicked() { //Кнопка "Построить связи" отображает связи между вершинами графа
    QString message1;
    QString message2; //Вспомогательные переменные для передачи значений в массив
    for (int i = 0; i < verticesCount; i++) {
        for ( int j = 0; j < verticesCount; j++) {
            if (i == j) {
                roadsForComputing[i][j] = -1;
                roadsForDrawing[i][j] = -1;
                continue;
            }
            message1 = "road" + QString::number(i + 1) + QString::number(j + 1); //Из таблицы передаем значения в массивы
            message2 = this->findChild<QLineEdit*>(message1)->text();
            roadsForDrawing[i][j] = message2.toInt(); //Преобразуем из string в int
            if (!message2.length()) {
                message2 = QString::number(INT32_MAX);
            }
            roadsForComputing[i][j] = message2.toInt();
            if (i == deletedVertices1 - 1 || j == deletedVertices1 - 1 || i == deletedVertices2 - 1 || j == deletedVertices2 - 1) {
                //Если хотя бы одна вершина удалена, условие выполняется
                roadsForComputing[i][j] = -1;
            }
        }
    }
    ui->calc->show();
    currentStage = 2;
    this->repaint();
}

void MainWindow::on_calc_clicked() {//Кнопка "Вычислить" - определяет оптимальный путь
    QString massage1;
    QString message2;
    for(int i = 0; i < verticesCount; i++) {
        findStartVertex(); //Функция поиска минимального элемента для оптимального пути
        findBestWay(i); //Функция поиска элементов для оптимального пути
    }
    for (int i = 0; i < verticesCount; i++) {
        for (int j = 0; j < verticesCount; j++) {
            if (i == j) {
                roadsForComputing[i][j] = -1;
                continue;
            }
            massage1= "road" + QString::number(i + 1) + QString::number(j + 1); //Из матрицы передаем значения в массивы
            message2 = this->findChild<QLineEdit*>(massage1)->text();
            if (!message2.length()) {
                message2 = QString::number(INT32_MAX);
            }
            roadsForComputing[i][j] = message2.toInt();
        }
    }
    QString bestWay;
    bestWay += "Кратчайший путь: " + QString::number(smallesVertices[0][0] + 1) + " -> " + QString::number(smallesVertices[0][1] + 1);
    int point = smallesVertices[0][1];
    int summ = roadsForComputing[smallesVertices[0][0]][smallesVertices[0][1]];

    bestWayList[0] = smallesVertices[0][0];
    bestWayList[1] = smallesVertices[0][1];

    int currentVertex = 1;
    while (currentVertex < verticesCount) { //Получаем наикратчайший путь
        for (int i = 1; i < verticesCount; i++) {
            if (!(smallesVertices[i][0] == point && smallesVertices[i][0] != smallesVertices[i][1])) {
                continue;
            }
            bestWayList[currentVertex+1] = point = smallesVertices[i][1];
            bestWay += " -> " + QString::number(smallesVertices[i][1]+1);
            summ += roadsForComputing[smallesVertices[i][0]][smallesVertices[i][1]];
            smallesVertices[i][0] = -1;
            smallesVertices[i][1] = -1;
            break;
        }
        currentVertex++;
    }
    currentStage = 3;
    this->repaint();
    QString duration = "\nДлина маршрута: " + QString::number(summ) + '\n';
    ui->label_2->show();
    ui->label_2->setText(duration + bestWay); //Выводим ответ
}

void MainWindow::findStartVertex() {
    initializeMinArrays();
    subtract_row_minima();
    subtract_column_minima();
}

void MainWindow::initializeMinArrays() {
    for (int i = 0; i < verticesCount; i++) {
        smallesVerticesInRow[i] = INT32_MAX;
        smallesVerticesInColumn[i] = INT32_MAX;
    }
}

void MainWindow::subtract_row_minima() {
    for (int i = 0; i < verticesCount; i++) {
        for (int j = 0; j < verticesCount; j++) {
            if (i != j && roadsForComputing[i][j] > -1 && roadsForComputing[i][j] < smallesVerticesInRow[i]) {
                smallesVerticesInRow[i] = roadsForComputing[i][j];
            }
        }
        for (int j = 0; j < verticesCount; j++) {
            if (i != j && smallesVerticesInRow[i] > 0 && smallesVerticesInRow[i] < 1000) {
                roadsForComputing[i][j] -= smallesVerticesInRow[i];
            }
        }
    }
}

void MainWindow::subtract_column_minima() {
    for (int j = 0; j < verticesCount; j++) {
        for (int i = 0; i < verticesCount; i++) {
            if (i != j && roadsForComputing[i][j] > -1 && roadsForComputing[i][j] < smallesVerticesInColumn[j]) {
                smallesVerticesInColumn[j] = roadsForComputing[i][j];
            }
        }
        for (int i = 0; i < verticesCount; i++) {
            if (i != j && smallesVerticesInColumn[j] > 0 && smallesVerticesInColumn[j] < 1000) {
                roadsForComputing[i][j] -= smallesVerticesInColumn[j];
            }
        }
    }
}

void MainWindow::findBestWay(int num) { //Функция поиска элементов для оптимального пути
    int min_in_row = INT32_MAX;
    int min_in_column = INT32_MAX;
    int max_rank_null = 0;
    int max_i = 100;
    int max_j = 100;

    for (int i = 0; i < verticesCount; i++) {
        for (int j = 0; j < verticesCount; j++) {
            if (roadsForComputing[i][j] != 0 || i ==j) {
                continue;
            }
            for (int k = 0; k < verticesCount; k++) {
                if (roadsForComputing[k][j] > -1 && roadsForComputing[k][j] < min_in_column && k!=j && k!=i) {
                    min_in_column = roadsForComputing[k][j];
                }
                if (roadsForComputing[i][k] > -1 && roadsForComputing[i][k] < min_in_row && i!=k && k!=j) {
                    min_in_row = roadsForComputing[i][k];
                }
            }
            min_in_row = min_in_row * (min_in_row <= 1000);    //обнуляем если <= 1000
            min_in_column = min_in_column * (min_in_column <= 1000);
            if (min_in_row > -1 && min_in_column > -1 && i!=j && max_rank_null <= (min_in_row + min_in_column)) {
                max_rank_null = min_in_row + min_in_column;
                max_i = i;
                max_j = j;
            }
        }
    }
    smallesVertices[num][0] = max_i;
    smallesVertices[num][1] = max_j;

    for (int i = 0; i < verticesCount; i++) {
        roadsForComputing[max_i][i] = -1;
        roadsForComputing[i][max_j] = -1;
    }
    roadsForComputing[max_j][max_i] = -1;
}

void MainWindow::hideAdjacencyMatrix() {  //Скрывание матрицы смежности 9*9
    QString message;
    for (int i = 1; i < 10; i++) {
        message = "column" + QString::number(i); //Столбцы
        this->findChild<QWidget*>(message)->hide();

        message = "row" + QString::number(i); //Строки
        this->findChild<QWidget*>(message)->hide();

        message = "minus" + QString::number(i); //Перегородка
        this->findChild<QWidget*>(message)->hide();

        for (int j = i + 1; j < 10; j++) { //Дороги справа от перегородки
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(message)->hide();
        }
        for (int j = 1; j < i; j++) { //Дороги слева от перегородки
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(message)->hide();
        }
    }
}

void MainWindow::showAdjacencyMatrix() { //Показывание матрицы смежности count*count
    hideAdjacencyMatrix();    //скрываем старую, перед отрисовкой новой
    QString message;
    for (int i = 1; i <= verticesCount; i++) {
        message = "column" + QString::number(i);
        this->findChild<QWidget*>(message)->show();
        this->findChild<QLineEdit*>(message);
    }
    for (int i = 1; i <= verticesCount; i++) {
        message = "row" + QString::number(i);
        this->findChild<QWidget*>(message)->show();
        this->findChild<QLineEdit*>(message);
        message = "minus" + QString::number(i);
        this->findChild<QWidget*>(message)->show();
        this->findChild<QLineEdit*>(message);
        for (int j = i + 1; j <= verticesCount; j++) {
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(message)->show();
            this->findChild<QLineEdit*>(message);
        }
        for (int j = 1; j < i; j++) {
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QWidget*>(message)->show();
            this->findChild<QLineEdit*>(message);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *) { //Функция отрисовки графа полностью
    if (!currentStage) {
        return;
    }
    int vetexNumberIndent;
    circusGrad /= verticesCount;
    int distanceBetweenCircles;
    distanceBetweenCircles = circusGrad;
    circusGrad = 0;
    QPainter paint(this);
    QPen pen;
    pen.setColor(Qt::black);
    paint.setPen(pen);
    for (int i = 0; i < verticesCount; i++) {
        vetexNumberIndent = -10;
        if (circusGrad > 180) {
            vetexNumberIndent = 130;
        }
        coord[i].xCoordinate = 800 + (200 * cos(circusGrad / 57));
        coord[i].yCoordinate = 359 + (200 * -sin(circusGrad / 57));
        paint.setFont(QFont("Arial",32));

        circusGrad += distanceBetweenCircles;
        if (i == deletedVertices1 - 1 || i == deletedVertices2 - 1) {   //Если вершина была удалена, то её не рисуем
            continue;
        }
        paint.drawEllipse(coord[i].xCoordinate, coord[i].yCoordinate, 100, 100); //Рисуем вершины
        int x1 = coord[i].xCoordinate + 35;
        int y1 = coord[i].yCoordinate + 65;// + vetexNumberIndent + 35;
        paint.drawText(x1, y1, QString::number(i + 1)); //Нумерация вершин

    }
    if (currentStage <= 1) {
        return;
    } //Если была нажата кнопка "построить" или "вычислить"
    for (int i = 0; i < verticesCount; i++) {
        for (int j = 0; j < verticesCount; j++) {
            bool isValid = roadsForComputing[i][j] < 1000 && roadsForComputing[i][j] > 0;
            bool areVerticesDelteted = (i != deletedVertices1 - 1 && j != deletedVertices1 - 1 && i!= deletedVertices2 - 1 && j!= deletedVertices2 - 1);
            if (!(isValid && areVerticesDelteted)) {
                continue;
            }
            int x1 = coord[i].xCoordinate + 50;
            int y1 = coord[i].yCoordinate + 50;
            int x2 = coord[j].xCoordinate + 50;
            int y2 = coord[j].yCoordinate + 50;
            paint.drawLine(x1, y1, x2, y2); //Рисуем дороги
        }
    }

    if (currentStage != 3) {   //Если была нажата кнопка "вычислить"
        return;
    }
    int reg = 0;    //Корректировка по количеству удаленных вершин
    if (deletedVertices1 < 10) {
        reg++;
    }
    if (deletedVertices2 < 10) {
        reg++;
    }
    //pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    pen.setColor(Qt::red);
    paint.setPen(pen);
    for (int i = 1; i <= verticesCount - reg; i++) {
        int x1 = coord[bestWayList[i-1]].xCoordinate + 50;
        int y1 = coord[bestWayList[i-1]].yCoordinate + 50;
        int x2 = coord[bestWayList[i]].xCoordinate + 50;
        int y2 = coord[bestWayList[i]].yCoordinate + 50;
        paint.drawLine(x1, y1, x2, y2);
        //Рисуем оптимальный путь в соответствии с количеством удаленных вершин
    }
    paint.end();
}

void MainWindow::on_clear_clicked() { //Кнопка "Очистить" - полностью чистит таблицу и удаляет все дороги у графа, оставляя только вершины
    QString message;
    for (int i = 1; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText("");
            this->findChild<QLineEdit*>(message);
        }
        for(int j = 1; j < i; j++) {
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText("");
            this->findChild<QLineEdit*>(message);
        }
    }
    ui->label_2->hide();
    deletedVertices1 = 12;
    deletedVertices2 = 12;
    deletedVerticesCount = 1;
    ui->create->click();
}

void MainWindow::on_fill_clicked() {
    rangeToFillAdjacencyMatrix = ui->fillRange->toPlainText().toInt();
    QString message;
    int randomValue;
    for (int i = 1; i <= verticesCount; i++) {
        for (int j = i + 1; j <= verticesCount; j++) {
            randomValue = rand() % rangeToFillAdjacencyMatrix;
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText(QString::number(randomValue));
        }
        for (int j = 1; j < i; j++) {
            randomValue = rand() % rangeToFillAdjacencyMatrix;
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText(QString::number(randomValue));
        }
    }
}

void MainWindow::on_del_clicked() { //Кнопка "Удаления вершину" - удаляет заданную вершину
    vertexToDelete = ui->vertexToDeleteInputBox->toPlainText().toInt();
    if (deletedVerticesCount == 2) {//Если удаляем уже вторую вершины
        deletedVertices2 = deletedVertices1;
    }
    QString message;
    deletedVertices1 = vertexToDelete;
    for (int i = 1; i <= verticesCount; i++) {
        for (int j = i + 1; j <= verticesCount; j++) {
            if (i != deletedVertices1 && j != deletedVertices1) {
                continue;
            }
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText("");
            this->findChild<QLineEdit*>(message);
        }
        for (int j = 1; j < i; j++) {
            if (i != deletedVertices1 && j != deletedVertices1) {
                continue;
            }
            message = "road" + QString::number(i) + QString::number(j);
            this->findChild<QLineEdit*>(message)->setText("");
            this->findChild<QLineEdit*>(message);
        }
    }
    this->repaint();
    ui->build->click();     //Искусственно нажимаем кнопку "Построить" для того, чтобы после удаления вершины граф сразу перестроился
    deletedVerticesCount++;
}
