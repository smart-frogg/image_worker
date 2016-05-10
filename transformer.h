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
    void afinFormMatrix(vector<Descriptor*> descs, gsl_matrix *A);
    double affinCheckHypothesis(gsl_vector_view h);
    void ransac(double *result, int iterations);
    void afinn(double *result);
    int getRandomPoint(int n1,int n2,int n3);
    vector <Descriptor*> positiveDescs;
    vector <Descriptor*> descFrom;
    vector <Descriptor*> descAll;
public:
    void setDescs(vector<Descriptor> *desc);
    double positives;
    void setDescs(vector<Descriptor*> desc, vector<Descriptor> *all);
    Transformer(double treshold);
    vector<QTransform> getTransformVec();
    QTransform getTransform();
    QTransform getAffinTransform();
};

#endif // TRANSFORMER_H
