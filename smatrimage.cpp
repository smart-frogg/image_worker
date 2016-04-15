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

void SmartImage::saveFiltered(FilterKernel *filter)
{
     unique_ptr<ImageMap> imageMap = filter->apply(*data);
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
        case 'm':
            detector = make_unique<MoravekDetector>(data.get());
            ((MoravekDetector*)detector.get())->configure(0.2,1);
            detector->detect();
            detector->save(baseName+"_MoravekDetector.jpg");
          break;
        case 'h':
        default:
            detector = make_unique<HarrisDetector>(data.get());
            ((HarrisDetector*)detector.get())->configure(0.02,0.06,1);
            detector->detect();
            detector->save(baseName+"_HarrisonDetector.jpg");
    }
}

void SmartImage::genDescriptors()
{
    harrisDetector = make_unique<HarrisDetector>(data.get());
    harrisDetector.get()->configure(0.02,0.06,1);
    harrisDetector->detect();
    harrisDetector->clear(40);
    harrisDetector->calcDescriptors(0.5);
//    harrisDetector->save(baseName+"_HarrisonDetector.jpg");
}

void SmartImage::genDescriptorsPiramid()
{
    createPiramid(10,0.5,1.);
    piramid->genDescriptors();
}

vector<Descriptor> *SmartImage::getDescriptors()
{
    return harrisDetector->getDescriptors();
}

void SmartImage::compare(SmartImage *img)
{
    genDescriptors();
    img->genDescriptors();
    vector<Descriptor> *descriptors1 = getDescriptors();
    vector<Descriptor> *descriptors2 = img->getDescriptors();
    for(int i=0,n=descriptors1->size();i<n;i++)
        (*descriptors1)[i].findClothest(descriptors2);
    harrisDetector->saveCompare(baseName+"_compare.jpg",img->getImageMap());
}

ImageMap* SmartImage::getImageMap()
{
    return data.get();
}

void SmartImage::testDetect(char detectorName)
{
    unique_ptr<AbstractDetector> detector;
    unique_ptr<ImageMap> maps[7];
    for(int i=0;i<7;i++)
        maps[i] = data->copy();
    maps[1]->brightness(0.8);
    maps[2]->brightness(1.2);
    maps[3]->kontrast(0.1);
    maps[4]->kontrast(1.1);
    maps[5]->saltAndPepper(0.8);
    maps[6]->saltAndPepper(0.5);
    char s[2];
    s[1]=0;
    switch (detectorName)
    {
        case 'm':
            for(int i=0;i<7;i++)
            {
                s[0]=i+'0';
                detector = make_unique<MoravekDetector>(maps[i].get());
                ((MoravekDetector*)detector.get())->configure(0.2,1);
                detector->detect();
                detector->save(baseName+"_MoravekDetector"+s+".jpg");
                detector->clear(50);
                detector->save(baseName+"_MoravekDetector"+s+"clear.jpg");
            }
          break;
        case 'h':
        default:
            for(int i=0;i<7;i++)
            {
                s[0]=i+'0';
                detector = make_unique<HarrisDetector>(maps[i].get());
                ((HarrisDetector*)detector.get())->configure(0.02,0.06,1);
                detector->detect();
                detector->save(baseName+"_HarrisonDetector"+s+".jpg");
                detector->clear(50);
                detector->save(baseName+"_HarrisonDetector"+s+"clear.jpg");
            }
    }
}


