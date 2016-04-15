#ifndef PIRAMID_H
#define PIRAMID_H

#include "octave.h"
#define MIN_SIZE 40
class Piramid
{
private:
    vector<unique_ptr<Octave>> octavs;
    int countLayers;
    double sigma0;
    double sigmaFirst;
    void calculateOctavs(const ImageMap &input);

public:
    double L(int x, int y, double sigma);
    void save(QString filename);
    unique_ptr<vector<Descriptor>> genDescriptors();
    Piramid(int countLayers, double sigma0, double sigmaFirst, const ImageMap &input);
};

#endif // PIRAMID_H
