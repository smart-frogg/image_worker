#ifndef SYMMETRYREADER_H
#define SYMMETRYREADER_H
#include "imagereader.h"

class SymmetryReader : public ImageReader
{
public:
    SymmetryReader();
    double virtual get(int x, int y);
    unique_ptr<ImageReader> virtual copy();
};

#endif // SYMMETRYREADER_H
