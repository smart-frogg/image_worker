#ifndef OCTAVE_H
#define OCTAVE_H

#include <stdlib.h>
#include "harrisdetector.h"

class Octave
{
private:
    int height;
    int width;
    int countLayers;
    double sigmaFirst;
    int scaleSize;
    vector<unique_ptr<ImageMap>> layers;
    void calculateLayers(const ImageMap &input);

public:
    double getStep();
    double L(int x,int y,double sigma);
    void save(QString filename);
    unique_ptr<ImageMap> scale();
    void genDescriptors(vector<Descriptor> *descriptors);
    Octave(int countLayers, double sigma, int scale, const ImageMap &input);
};

#endif // OCTAVE_H
