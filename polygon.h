#ifndef POLYGON_H
#define POLYGON_H

#include <QImage>
#include <QDebug>
//#include <QPixmap>

class polygon
{
    friend class Model_main;

private:
    int x0, y0, polyHeight, polyWidth, // polygon start coordinates and its height & width
    intensity; // polygon intensivity
public:

    polygon(int x0, int y0, int width, int height, int intens);
    int getHeight() { return polyHeight; }
    int getWidth() { return polyWidth; }
    int getX0() { return x0; }
    int getY0() { return y0; }
    int getIntensity() { return intensity; }
    void setIntensity(int intns) { intensity = intns; }
    int getIntensityFromPic(const QImage &img);
    int pixelInsidePolygon(int x, int y);

//    void split(const QImage &img, QImage &grid, std::vector<polygon> &polyVector, int P);
//    void formNewPic(const std::vector<polygon> &polyVector, QImage &res);
};

#endif // POLYGON_H
