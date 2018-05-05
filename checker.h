#ifndef CHECKER_H
#define CHECKER_H

#include <vector>
#include "polygon.h"

bool checker(int x0, int y0, int x, int y, int maxX, int maxY,
             std::vector<polygon> polyVector,
             int *maxIntensity, int *minIntensity) {
//    if (x < 256 && y < 256) {
//        bool pixelIsFree = true;
//        for (int i = 0; i < polyVector.size(); i++) {
//            if (!polyVector[i].pixelInsidePolygon(x, y)) {
//                pixelIsFree = false;
//            }
//        }
//        if (pixelIsFree) {
//            int intensity = qGray(img.pixel(x, y));
//            if (intensity < *minIntensity) *minIntensity = intensity;
//            if (intensity > *maxIntensity) *maxIntensity = intensity;

//            if ((*maxIntensity - *minIntensity) >= P) {
//                polygon A = new polygon(x0, y0, (maxY - y0), (maxX - x0));
//                polyVector.push_back(A);
//            }

////            if (((x - 1) >= x0) && (y + 1) >= y0) {
////                checker(x0, y0, (x - 1), (y + 1), maxX, maxY, polyVector, &maxIntensity, &minIntensity);
////            } else if (x == x0) {
////                checker(x0, y0, (maxX + 1), y0, (maxX + 1), maxY, polyVector, &maxIntensity, &minIntensity);
////            } else if (y == y0) {
////                checker(x0, y0, (x - 1), (y + 1), (maxX + 1), (y + 1), polyVector, &maxIntensity, &minIntensity);
////            }
//            if (x == x0) {
//                checker(x0, y0, (maxX + 1), y0, (maxX + 1), maxY, polyVector, &maxIntensity, &minIntensity);
//            } else if (y == maxY) {
//                checker(x0, y0, (x - 1), (y + 1), maxX, (maxY + 1), polyVector, &maxIntensity, &minIntensity);
//            } else if (x > x0 && y >= y0) {

//            }
//        }
//    }
}

#endif // CHECKER_H
