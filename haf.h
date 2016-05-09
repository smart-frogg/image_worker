#ifndef HAF_H
#define HAF_H
#include "smatrimage.h"
#include <map>

#define ORIENTATION_BC 20
#define SCALE_BC 10
#define COORDS_BC 10

struct Index {
    int xI;
    int yI;
    int aI;
    int sI;
    bool operator< (const Index &p) const ;
};

struct MetaData {

    double x;
    double y;
    double angle;
    double scale;
    Descriptor *d;
    vector <Descriptor *> desc;
};

class Haf
{
private:
    SmartImage *pattern;
    Point center;
    vector<MetaData> data;
    vector<MetaData> transData;
    map<Index, vector<MetaData>> buskets;
    //int buskets[SCALE_BC][ORIENTATION_BC][COORDS_BC][COORDS_BC]; // scale angle x y
public:

    void calcCenter();
    Haf(SmartImage *pattern);
    vector<MetaData> search(int w, int h);
};

#endif // HAF_H
