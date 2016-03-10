#include "abstractdetector.h"
#include <QPainter>

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
    unique_ptr<QImage> image = this->data->asImage();
    unique_ptr<QPainter> painter = make_unique<QPainter>(image.get());
    painter->setBrush(Qt::green);
    painter->setPen(Qt::white);
    for(Point p:points)
    {
        painter->drawEllipse(p.x-3,p.y-3,7,7);
        //image->setPixel(p.x,p.y,qRgb(255,0,0));
    }
    image->save(filename,"JPG");
}
