#include "goodtrans.h"

GoodTrans::GoodTrans(double treshold)
{
this->treshold = treshold;
}

QTransform GoodTrans::normalize(vector<Descriptor> *desc)
{
    double maxX = -1;
    double maxY = -1;
    double minX = -1;
    double minY = -1;
    for (Descriptor &d:*desc)
    {
        if (minX==-1 || minX>d.getPoint()->x)
            minX=d.getPoint()->x;
        if (minY==-1 || minY>d.getPoint()->y)
            minY=d.getPoint()->y;
        if (maxX<d.getPoint()->x)
            maxX=d.getPoint()->x;
        if (maxY<d.getPoint()->y)
            maxY=d.getPoint()->y;
    }
    for (Descriptor &d:*desc)
    {
        d.getPoint()->normX = 2*(d.getPoint()->x - minX)/(maxX - minX) - 1;
        d.getPoint()->normY = 2*(d.getPoint()->y - minY)/(maxY - minY) - 1;
    }
    QTransform transform;
    transform.translate(-minX, -minY);
    transform.scale(2/(maxX - minX), 2/(maxY - minY));
    transform.translate(-1, -1);
    return transform;
}

int  GoodTrans::getRandomPoint()
{
     int n = rand() % descFrom->size();
     while (!(*descFrom)[n].used)
         n = rand() % descFrom->size();
     return n;
}
