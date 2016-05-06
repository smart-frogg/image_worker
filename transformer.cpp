#include "transformer.h"
#include <math.h>
#include <iostream>


Transformer::Transformer(double treshold)
{
    this->treshold = treshold;
}

QTransform Transformer::normalize(vector<Descriptor> *desc)
{
    double maxX = -1;
    double maxY = -1;
    double minX = -1;
    double minY = -1;
    for (Descriptor &d:*desc)
    {
        if (minX==-1 || minX>d.getPoint()->x)
            minX=d.getPoint()->x;
        if (minY==-1 || minY>d.getPoint()->y)
            minY=d.getPoint()->y;
        if (maxX<d.getPoint()->x)
            maxX=d.getPoint()->x;
        if (maxY<d.getPoint()->y)
            maxY=d.getPoint()->y;
    }
    for (Descriptor &d:*desc)
    {
        d.getPoint()->normX = 2*(d.getPoint()->x - minX)/(maxX - minX) - 1;
        d.getPoint()->normY = 2*(d.getPoint()->y - minY)/(maxY - minY) - 1;
    }
    QTransform transform;
    transform.translate(-minX, -minY);
    transform.scale(2/(maxX - minX), 2/(maxY - minY));
    transform.translate(-1, -1);
    return transform;
}

void Transformer::formMatrix(vector<Descriptor*> descs, gsl_matrix *A)
{
    int i=0;
    for (Descriptor *d:descs)
    {
    Point *p1 = d->getPoint();
    Point *p2 = d->getClothest()->getPoint();
    gsl_matrix_set(A,i+0,0,p1->normX);
    gsl_matrix_set(A,i+1,3,p1->normX);
    gsl_matrix_set(A,i+0,1,p1->normY);
    gsl_matrix_set(A,i+1,4,p1->normY);
    gsl_matrix_set(A,i+0,2,1);
    gsl_matrix_set(A,i+1,5,1);
    gsl_matrix_set(A,i+0,6,-p2->normX*p1->normX);
    gsl_matrix_set(A,i+0,7,-p2->normX*p1->normY);
    gsl_matrix_set(A,i+0,8,-p2->normX);
    gsl_matrix_set(A,i+1,6,-p2->normY*p1->normX);
    gsl_matrix_set(A,i+1,7,-p2->normY*p1->normY);
    gsl_matrix_set(A,i+1,8,-p2->normY);
    i+=2;
    }
}

double Transformer::checkHypothesis(gsl_matrix *H)
{
    gsl_vector *x = gsl_vector_alloc(3);
    gsl_vector *x1 = gsl_vector_alloc(3);

    int positives = 0;
    int used = 0;
    for(Descriptor &d:*descFrom)
    {
        if (!d.used) continue;
       // cout<<"Used descriptor "<<used<<endl;
        used++;
        Point *p1 = d.getPoint();
        Point *p2 = d.getClothest()->getPoint();
        gsl_vector_set(x,0,p1->normX);
        gsl_vector_set(x,1,p1->normY);
        gsl_vector_set(x,2,1.0);
        gsl_vector_set(x1,0,p2->normX);
        gsl_vector_set(x1,1,p2->normY);
        gsl_vector_set(x1,2,1.0);

        gsl_blas_dgemv(CblasNoTrans, 1.0, H, x, -1.0, x1);
        double l = 0;
        for (int i=0;i<3;i++)
            l += gsl_vector_get(x1,i)*gsl_vector_get(x1,i);
        l = sqrt(l);
        cout << l <<endl;
        if (l < treshold)
            positives++;
    }
    gsl_matrix_free(H);
    gsl_vector_free(x);
    gsl_vector_free(x1);
    return positives*1.0/used;
}

int Transformer::getRandomPoint()
{
     int n = rand() % descFrom->size();
     while (!(*descFrom)[n].used)
         n = rand() % descFrom->size();
     return n;
}

gsl_matrix *Transformer::ransac()
{
    double positives = 0;
    gsl_matrix *AT = gsl_matrix_alloc(9,8);
    gsl_matrix *result = gsl_matrix_alloc(3,3);
    gsl_matrix *B = gsl_matrix_alloc(9,9);
    gsl_matrix *V = gsl_matrix_alloc(9,9);
    gsl_vector *S = gsl_vector_alloc(9);
    gsl_vector *work = gsl_vector_alloc(9);
    gsl_matrix *A = gsl_matrix_alloc(8,9);
    gsl_matrix *H = gsl_matrix_alloc(3,3);
    gsl_matrix_set_zero(A);
    for(int i=0; i<descFrom->size()/10; i++)
    {
        vector<Descriptor *> descs;

        int n = getRandomPoint();
        descs.push_back(&((*descFrom)[n]));
        n = getRandomPoint();
        descs.push_back(&((*descFrom)[n]));
        n = getRandomPoint();
        n = getRandomPoint();
        descs.push_back(&((*descFrom)[n]));
        formMatrix(descs, A);

        gsl_matrix_transpose_memcpy (AT, A);

        gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1, AT, A, 0, B);

        gsl_linalg_SV_decomp(B,V,S,work);

        int min = gsl_vector_min_index(S);
        gsl_vector_view h = gsl_matrix_column(B,min);
        gsl_vector_scale(&(h.vector),1/gsl_vector_get(&(h.vector),8));

        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
            {
                gsl_matrix_set(H,i,j,gsl_vector_get(&(h.vector),i*3+j));
                cout<<gsl_vector_get(&(h.vector),i*3+j)<<endl;
            }
        double curPositives = checkHypothesis(H);
        if (positives < curPositives)
        {
            positives = curPositives;
            gsl_matrix_memcpy(result,H);
        }
        if (positives>0.3)
            break;
    }
    gsl_matrix_free(A);
    gsl_matrix_free(AT);
    gsl_matrix_free(B);
    gsl_matrix_free(V);
    gsl_matrix_free(H);
    gsl_vector_free(S);
    gsl_vector_free(work);

    return result;
}

QTransform * Transformer::getTransform()
{
    QTransform transform1 = normalize(descFrom);
    QTransform transform2 = normalize(descTo);
    gsl_matrix *result = ransac();
    QTransform * transform = new QTransform(
                gsl_matrix_get(result,1,0),
                gsl_matrix_get(result,1,1),
                gsl_matrix_get(result,2,1),
                gsl_matrix_get(result,0,0),
                gsl_matrix_get(result,0,1),
                gsl_matrix_get(result,0,2),
                gsl_matrix_get(result,2,0),
                gsl_matrix_get(result,2,1),
                gsl_matrix_get(result,2,2)
                );
     gsl_matrix_free(result);
    *transform = transform1*(* transform)*transform2.inverted();
    return transform;
}
