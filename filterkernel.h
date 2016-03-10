#ifndef FILTERKERNEL_H
#define FILTERKERNEL_H

#include "imagemap.h"
#include "imagereader.h"

class FilterKernel
{
protected:
    int height;
    int width;
public:
    FilterKernel(int height, int width);
    unique_ptr<ImageMap> virtual apply(const ImageMap &data) = 0 ;
};

#endif // FILTERKERNEL_H
