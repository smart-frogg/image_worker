#ifndef SMATRIMAGE_H
#define SMATRIMAGE_H

#include <QObject>
#include <memory>
#include "imagemap.h"
#include "piramid.h"
using namespace std;

class SmartImage
{
private:
    QString baseName;
    int height;
    int width;
    unique_ptr<QImage> img;
    unique_ptr<ImageMap> data;
    unique_ptr<Piramid> piramid;
    void normalize();
    void createPiramid(int countLayers, double sigma0, double sigmaFirst);

public:
    SmartImage(QString filename);
    double L(int x, int y, double sigma);
    void saveNormalize();
    void setBorderType(char border);
    void saveFiltered(const FilterKernel & filter);
    void savePiramid(int countLayers,double sigma0, double sigmaFirst);
};

#endif // SMATRIMAGE_H