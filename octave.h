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
    void calculateLayers(const ImageMap &input);

public:
    vector<unique_ptr<ImageMap>> layers;
    vector<unique_ptr<ImageMap>> dogs;

    unique_ptr<ImageMap> genOneDOG(int i);
    void genDOG();
    double getStep();
    double L(int x,int y,double sigma);
    void save(QString filename);
    int size();
    unique_ptr<QImage> asImage();
    unique_ptr<ImageMap> scale();
    void genDescriptors(vector<Descriptor> *descriptors, double sigma);
    Octave(int countLayers, double sigma, int scale, const ImageMap &input);
    void saveDOG(QString filename);
};

#endif // OCTAVE_H
