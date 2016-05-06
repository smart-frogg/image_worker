#include "haf.h"
#include <iostream>

Haf::Haf(SmartImage *pattern)
{
    this->pattern = pattern;
}
MetaData Haf::search(int w,int h)
{
    //vector<Descriptor> *descs = pattern->getDescriptors();
    for(MetaData &m:data)
    {
        Descriptor *d1 = m.d;
        Descriptor *d2 = d1->getClothest();

        MetaData mNew;
        mNew.angle = m.angle - d1->getPoint()->orientation + d2->getPoint()->orientation;
        if (mNew.angle < 0) mNew.angle += 2*M_PI;
        double r = sqrt(m.x*m.x + m.y*m.y);
        mNew.scale = d2->getPoint()->sigma / d1->getPoint()->sigma;
        double r1 = mNew.scale * r;
        mNew.x = r1*cos(mNew.angle);
        mNew.y = r1*sin(mNew.angle);
        int scaleI = mNew.scale/SCALE_BC;
        int angleI = mNew.angle*ORIENTATION_BC/(2*M_PI);
        int xI = (mNew.x*COORDS_BC)/w;
        int yI = (mNew.y*COORDS_BC)/h;
        cout << scaleI << "=s " << angleI << "=a " << xI << "=x " << yI << "=y " <<endl;
        buskets[scaleI  ][angleI  ][xI  ][yI  ]++;
        buskets[scaleI  ][angleI  ][xI  ][yI+1]++;
        buskets[scaleI  ][angleI  ][xI+1][yI  ]++;
        buskets[scaleI  ][angleI  ][xI+1][yI+1]++;
        buskets[scaleI  ][angleI+1][xI  ][yI  ]++;
        buskets[scaleI  ][angleI+1][xI  ][yI+1]++;
        buskets[scaleI  ][angleI+1][xI+1][yI  ]++;
        buskets[scaleI  ][angleI+1][xI+1][yI+1]++;

        buskets[scaleI+1][angleI  ][xI  ][yI  ]++;
        buskets[scaleI+1][angleI  ][xI  ][yI+1]++;
        buskets[scaleI+1][angleI  ][xI+1][yI  ]++;
        buskets[scaleI+1][angleI  ][xI+1][yI+1]++;
        buskets[scaleI+1][angleI+1][xI  ][yI  ]++;
        buskets[scaleI+1][angleI+1][xI  ][yI+1]++;
        buskets[scaleI+1][angleI+1][xI+1][yI  ]++;
        buskets[scaleI+1][angleI+1][xI+1][yI+1]++;
    }
    int scale,angle,x,y;
    int max=0;
    for(int i1=0;i1<SCALE_BC;i1++)
        for(int i2=0;i2<ORIENTATION_BC;i2++)
            for(int i3=0;i3<COORDS_BC;i3++)
                for(int i4=0;i4<COORDS_BC;i4++)
                {
                    if(max < buskets[i1][i2][i3][i4])
                    {
                        max = buskets[i1][i2][i3][i4];
                        scale = i1;
                        angle = i2;
                        x = i3;
                        y = i4;
                    }
                }
    MetaData res;
    res.angle = angle*(2*M_PI)/ORIENTATION_BC;
    res.x = (x * w)/COORDS_BC;
    res.y = (y * h)/COORDS_BC;
    res.scale = scale*10./SCALE_BC;
    return res;

}


void Haf::calcCenter()
{
    vector<Descriptor> *descs = pattern->getDescriptors();
    double minX = pattern->getImageMap()->getWidth();
    double minY = pattern->getImageMap()->getHeight();
    double maxX = 0;
    double maxY = 0;
    for(Descriptor &d:(*descs))
    {
        if (!d.used) continue;
        if (d.getPoint()->x < minX)
            minX = d.getPoint()->x;
        if (d.getPoint()->x > maxX)
            maxX = d.getPoint()->x;
        if (d.getPoint()->y < minY)
            minY = d.getPoint()->y;
        if (d.getPoint()->y > maxY)
            maxY = d.getPoint()->y;
    }
    center.x=0;//(maxX-minX)/2;
    center.y=0;//(maxY-minY)/2;
    for(Descriptor &d:(*descs))
    {
        if (!d.used) continue;
        MetaData m;
        m.d = &d;
        m.x = d.getPoint()->x - center.x;
        m.y = d.getPoint()->y - center.y;
        m.angle = atan2(m.x,m.y);
        if (m.angle < 0) m.angle+=2*M_PI;
        m.scale = 1;
        data.push_back(m);
    }
}


