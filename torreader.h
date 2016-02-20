#ifndef TORREADER_H
#define TORREADER_H
#include "imagereader.h"

class TorReader : public ImageReader
{
public:
    TorReader();
    double virtual get(int x, int y);
    unique_ptr<ImageReader> virtual copy();
};

#endif // TORREADER_H
