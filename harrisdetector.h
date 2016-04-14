#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
#define BUSKETS_COUNT 8
class Histogram
{
public:
    double buskets[BUSKETS_COUNT];
    Histogram();
    void normalize();
    void crop();
    void put(double dX, double dY);
    double destination(Histogram hist);
};
class Descriptor
{
private:
    Point point;
    Histogram hists[4];
    SobelFilter *sobelFilter;
    Histogram calc(int startX, int startY, int size);
    Descriptor *clothest;
public:
    void calc(int histNum, int startX, int startY, int size);
    double destination(Descriptor descriptor);
    Descriptor(const Point &point, SobelFilter *sobelFilter);
    void findClothest(vector<Descriptor> *descriptors);
    Point *getPoint();
    Descriptor* getClothest();
};

class HarrisDetector : public AbstractDetector
{
protected:
    unique_ptr<SobelFilter> sobelFilter;
    double T;
    double k;
    int winR;
    vector<Descriptor> descriptors;
public:
    void calcDescriptors();
    vector<Descriptor> *getDescriptors();
    HarrisDetector(ImageMap *data);
    void configure(double T, double k, int winR);
    virtual void detect();
    void saveCompare(QString filename, ImageMap *data2);
};

#endif // HARRISDETECTOR_H
