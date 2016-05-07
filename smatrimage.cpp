#include "smatrimage.h"

#include <string>
#include <memory>
#include "transformer.h"
#include "haf.h"
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
    harrisDetector->clear(150);
    harrisDetector->calcDescriptors(0.5,1);
    descriptors = harrisDetector->getDescriptors();
//    harrisDetector->save(baseName+"_HarrisonDetector.jpg");
}

void SmartImage::genDescriptorsPiramid()
{
    createPiramid(6,0.5,1.);
    //piramid->save(baseName);
    //piramid->saveDOG(baseName);
    piramid->genDescriptors();
    piramid->genDOG();
    //    piramid->genBlobs();
    piramid->findGoodPoints();
    piramid->saveDescriptors(baseName);
    piramid->clear();

    descriptors = piramid->getDescriptors();
}

vector<Descriptor> *SmartImage::getDescriptors()
{
    return descriptors;
}

void SmartImage::compare(SmartImage *img)
{
    genDescriptorsPiramid();
    img->genDescriptorsPiramid();
    vector<Descriptor> *descriptors1 = getDescriptors();
    vector<Descriptor> *descriptors2 = img->getDescriptors();
    for(Descriptor &descriptor:*descriptors1)
        descriptor.findClothest(descriptors2);
    for(Descriptor &descriptor:*descriptors2)
        descriptor.findClothest(descriptors1);
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            descriptor.used = false;
    }
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            d->used = false;
    }
    saveCompare(img->getImageMap());
}
void SmartImage::bind(SmartImage *img)
{
    genDescriptorsPiramid();
    img->genDescriptorsPiramid();
    vector<Descriptor> *descriptors1 = getDescriptors();
    vector<Descriptor> *descriptors2 = img->getDescriptors();
    for(Descriptor &descriptor:*descriptors1)
        descriptor.findClothest(descriptors2);
    for(Descriptor &descriptor:*descriptors2)
        descriptor.findClothest(descriptors1);
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            descriptor.used = false;
    }
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            d->used = false;
    }
    saveCompare(img->getImageMap());
    ImageMap *data2 = img->getImageMap();
    Transformer t(0.007 * max(data2->getHeight(), data2->getWidth()));
    t.setDescs(descriptors1);
    QTransform transform = t.getTransform();
    QImage *image = this->img.get();
    QImage *image2 = img->img.get();
    unique_ptr<QImage> result = make_unique<QImage>(3*max(this->data->getWidth(),data2->getWidth()), 3*max(this->data->getHeight(),data2->getHeight()), QImage::Format_RGB32);
    result->fill(0);
    unique_ptr<QPainter> painter = make_unique<QPainter>(result.get());

    painter->drawImage(result->width()/3,result->height()/3,*image);
    QTransform trans(1,0,0,
                    0,1,0,
                    result->width()/3,result->height()/3,1);
    painter->setTransform(transform*trans);
    painter->drawImage(0,0,*image2);
    result->save(baseName+"_bind.jpg","JPG", 100);
 }

void SmartImage::search(SmartImage *img)
{
    genDescriptorsPiramid();
    img->genDescriptorsPiramid();
    vector<Descriptor> *descriptors1 = getDescriptors();
    vector<Descriptor> *descriptors2 = img->getDescriptors();
    for(Descriptor &descriptor:*descriptors1)
        descriptor.findClothest(descriptors2);
    for(Descriptor &descriptor:*descriptors2)
        descriptor.findClothest(descriptors1);
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            descriptor.used = false;
    }
    for(Descriptor &descriptor:*descriptors1)
    {
        Descriptor *d = descriptor.getClothest();
        if(d->getClothest()->getPoint()->destination(*(descriptor.getPoint()))>4)
            d->used = false;
    }
    saveCompare(img->getImageMap());
    ImageMap *data2 = img->getImageMap();
    Haf haf(this);
    haf.calcCenter();
    MetaData m = haf.search(data2->getWidth(),data2->getHeight());
    unique_ptr<QImage> image2 = data2->asImage();
    unique_ptr<QPainter> painter = make_unique<QPainter>(image2.get());
    painter->setPen(qRgb(rand()%255,rand()%255,rand()%255));
    painter->drawEllipse(m.x,m.y,10,10);
    image2->save(baseName+"_haf.jpg","JPG", 100);
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

void SmartImage::saveCompare(ImageMap *data2)
{
    descriptors = getDescriptors();
    unique_ptr<QImage> image = this->data->asImage();
    unique_ptr<QImage> image2 = data2->asImage();
    unique_ptr<QImage> result = make_unique<QImage>(
                this->data->getWidth()+data2->getWidth(),
                max(this->data->getHeight(),data2->getHeight()), QImage::Format_RGB32);
    unique_ptr<QPainter> painter = make_unique<QPainter>(result.get());
    painter->drawImage(0,0,*image);
    painter->drawImage(this->data->getWidth(),0,*image2);
    //painter->setBrush(Qt::green);

    for(Descriptor &d:*descriptors) if (d.used)
    {
        painter->setPen(qRgb(rand()%255,rand()%255,rand()%255));
        Point *p1 = d.getPoint();
        Point *p2 = d.getClothest()->getPoint();
        double scale1 = 1<<p1->scale;
        double scale2 = 1<<p2->scale;
        painter->drawLine(p1->x*scale1,p1->y*scale1,p2->x*scale2 + this->data->getWidth(),p2->y*scale2);
        double r = p1->sigma*sqrt(2)+1;
        painter->drawEllipse(p1->x*scale1-r,p1->y*scale1-r,2*r,2*r);
        r = p2->sigma*sqrt(2)+1;
        painter->drawEllipse(p2->x*scale2-r+ this->data->getWidth(),p2->y*scale2-r,2*r,2*r);
        //if(d->b != NULL)
        //    painter->drawEllipse(p2->b->x-p2->b->r+ this->data->getWidth(),p2->b->y-p2->b->r,p2->b->r,p2->b->r);

        //image->setPixel(p.x,p.y,qRgb(255,0,0));
    }
    result->save(baseName+"_compare.jpg","JPG", 100);
}
