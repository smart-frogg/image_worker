#include "abstractdetector.h"

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

double Point::destination (Point p)
{
    return (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y);
}

double Point::destination (int x, int y)
{
    return (x-this->x)*(x-this->x) + (y-this->y)*(y-this->y);
}

AbstractDetector::AbstractDetector(ImageMap *data)
{
    this->data = data;
}

void AbstractDetector::save(QString filename)
{
    QImage *image = this->data->asImage().get();
    for(Point p:points)
    {
        image->setPixel(p.x,p.y,qRgb(0,255,0));
    }
    image->save(filename,"JPG");
}
