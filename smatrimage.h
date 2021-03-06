#ifndef SMATRIMAGE_H
#define SMATRIMAGE_H

#include <QObject>
#include <memory>
#include "imagemap.h"
#include "piramid.h"
#include "harrisdetector.h"
#include "moravekdetector.h"
#include "blobdetector.h"
using namespace std;

class SmartImage
{
private:
    QString baseName;
    int height;
    int width;
    unique_ptr<ImageMap> data;
    unique_ptr<Piramid> piramid;
    void normalize();
    void createPiramid(int countLayers, double sigma0, double sigmaFirst);
    unique_ptr<HarrisDetector> harrisDetector;
    vector<Descriptor> *descriptors;
public:
    void compare(SmartImage *img);
    void search(SmartImage *img);
    unique_ptr<QImage> img;
    void bind(SmartImage *img);
    SmartImage(QString filename);
    double L(int x, int y, double sigma);
    void saveNormalize();
    void setBorderType(char border);
    void saveFiltered(FilterKernel *filter);
    void testDetect(char detectorName);
    void savePiramid(int countLayers,double sigma0, double sigmaFirst);
    void detect(char detectorName);
    void genDescriptors();
    void genDescriptorsPiramid();
    vector<Descriptor> *getDescriptors();
    ImageMap* getImageMap();
    void saveCompare(ImageMap *data2);
};

#endif // SMATRIMAGE_H
