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
    unique_ptr<ImageMap> virtual apply(const ImageMap &data);
    ImageMap *getImageX();
    ImageMap *getImageY();
    double getM(int x, int y);
    double getAlpha(int x, int y);

};

#endif // SOBELFILTER_H
