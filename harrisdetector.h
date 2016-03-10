#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
class HarrisDetector : public AbstractDetector
{
protected:
    unique_ptr<SobelFilter> sobelFilter;
public:
    HarrisDetector(ImageMap *data);
    virtual void detect(double T, int winR);
};

#endif // HARRISDETECTOR_H
