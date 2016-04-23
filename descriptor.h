#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
#define BUSKETS_COUNT 8
#define BIG_BUSKETS_COUNT 36
#define HISTS_COUNT 4

class Histogram
{
private:
    int size;
    void clearBuskets();
    unique_ptr<double[]> buskets;
    int firstMax = -1;
    int secondMax = -1;
public:
    Histogram();
    Histogram(int size);
    void normalize();
    void crop();
    void put(double dX, double phi);
    double destination(const Histogram &hist) const;
    void calcMax(double t);
    double getFirstMax();
    double getSecondMax();

};
class Descriptor
{
private:
    Point point;
    Histogram hists[HISTS_COUNT][HISTS_COUNT];
    SobelFilter *sobelFilter;
    Histogram calc(int startX, int startY, int size);
    Descriptor *clothest;
public:
    bool used = true;
    void calc(int histNum, int startX, int startY, int size);
    double destination(const Descriptor &descriptor) const;
    Descriptor(const Point &point, int winSize, SobelFilter *sobelFilter);
    void findClothest(vector<Descriptor> *descriptors);
    Point *getPoint();
    Descriptor* getClothest();
};

#endif // DESCRIPTOR_H
