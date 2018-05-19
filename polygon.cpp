#include "polygon.h"

polygon::polygon(int x, int y, int width, int height, int intens)
{
    x0 = x;
    y0 = y;
    polyHeight = height;
    polyWidth = width;
    intensity = intens;
}

int polygon::getIntensityFromPic(const QImage &img) {
//    int intensity = 0;
//    for (int i = x0; i < x0 + R; i++) {
//        for (int j = y0; j < y0 + R; j++) {
//            intensity += qGray(img.pixel(i, j));
//        }
//    }
//    return intensity /= R * R;
}


int polygon::pixelInsidePolygon(int x, int y) {
    if ((x0 <= x && (x0 + polyWidth - 1) >= x)
            &&
            (y0 <= y && (y0 + polyHeight - 1) >= y)) {
        return polyHeight;
    } else {
        return 0;
    }
}




//void polygon::formNewPic(const std::vector<polygon> &polyVector, QImage &res)
//{
//    qDebug() << polyVector[polyVector.size() - 1].getR() << polyVector[polyVector.size() - 2].getR() << polyVector[polyVector.size() - 3].getR() << polyVector[polyVector.size() - 4].getR();
//    for (int k = 0; k < polyVector.size(); k++) {
//        if (polyVector[k].isEmpty) {
//            int px0 = polyVector[k].getX0();
//            int py0 = polyVector[k].getY0();
//            int pR = polyVector[k].getR();
//            int pintensity = polyVector[k].getIntensity();
//            for (int i = px0; i < px0 + pR; i++) {
//                for (int j = py0; j < py0 + pR; j++) {
//                    res.setPixel(i, j, qRgb(pintensity, pintensity, pintensity));
//                }
//            }
//        }
//    }
//    ui->label_pic_res->setPixmap(QPixmap::fromImage(res));
//}




