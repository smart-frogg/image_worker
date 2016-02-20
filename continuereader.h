#ifndef CONTINUEREADER_H
#define CONTINUEREADER_H
#include "imagereader.h"

class ContinueReader : public ImageReader
{
public:
    ContinueReader();
    double virtual get(int x, int y);
    unique_ptr<ImageReader> virtual copy();
};

#endif // CONTINUEREADER_H
