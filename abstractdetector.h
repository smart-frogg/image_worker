#ifndef ABSTRACTDETECTOR_H
#define ABSTRACTDETECTOR_H

#include "ImageMap.h"
struct Point
{
    int x;
    int y;
    Point(int x, int y);
    double destination (Point p);
    double destination (int x, int y);
};

class AbstractDetector
{
protected:
    ImageMap *data;
    vector<Point> points;
public:
    AbstractDetector(ImageMap *data);
    void save(QString filename);
    void clear(int count);
    virtual void detect() = 0;
};

#endif // ABSTRACTDETECTOR_H
