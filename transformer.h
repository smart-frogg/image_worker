#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include "descriptor.h"

#define GSL_EXPORT
extern "C"{
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
}
//#include <eigen3/Eigen/Dense>


//using namespace Eigen;

#include <QTransform>

class Transformer
{
private:
    double treshold;
    QTransform normalize(vector<Descriptor*> *desc);
    void formMatrix(vector<Descriptor*> descs, gsl_matrix *A);
    double checkHypothesis(gsl_vector_view h);
    void ransac(double *result, int iterations);
    int getRandomPoint(int n1,int n2,int n3);
    vector <Descriptor*> positiveDescs;
    vector <Descriptor*> descFrom;
    vector <Descriptor*> descTo;
public:
    void setDescs(vector<Descriptor> *desc);
    Transformer(double treshold);
    vector<QTransform> getTransformVec();
    QTransform getTransform();
};

#endif // TRANSFORMER_H
