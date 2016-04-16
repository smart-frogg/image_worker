#ifndef PIRAMID_H
#define PIRAMID_H

#include "octave.h"
#define MIN_SIZE 20
class Piramid
{
private:
    double sigma0;
    double sigmaFirst;
    double width;
    double height;
    vector<Descriptor> descriptors;
    unique_ptr<bool[]> usingPoints;
    void calculateOctavs(const ImageMap &input);
public:
    int countLayers;
    void saveDOG(QString filename);
    vector<unique_ptr<Octave>> octavs;
    double getHeight();
    double getWidth();
    double getSigmaFirst();
    double getSigmaLast();
    double getSigmaStep();
    unique_ptr<QImage> asImage();
    double L(int x, int y, double sigma);
    void save(QString filename);
    void saveDescriptors(QString filename);
    void genDescriptors();
    void findGoodPoints();
    Piramid(int countLayers, double sigma0, double sigmaFirst, const ImageMap &input);
};

#endif // PIRAMID_H
