#ifndef MORAVEKDETECTOR_H
#define MORAVEKDETECTOR_H

#include "abstractdetector.h"
#include "sobelfilter.h"
class MoravekDetector : public AbstractDetector
{
    int d[8][2];
    double T;
    int winR;
public:
    MoravekDetector(ImageMap *data);
    void configure(double T, int winR);
    virtual void detect();
};

#endif // MORAVEKDETECTOR_H
