#include "smatrimage.h"

#include <string>
#include <memory>
using namespace std;

SmartImage::SmartImage(QString filename)
{
    QFileInfo fileinfo(filename);
    baseName = fileinfo.absolutePath()+"/"+fileinfo.baseName();
    img = make_unique<QImage>(filename);
    height = img->height();
    width = img->width();
    normalize();
}

void SmartImage::normalize()
{
    data = make_unique<ImageMap>(height,width);
    for(int i=0; i<width; i++)
        for(int j=0; j<height; j++)
            data->setData(qGray(img->pixel(i,j))/255.,i,j);
}

void SmartImage::createPiramid(int countLayers, double sigma0, double sigmaFirst)
{
    piramid = make_unique<Piramid>(countLayers, sigma0, sigmaFirst, *data);
}

void SmartImage::saveNormalize()
{
    data->saveToFile ((baseName+"_normalize.jpg"));
}

void SmartImage::saveFiltered(const FilterKernel & filter)
{
     unique_ptr<ImageMap> imageMap = filter.apply(*data);
     imageMap->saveToFile (baseName+"_filter.jpg");
}


void SmartImage::savePiramid(int countLayers, double sigma0, double sigmaFirst)
{
    createPiramid(countLayers,sigma0, sigmaFirst);
    piramid->save(baseName+"_pyramid");
}

double SmartImage::L(int x, int y, double sigma)
{
    return piramid->L(x, y, sigma);
}
void SmartImage::setBorderType(char border)
{
    data->setBorderType(border);
}

void SmartImage::detect(char detectorName)
{
    unique_ptr<AbstractDetector> detector;
    switch (detectorName)
    {
        case 'm':  detector = make_unique<MoravekDetector>(data.get()); break;
        case 'h':
        default:
            detector = make_unique<HarrisDetector>(data.get());
    }
    detector->detect(4,1);
    detector->save(baseName+"_detector");
}
