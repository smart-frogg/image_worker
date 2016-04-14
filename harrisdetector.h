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
    void calcDescriptors();
    vector<Descriptor> *getDescriptors();
    HarrisDetector(ImageMap *data);
    void configure(double T, double k, int winR);
    virtual void detect();
    void saveCompare(QString filename, ImageMap *data2);
    void calcDirection(Point &p);
};

#endif // HARRISDETECTOR_H
