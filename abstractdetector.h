#ifndef ABSTRACTDETECTOR_H
#define ABSTRACTDETECTOR_H

#include "imagemap.h"
class Blob;
struct Point
{
    int x;
    int y;
    double normX;
    double normY;

    double laplasian;
    Blob *b = NULL;
    int scale = 1;
    int count = 0;
    bool isClone = false;
    double sigma = 0.5;
    double orientation = 0;
    Point (const Point &p);
    Point(){};
    Point(int x, int y);
    double destination (Point p) const;
    double destination (Blob p) const;
    double destination (int x, int y) const;
    double angle(int x, int y) const;
    void setSigma(double sigma);
    void setScale(double scale);
    int getRealX();
    int getRealY();
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
