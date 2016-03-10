#ifndef MORAVEKDETECTOR_H
#define MORAVEKDETECTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
class MoravekDetector : public AbstractDetector
{
    unique_ptr<SobelFilter> sobelFilter;
public:
    MoravekDetector(ImageMap *data);
    virtual void detect(double T, int winR);
};

#endif // MORAVEKDETECTOR_H
