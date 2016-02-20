#ifndef OCTAVE_H
#define OCTAVE_H

#include <stdlib.h>
#include "filterkernel.h"

class Octave
{
private:
    int height;
    int width;
    int countLayers;
    double sigmaFirst;
    vector<unique_ptr<ImageMap>> layers;
    void calculateLayers(const ImageMap &input);

public:
    double getStep();
    double L(int x,int y,double sigma);
    void save(QString filename);
    unique_ptr<ImageMap> scale();
    Octave(int countLayers, double sigma, const ImageMap &input);
};

#endif // OCTAVE_H
