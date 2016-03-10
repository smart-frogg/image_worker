#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
class HarrisDetector : public AbstractDetector
{
protected:
    unique_ptr<SobelFilter> sobelFilter;
    double T;
    double k;
    int winR;
public:
    HarrisDetector(ImageMap *data);
    void configure(double T, double k, int winR);
    virtual void detect();
};

#endif // HARRISDETECTOR_H
