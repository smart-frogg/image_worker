#include "abstractdetector.h"
#include <QPainter>
#include <iostream>

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
    painter->setPen(Qt::blue);
    for(Point p:points)
    {
        painter->drawEllipse(p.x-2,p.y-2,5,5);
        //image->setPixel(p.x,p.y,qRgb(255,0,0));
    }
    image->save(filename,"JPG", 100);
}

void AbstractDetector::clear(int count)
{
    if (points.size()<=count) return;
    int height = data->getHeight();
    int width = data->getWidth();
    int rMax = height > width ? width : height;
    vector<Point> res;
    for(int r=0; r<rMax; r++)
    {
        res.clear();
        for(Point p:points)
        {
            bool isMax = true;
            double cur = data->getData(p.x,p.y);
            for(Point pi:points) if (p.destination(pi)<r*r)
            {
                isMax &= cur > 0.9*data->getData(pi.x,pi.y);
            }
            if (isMax)
            {
               res.push_back(p);
            }
        }
     //   cout<<r<<" "<<res.size()<<endl;
        if (res.size()<=count)
        {
            points = res;
            return;
        }
    }
}
