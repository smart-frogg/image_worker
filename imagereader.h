#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include "imagemap.h"

class ImageReader
{
protected:
    unique_ptr<ImageMap> *image;

public:
    ImageReader();
    double virtual get(int x, int y) = 0;
    unique_ptr<ImageReader> virtual copy() = 0;
    void setData(unique_ptr<ImageMap>* data);
    int getHeight();
    int getWidth();
};

#endif // IMAGEREADER_H
