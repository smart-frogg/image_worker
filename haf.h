#ifndef HAF_H
#define HAF_H
#include "smatrimage.h"
#include <map>

#define ORIENTATION_BC 30
#define SCALE_BC 10
#define COORDS_BC 20
struct MetaData {
    double x;
    double y;
    double angle;
    double scale;
    Descriptor *d;
};

class Haf
{
private:
    SmartImage *pattern;
    Point center;
    vector<MetaData> data;
    int buskets[SCALE_BC][ORIENTATION_BC][COORDS_BC][COORDS_BC]; // scale angle x y
public:
    void calcCenter();
    Haf(SmartImage *pattern);
    MetaData search(int w, int h);
};

#endif // HAF_H