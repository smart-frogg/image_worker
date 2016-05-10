#include "haf.h"
#include <iostream>

Haf::Haf(SmartImage *pattern)
{
    this->pattern = pattern;
}
bool Index::operator< (const Index &p) const {
       if (sI != p.sI)
           return sI < p.sI;
       if (aI != p.aI)
           return aI < p.aI;
       if (xI != p.xI)
           return xI < p.xI;
       return yI < p.yI;
   }

vector<MetaData> Haf::search(int w,int h)
{
    //vector<Descriptor> *descs = pattern->getDescriptors();
   /* for(int i1=0;i1<SCALE_BC;i1++)
        for(int i2=0;i2<ORIENTATION_BC;i2++)
            for(int i3=0;i3<COORDS_BC;i3++)
                for(int i4=0;i4<COORDS_BC;i4++)
                {
                    buskets[i1][i2][i3][i4] = 0;
                }*/
    for(MetaData &m:data)
    {
        Descriptor *d1 = m.d;
        Descriptor *d2 = d1->getClothest();

        double sBinSize = .25;
        double aBinSize = (2*M_PI)/ORIENTATION_BC;
        double xBinSize = w/COORDS_BC;
        double yBinSize = h/COORDS_BC;

        MetaData mNew;
        mNew.d = d2;
        mNew.angle = /*m.angle - */  d2->getPoint()->orientation - d1->getPoint()->orientation;
        if (mNew.angle < 0) mNew.angle += 2*M_PI;
        mNew.scale = d2->getPoint()->sigma / d1->getPoint()->sigma;
        mNew.x  = d2->getPoint()->getRealX() - mNew.scale*(d1->getPoint()->getRealX()  * cos(mNew.angle) - d1->getPoint()->getRealY() * sin(mNew.angle));
        mNew.y = d2->getPoint()->getRealY() - mNew.scale*(d1->getPoint()->getRealX() * sin(mNew.angle) + d1->getPoint()->getRealY() * cos(mNew.angle));
        int sI = mNew.scale/sBinSize;
        int aI = mNew.angle/aBinSize;
        int xI = mNew.x/xBinSize;
        int yI = mNew.y/yBinSize;

        //cout << mNew.scale << "=s " << mNew.angle << "=a " << mNew.x << "=x " << mNew.y << "=y " <<endl;

        transData.push_back(mNew);

        Index ind;
        for(int i1=0;i1<=1;i1++)
        {
            if (sI+i1 < 0 || sI+i1 >= SCALE_BC ) continue;
            for(int i2=0;i2<=1;i2++)
            {
                if (aI+i2 < 0 || aI+i2 >= ORIENTATION_BC ) continue;
                for(int i3=0;i3<=1;i3++)
                {
                    if (xI+i3 < 0 || xI+i3 >= COORDS_BC ) continue;
                    for(int i4=0;i4<=1;i4++)
                    {
                        if (yI+i4 < 0 || yI+i4 >= COORDS_BC ) continue;
                        ind.sI = sI+i1;
                        ind.aI = aI+i2;
                        ind.xI = xI+i3;
                        ind.yI = yI+i4;
                       // cout << sI << "=s " << aI << "=a " << xI << "=x " << yI << "=y " <<buskets[ind].size()<<endl;
                        if(buskets.find(ind) == buskets.end())
                            buskets[ind] = vector<MetaData>();
                        buskets[ind].push_back(mNew);
                    }
                }
            }
        }
        /*if (scaleI>=0 && angleI>=0 && xI>=0 && yI>=0)   buskets[scaleI  ][angleI  ][xI  ][yI  ]++;
        if (scaleI>=0 && angleI>=0 && xI>=0 && yI>=-1)  buskets[scaleI  ][angleI  ][xI  ][yI+1]++;
        if (scaleI>=0 && angleI>=0 && xI>=-1 && yI>=0)  buskets[scaleI  ][angleI  ][xI+1][yI  ]++;
        if (scaleI>=0 && angleI>=0 && xI>=-1 && yI>=-1) buskets[scaleI  ][angleI  ][xI+1][yI+1]++;
        if (scaleI>=0 && angleI>=-1 && xI>=0 && yI>=0)  buskets[scaleI  ][angleI+1][xI  ][yI  ]++;
        if (scaleI>=0 && angleI>=-1 && xI>=0 && yI>=-1) buskets[scaleI  ][angleI+1][xI  ][yI+1]++;
        if (scaleI>=0 && angleI>=-1 && xI>=-1 && yI>=0) buskets[scaleI  ][angleI+1][xI+1][yI  ]++;
        if (scaleI>=0 && angleI>=-1 && xI>=-1 && yI>=-1)buskets[scaleI  ][angleI+1][xI+1][yI+1]++;

        if (scaleI>=-1 && angleI>=0 && xI>=0 && yI>=0)   buskets[scaleI+1][angleI  ][xI  ][yI  ]++;
        if (scaleI>=-1 && angleI>=0 && xI>=0 && yI>=-1)  buskets[scaleI+1][angleI  ][xI  ][yI+1]++;
        if (scaleI>=-1 && angleI>=0 && xI>=-1 && yI>=0)  buskets[scaleI+1][angleI  ][xI+1][yI  ]++;
        if (scaleI>=-1 && angleI>=0 && xI>=-1 && yI>=-1) buskets[scaleI+1][angleI  ][xI+1][yI+1]++;
        if (scaleI>=-1 && angleI>=-1 && xI>=0 && yI>=0)  buskets[scaleI+1][angleI+1][xI  ][yI  ]++;
        if (scaleI>=-1 && angleI>=-1 && xI>=0 && yI>=-1) buskets[scaleI+1][angleI+1][xI  ][yI+1]++;
        if (scaleI>=-1 && angleI>=-1 && xI>=-1 && yI>=0) buskets[scaleI+1][angleI+1][xI+1][yI  ]++;
        if (scaleI>=-1 && angleI>=-1 && xI>=-1 && yI>=-1) buskets[scaleI+1][angleI+1][xI+1][yI+1]++;*/
    }
   //int max=0;
    MetaData res;
    vector<MetaData> models;
    /*for(int i1=0;i1<SCALE_BC;i1++)
        for(int i2=0;i2<ORIENTATION_BC;i2++)
            for(int i3=0;i3<COORDS_BC;i3++)
                for(int i4=0;i4<COORDS_BC;i4++)
                {
                    if(buskets[i1][i2][i3][i4]>2)
                    {
                        res.sI = i1;
                        res.aI = i2;
                        res.xI = i3;
                        res.yI = i4;
                        models.push_back(res);
                    }

                }*/
    for(auto &b:buskets)
    {
        if(b.second.size()>2)
        {
            res.angle = 0;//res.aI*(2*M_PI)/ORIENTATION_BC;
            res.x = 0;//(res.xI * w)/COORDS_BC;
            res.y = 0;//(res.yI * h)/COORDS_BC;
            res.scale = 0;//res.sI*0.25;
            res.desc.clear();

            for(MetaData &m:b.second)
            {
               /* if ((m.aI == res.aI || m.aI == res.aI-1) &&
                    (m.aI == res.aI || m.aI == res.aI-1) &&
                        (m.xI == res.xI || m.xI == res.xI-1) &&
                        (m.yI == res.yI || m.yI == res.yI-1))*/
                {
                    res.angle += m.angle;
                    res.scale += m.scale;
                    res.x += m.x;
                    res.y += m.y;
                    res.desc.push_back(m.d);
                }
            }
            res.angle /= res.desc.size();
            res.x /= res.desc.size();
            res.y /= res.desc.size();
            res.scale /= res.desc.size();
            //cout << res.scale << "=s " <<  res.angle << "=a " << res.x << "=x " << res.y << "=y " <<res.desc.size()<<endl;
            models.push_back(res);
        }
    }
    return models;

}


void Haf::calcCenter()
{
    vector<Descriptor> *descs = pattern->getDescriptors();
    double minX = pattern->getImageMap()->getWidth();
    double minY = pattern->getImageMap()->getHeight();
    double maxX = 0;
    /*double maxY = 0;
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
    }*/
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


