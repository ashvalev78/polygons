#include "model_main.h"
#include "ui_model_main.h"

Model_main::Model_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Model_main)
{
    ui->setupUi(this);

    img.load(":/resouces/img/spichki.bmp");
    img = img.scaled(MAX_R, MAX_R);
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }
    ui->label_pic_src->setPixmap(QPixmap::fromImage(img));


    grid = res = QImage(MAX_R, MAX_R, QImage::Format_Grayscale8);
    grid.fill(qRgb(255,255,255));
    res.fill(qRgb(255, 255, 255));
    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
}

Model_main::~Model_main()
{
    delete ui;
}

void Model_main::on_pushButton_load_clicked()
{
//    QDir::homePath();
    QString imgFile = QFileDialog::getOpenFileName(this, tr("Открытие изображения"), "/Users/artemkaloev/faxmepic", tr("Image Files (*.gif *.tiff *.png *.jpg *.jpeg *.bmp)"));
    if (imgFile.isNull()) return;
    img.load(imgFile);

    img = img.scaled(MAX_R, MAX_R);
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }
    ui->label_pic_src->setPixmap(QPixmap::fromImage(img));
}

void Model_main::process(int P)
{
    //очистить вектор полигонов
    polyVector.clear();
    //создать сетку
//    polygon poly(0, 0, 256);
//    polyVector.push_back(poly);
//    poly.split(img, grid, polyVector, P);
    finder(P);
    draw();

//    не работает????
//    qDebug() << (*poly.LD).getX0();
    //отобразить сетку
    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    //отобразить количество полигонов
    ui->label_polyNum->setText(QString::number(polyVector.size()));
    //отобразить количество полигонов заданного размера
    on_spinBox_valueChanged(QString::number(ui->spinBox->value()));
    //формируем результирующее изображение и отображаем его
    formNewPic();
}

void Model_main::on_horizontalSlider_threshold_actionTriggered()
{
    grid.fill(qRgb(255, 255, 255));
    res.fill(qRgb(255, 255, 255));
    int P = ui->horizontalSlider_threshold->value();
    process(P);
    ui->spinBox_threshold->setValue(P);
}

void Model_main::on_spinBox_threshold_valueChanged(const QString &arg1)
{
    grid.fill(qRgb(255,255,255));
    res.fill(qRgb(255, 255, 255));
    int P = arg1.toInt();
    process(P);
    ui->horizontalSlider_threshold->setValue(P);
}

void Model_main::on_spinBox_valueChanged(const QString &arg1)
{
    int polySize = arg1.toInt();
    int counter = 0;
    for (int i = 0; i < polyVector.size(); i++) {
        if ((polyVector[i].getHeight() * polyVector[i].getWidth()) == polySize) {
            counter++;
        }
    }
    ui->label_reqPolyNum->setText(QString::number(counter));
}

int Model_main::checker(int x0, int y0, int growX, int growY, int P,
             std::vector<polygon> &polyVector,
             int &maxIntensity, int &minIntensity) {
    if ((x0 + growX) < 256 && (y0 + growY) < 256) {
        // If we have no borders
        for (int x = (x0 + growX); x > x0; x--) {
            for (int y = y0; y < (y0 + growY); y++) {
                int intensity = qGray(img.pixel(x, y));
                if (intensity < minIntensity) minIntensity = intensity;
                if (intensity > maxIntensity) maxIntensity = intensity;

                if ((maxIntensity - minIntensity) >= P) {
                    if (y != (y0 + growY - 1)) {
                        polygon A(x0, y0, growX, (growY - 1));
                        polyVector.push_back(A);
                    } else {
                        polygon A(x0, y0, growX, growY);
                        polyVector.push_back(A);
                    }
                    maxIntensity = 0;
                    minIntensity = 255;
                    return (y0 + growY);
                }
            }
        }
    }
    //bottom border
    if (insidePolygon(x0, (y0 + growY + 1)) || (y0 + growY + 1) > 255) {
        int q = checker(x0, y0, (growX + 1), growY, P, &polyVector, &maxIntensity, &minIntensity);
    } else if ((x0 + growX + 1) > 255) { // right border
        int q = checker(x0, y0, growX, (growY + 1), P, &polyVector, &maxIntensity, &minIntensity);
    } else { // no borders
        int q = checker(x0, y0, (growX + 1), (growY + 1), P, &polyVector, &maxIntensity, &minIntensity);
    }

    return (y0 + growY);
}

void  Model_main::finder(int P) {
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256;) {
            int height = insidePolygon(x,y);
            if (height) {
                y += height;
            } else {
                y += checker(x, y, 0, 0, P, &polyVector, 0, 255);
            }
        }
    }
}

//void Model_main::split(int x0, int y0, int R, int P)
//{
//    polygon poly(x0, y0, R);
//    if (poly.reqSplit(img, P)) {
//        poly.isEmpty = false;
//        poly.draw(grid);
//        for (int x = x0; x <= x0 + R/2; x += R/2) {
//            for (int y = y0; y <= y0 + R/2; y += R/2) {
//                split(x, y, R/2, P);
//            }
//        }
//    } else {
//        poly.setIntensity(poly.getIntensityFromPic(img));
//    }
//    polyVector.push_back(poly);
//}



void Model_main::draw()
{
    for (int i = 0; i < polyVector.size(); i++) {
        // Top
        for (int j = polyVector[i].getX0(); j < (polyVector[i].getWidth() + polyVector[i].getX0()); j++) {
            grid.setPixel(j, polyVector[i].getY0(), qRgb(0, 0, 0));
        }
        // Bottom
        for (int j = polyVector[i].getX0(); j < (polyVector[i].getWidth() + polyVector[i].getX0()); j++) {
            grid.setPixel(j, (polyVector[i].getY0() + polyVector[i].getHeight()), qRgb(0, 0, 0));
        }
        // Left
        for (int j = polyVector[i].getY0(); j < (polyVector[i].getHeight() + polyVector[i].getY0()); j++) {
            grid.setPixel(j, polyVector[i].getX0(), qRgb(0, 0, 0));
        }
        // Right
        for (int j = polyVector[i].getY0(); j < (polyVector[i].getHeight() + polyVector[i].getY0()); j++) {
            grid.setPixel(j, (polyVector[i].getX0() + polyVector[i].getWidth()), qRgb(0, 0, 0));
        }
    }
}



int Model_main::insidePolygon(int x, int y) {
    for (int i = 0; i < polyVector.size(); i++) {
        if (polyVector[i].pixelInsidePolygon(x, y)) return polyVector[i].getHeight();
    }
    return 0;
}

void Model_main::formNewPic()
{
//    qDebug() << polyVector[polyVector.size() - 1].getR() << polyVector[polyVector.size() - 2].getR() << polyVector[polyVector.size() - 3].getR() << polyVector[polyVector.size() - 4].getR();
    for (int k = 0; k < polyVector.size(); k++) {
        int x0 = polyVector[k].getX0();
        int y0 = polyVector[k].getY0();
        int intensity = polyVector[k].getIntensity();
        for (int i = x0; i < x0 + polyVector[k].getWidth(); i++) {
            for (int j = y0; j < y0 + polyVector[k].getHeight(); j++) {
                res.setPixel(i, j, qRgb(intensity, intensity, intensity));
            }
        }
    }
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
}
