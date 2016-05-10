#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "abstractdetector.h"
#include "descriptor.h"


class HarrisDetector : public AbstractDetector
{
protected:
    unique_ptr<SobelFilter> sobelFilter;
    double T;
    double k;
    int winR;
    vector<Descriptor> descriptors;
public:
    void calcDescriptors(double sigma, int scale);
    void calcDescriptors(double sigma, double littleSigma, int scale, vector<Descriptor> *descriptors);
    Descriptor calcDescriptor(double littleSigma, Point *p);
    vector<Descriptor> *getDescriptors();
    HarrisDetector(ImageMap *data);
    void configure(double T, double k, int winR);
    virtual void detect();
    void calcDirection(Point &p);
};

#endif // HARRISDETECTOR_H
