#ifndef ABSTRACTDETECTOR_H
#define ABSTRACTDETECTOR_H

#include "ImageMap.h"
struct Point
{
    int x;
    int y;
    Point (const Point &p);
    Point(){};
    Point(int x, int y);
    double destination (Point p);
    double destination (int x, int y);
};

class AbstractDetector
{
protected:
    ImageMap *data;
    vector<Point> points;
    bool isClear = false;
    unique_ptr<bool[]> usingPoints;
public:
    AbstractDetector(ImageMap *data);
    vector<Point> getPoints();
    void save(QString filename);
    void clear(int count);
    virtual void detect() = 0;
};

#endif // ABSTRACTDETECTOR_H
