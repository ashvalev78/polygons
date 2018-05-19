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

void Model_main::process()
{
    std::vector<polygon> polyVector = (*renderVector[P]);
    for (int i = 0; i < (*renderVector[P]).size(); i++) {

        int x0 = (*renderVector[P])[i].getX0();
        int y0 = (*renderVector[P])[i].getY0();
        int width = (*renderVector[P])[i].getWidth();
        int height = (*renderVector[P])[i].getHeight();

        QPainter painter(&grid);
        QPolygon p;
        p << QPoint(x0,y0) << QPoint(x0+width-1,y0) << QPoint(x0+width-1,y0+height-1) << QPoint(x0,y0+height-1);
        painter.drawPolygon(p);

        int intensity = polyVector[i].getIntensity();
        for (int k = x0; k < x0 + polyVector[i].getWidth(); k++) {
            for (int j = y0; j < y0 + polyVector[i].getHeight(); j++) {
                res.setPixel(k, j, qRgb(intensity, intensity, intensity));
            }
        }

    }

    ui->label_pic_grid->setPixmap(QPixmap::fromImage(grid));
    if (P == 0) grid.fill(qRgb(0, 0, 0));


//    std::vector<polygon> polyVector = *renderVector[P];
    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
//    draw();
//    не работает????
    //отобразить сетку
    //отобразить количество полигонов
    ui->label_polyNum->setText(QString::number(renderVector[P]->size()));
    //отобразить количество полигонов заданного размера
    on_spinBox_valueChanged(QString::number(ui->spinBox->value()));
    //формируем результирующее изображение и отображаем его
//    formNewPic();
   int summ = 0;
   for (int i = 0; i < 256; i++) {
       for (int j = 0; j < 256; j++) {
           summ += pow((qGray(img.pixel(i,j)) - qGray(res.pixel(i,j))),2);
       }
   }
   double result = sqrt(summ/(256*256));

   ui->label_SKO->setText(QString::number(result));
}

void Model_main::on_horizontalSlider_threshold_actionTriggered()
{
    grid.fill(qRgb(255, 255, 255));
    res.fill(qRgb(255, 255, 255));
    P = ui->horizontalSlider_threshold->value();
    process();
    if (P == 0) {
        grid.fill(qRgb(0,0,0));
    }

    ui->spinBox_threshold->setValue(P);
}

void Model_main::on_spinBox_threshold_valueChanged(const QString &arg1)
{
    grid.fill(qRgb(255,255,255));
    res.fill(qRgb(255, 255, 255));
    P = arg1.toInt();
    process();
    if (P == 0) {
        grid.fill(qRgb(0,0,0));
    }
    ui->horizontalSlider_threshold->setValue(P);
}

void Model_main::on_spinBox_valueChanged(const QString &arg1)
{
    std::vector<polygon> polyVector = *renderVector[P];
    int polySize = arg1.toInt();
    int counter = 0;
    for (int i = 0; i < polyVector.size(); i++) {
        if ((polyVector[i].getHeight() * polyVector[i].getWidth()) == polySize) {
            counter++;
        }
    }
    ui->label_reqPolyNum->setText(QString::number(counter));
}

void Model_main::render() {
    for (int i = 0; i < renderVector.size(); i++) {
        delete renderVector[i];
    }
    renderVector.clear();
    for (int i = 0; i < 256; i++) {
        std::vector<polygon> *A = new std::vector<polygon>;
        renderVector.push_back(A);
    }
    for (int i = 0; i < 256; i++) {
        P = i;
        renderVector[i] = finder();
    }
}

