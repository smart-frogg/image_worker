#ifndef SOBELFILTER_H
#define SOBELFILTER_H
#include "separablekernel.h"

class SobelFilter : public FilterKernel
{
private:
    unique_ptr<SeparableKernel> gx;
    unique_ptr<SeparableKernel> gy;

public:
    SobelFilter(int height, int width);
    unique_ptr<ImageMap> virtual apply(const ImageMap &data) const;
};

#endif // SOBELFILTER_H
