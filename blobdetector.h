#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H
#include "piramid.h"
class Piramid;
#define BLOB_R 3
struct Blob : public Point
{
    //int x;
    //int y;
    double r;
    double val;
   // double sigma;
    bool used = true;
    Blob(int x, int y, double sigma, double val);
};

class BlobDetector
{
private:
    Piramid *data;
public:
    vector<Blob> blobs;
    vector<Descriptor> desc;
    BlobDetector(Piramid *p);
    void detect();
    void save(QString filename);
};

#endif // BLOBDETECTOR_H
