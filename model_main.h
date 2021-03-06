#ifndef MODEL_MAIN_H
#define MODEL_MAIN_H

#include <vector>
#include <polygon.h>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QPainter>
#include <QFile>
#include <ctime>
#include <fstream>
#include <string>
#include <stdio.h>

namespace Ui {
class Model_main;
}

class Model_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Model_main(QWidget *parent = 0);
    ~Model_main();


private slots:
    void on_pushButton_load_clicked();

    void on_horizontalSlider_threshold_actionTriggered();

    void on_spinBox_threshold_valueChanged(const QString &arg1);

    void on_spinBox_valueChanged(const QString &arg1);

    void on_processButton_clicked();

    void on_pushButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::Model_main *ui;

    const int MAX_R = 256;

//    std::vector<polygon> polyVector;
    QImage img;
    QImage grid;
    QImage res;
    double SKO;
    int P;
    // The y value, from which we must start our finding.
    int maxY;
    int timeDiff[256];
    std::vector<std::vector<polygon>*> renderVector;

//    void split(int x0, int y0, int R, int P);
//    void draw();
//    bool reqSplit(int x0, int y0, int R, int P);
    void process();
    int getIntensityFromPic(int x0, int y0, int R);
//    void formNewPic();
    int insidePolygon(int x, int y, std::vector<polygon> &polyVector);
    std::vector<polygon> *finder();
    int checker(int x0, int y0,
                 std::vector<polygon> &polyVector);
    void render();

protected:

public slots:


signals:

};

#endif // MODEL_MAIN_H
