#ifndef PIRAMID_H
#define PIRAMID_H

#include "octave.h"
#include "blobdetector.h"
#define MIN_SIZE 40
class Piramid
{
private:
    double sigma0;
    double sigmaFirst;
    double width;
    double height;
    unique_ptr<bool[]> usingPoints;
    void calculateOctavs(const ImageMap &input);
    void invert(double dd1[3][3], double const dd[3][3]);
    void clarify();
    double dX(double x,double y,double s);
    double dY(double x,double y,double s);
    double dS(double x,double y,double s);
public:
    vector<Blob> blobs;
    vector<Descriptor> *getDescriptors();
    vector<Descriptor> descriptors;
    int countLayers;
    void saveDOG(QString filename);
    vector<unique_ptr<Octave>> octavs;
    double getHeight();
    double getWidth();
    double getSigmaFirst();
    double getSigmaLast();
    double getSigmaStep();
    void clear();
    unique_ptr<QImage> asImage();
    double L(int x, int y, double sigma);
    void save(QString filename);
    void saveDescriptors(QString filename);
    void genDescriptors();
    void genBlobs();
    void genDOG();
    void findGoodPoints();
    Piramid(int countLayers, double sigma0, double sigmaFirst, const ImageMap &input);
    double dogL(int x,int y,double sigma);
};

#endif // PIRAMID_H