int Model_main::checker(int x0, int y0,
             std::vector<polygon> &polyVector) {

    int x = x0, y= y0;
    int maxIntensity = 0, minIntensity = 255;
    int maxX = 255; int maxY = y0;
    while(maxY<255 && !insidePolygon(x0, maxY+1, polyVector)) maxY++;
    int summ = qGray(img.pixel(x0, y0)), count = 1;
    bool progress = true;
    while(progress)
    {
        progress = false;
        if(x < maxX){
            for(int t = 0; y0+t <= y; t++)
            {
                int intensity = qGray(img.pixel(x, y0+t));
                summ+=intensity; count++;
                if (intensity < minIntensity) minIntensity = intensity;
                if (intensity > maxIntensity) maxIntensity = intensity;

                if ((maxIntensity - minIntensity) >= P) {
                    polyVector.push_back(polygon(x0, y0, x-x0+1, y-y0+1, summ/count));
                    return y-y0+1;
                }
             }
            x++; progress++;
        }
        if(y < maxY){
            for(int t = 0; x0+t <= x; t++)
            {
                int intensity = qGray(img.pixel(x0+t, y));
                summ+=intensity; count++;
                if (intensity < minIntensity) minIntensity = intensity;
                if (intensity > maxIntensity) maxIntensity = intensity;

                if ((maxIntensity - minIntensity) >= P) {
                    polyVector.push_back(polygon(x0, y0, x-x0+1, y-y0+1, summ/count));
                    return y-y0+1;
                }
             }
            y++; progress++;
        }
    }
    polyVector.push_back(polygon(x0, y0, x-x0+1, y-y0+1, summ/count));
    return y-y0+1;
}

std::vector<polygon>* Model_main::finder() {
    timeDiff[P] = clock();
    std::vector<polygon> *polyVector = new std::vector<polygon>;
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256;) {

            int height = insidePolygon(x,y, *polyVector);
            if (height) {
                y += height;
            } else {
                y += checker(x, y, *polyVector);
            }
        }
    }
    timeDiff[P] = clock() - timeDiff[P];
    return polyVector;
}

int Model_main::insidePolygon(int x, int y, std::vector<polygon> &polyVector) {
//    if (P == 0) return 0;
//    if (P < 25) return 0;
//    if (polyVector.size() % 100 == 0)
//        qDebug() << polyVector.size();

    for (int i = 0; i < polyVector.size(); i++) {
                if (polyVector[i].pixelInsidePolygon(x, y)) {
                    return polyVector[i].getHeight();
                }
    }
    return 0;
}

void Model_main::on_processButton_clicked()
{
    render();
    process();
}

void Model_main::on_pushButton_clicked()
{
    std::vector<polygon> *polyVector = new std::vector<polygon>;
    polyVector = renderVector[P];
    std::ofstream compressFile("C:/qtProjects/files/compressFile.txt");
    for (int i = 0; i < polyVector->size(); i++) {
        compressFile << static_cast<char>((*polyVector)[i].getX0());
        compressFile << static_cast<char>((*polyVector)[i].getY0());
        compressFile << static_cast<char>((*polyVector)[i].getWidth() - 1);
        compressFile << static_cast<char>((*polyVector)[i].getHeight() - 1);
        compressFile << static_cast<char>((*polyVector)[i].getIntensity());
    }
    compressFile.close();
}



void Model_main::on_loadButton_clicked()
{
    img.fill(qRgb(255, 255, 255));
    std::ifstream compFile("C:/qtProjects/files/compressFile.txt");
//    std::string line;
//    if (compFile.open(QIODevice::ReadOnly)) {
//        line = compFile.readLine();
//    }
//    qDebug() << line;
    compFile.seekg(0, std::ios::end);
    int length = compFile.tellg();
    char line[length];
    compFile.seekg(0,std::ios::beg);
    compFile.read(line,length);
    int x0 = 0, y0 = 0, width = 0, height = 0, intensity = 0;
    int x = 1;
    for (int i = 0; i < length; i++) {
        int k = (unsigned char)(line[i]);
    }
//    std::ofstream compressFile("C:/qtProjects/files/compressFile.txt");
//    for (int i = 0; i < 1; i++) {
//        compressFile << line[0];
//        compressFile << line[1];
//        compressFile << line[2];
//        compressFile << line[3];
//        compressFile << line[4];
//    }
//    compressFile.close();
    for (int i = 0; i < 0; i++) {
//        if (x == 6) x = 1;
//        if (x == 1) x0 = int(line[i]);
//        if (x == 2) y0 = int(line[i]);
//        if (x == 3) width = int(line[i]) + 1;
//        if (x == 4) height = int(line[i]) + 1;
//        if (x == 5) intensity = int(line[i]);
//        qDebug() << int(line[i]);

        x++;

        for (int k = x0; k < x0 + width; k++) {
            for (int j = y0; j < y0 + height; j++) {
                img.setPixel(k, j, qRgb(intensity, intensity, intensity));
            }
        }
        ui->label_pic_src->setPixmap(QPixmap::fromImage(img));

//        if (char(ch) != '/n') {
////            qDebug() << char(ch);
//            qDebug() << ch;
//        }
    }
    compFile.close();
}
