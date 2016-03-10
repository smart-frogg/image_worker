#ifndef SOBELFILTER_H
#define SOBELFILTER_H
#include "separablekernel.h"

class SobelFilter : public FilterKernel
{
private:
    unique_ptr<SeparableKernel> gx;
    unique_ptr<SeparableKernel> gy;
    unique_ptr<ImageMap> imageX;
    unique_ptr<ImageMap> imageY;
public:
    SobelFilter();
    unique_ptr<ImageMap> virtual apply(const ImageMap &data) const;
    ImageMap *getImageX();
    ImageMap *getImageY();

};

#endif // SOBELFILTER_H
