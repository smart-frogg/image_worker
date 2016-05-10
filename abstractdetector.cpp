#include "abstractdetector.h"
#include "blobdetector.h"
#include <QPainter>
#include <iostream>

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Point::Point (const Point &p)
{
    this->x = p.x;
    this->y = p.y;
    this->scale = p.scale;
    this->sigma = p.sigma;
    this->orientation = p.orientation;
    isClone = true;
}
void Point::setSigma(double sigma) {
    this->sigma = sigma;
}
int Point::getRealX()
{
    return x*(1<<scale);
}

int Point::getRealY()
{
    return y*(1<<scale);
}

void Point::setScale(double scale) {
    this->scale = scale;
}
double Point::angle(int x, int y) const
{
    double phi = atan2(y-this->y,x-this->x);
    if (phi<0) phi+=2*M_PI;
    phi-=orientation;
    if (phi<0) phi+=2*M_PI;
    return phi;
}

double Point::destination (Point p) const
{
    return (x*(1<<scale)-p.x*(1<<p.scale))*(x*(1<<scale)-p.x*(1<<p.scale)) + (y*(1<<scale)-p.y*(1<<p.scale))*(y*(1<<scale)-p.y*(1<<p.scale));
}
double Point::destination (Blob p) const
{
    return (x*(1<<scale)-p.x)*(x*(1<<scale)-p.x) + (y*(1<<scale)-p.y)*(y*(1<<scale)-p.y);
}

double Point::destination (int x, int y) const
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

    vector<Point> pts = isClear ? getPoints(): points;
    for(Point p:pts)
    {
        painter->drawEllipse(p.x-2,p.y-2,5,5);
        //image->setPixel(p.x,p.y,qRgb(255,0,0));
    }
    image->save(filename,"JPG", 100);
}

void AbstractDetector::clear(int count)
{
    isClear = true;
    usingPoints = make_unique<bool[]>(2*points.size());
    for(int i=0;i<2*points.size();i++)
        usingPoints[i] = false;
    if (points.size()<=count)
    {
        for(int i=0;i<points.size();i++)
            usingPoints[i] = true;
        return;
    }
    int height = data->getHeight();
    int width = data->getWidth();
    int rMax = height > width ? width : height;
    for(int r=0; r<rMax; r++)
    {
        int i=0;
        int resCount = 0;
        for(Point p:points)
        {
            bool isMax = true;
            double cur = data->getData(p.x,p.y);
            for(Point pi:points) if (p.destination(pi)<r*r)
            {
                isMax &= cur > 0.9*data->getData(pi.x,pi.y);
            }
            usingPoints[i] = isMax;
            resCount += usingPoints[i];
            i++;
        }
     //   cout<<r<<" "<<res.size()<<endl;
        if (resCount<=count)
        {
            return;
        }
    }
}
vector<Point> AbstractDetector::getPoints()
{
    vector<Point> activePoints;
    for(int i=0;i<points.size();i++)
    {
        if(usingPoints[i])
            activePoints.push_back(points[i]);
    }
    return activePoints;
}
