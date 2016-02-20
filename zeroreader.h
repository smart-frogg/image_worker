#ifndef ZEROREADER_H
#define ZEROREADER_H
#include "imagereader.h"

class ZeroReader : public ImageReader
{
public:
    ZeroReader();
    double virtual get(int x, int y);
    unique_ptr<ImageReader> virtual copy();
};

#endif // ZEROREADER_H
