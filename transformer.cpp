#include "transformer.h"
#include <math.h>
#include <iostream>


Transformer::Transformer(double treshold)
{
    this->treshold = treshold;
}

void Transformer::normalize(vector<Descriptor> *desc)
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
        d.getPoint()->normX = (d.getPoint()->x - minX)/(maxX - minX);
        d.getPoint()->normY = (d.getPoint()->y - minY)/(maxY - minY);
    }
}

void Transformer::formMatrix(Point *p1, Point *p2,gsl_matrix_view A)
{
    gsl_matrix_set(&(A.matrix),0,0,p1->normX);
    gsl_matrix_set(&(A.matrix),1,3,p1->normX);
    gsl_matrix_set(&(A.matrix),0,1,p1->normY);
    gsl_matrix_set(&(A.matrix),1,4,p1->normY);
    gsl_matrix_set(&(A.matrix),0,2,1);
    gsl_matrix_set(&(A.matrix),1,5,1);
    gsl_matrix_set(&(A.matrix),0,6,-p2->normX*p1->normX);
    gsl_matrix_set(&(A.matrix),0,7,-p2->normX*p1->normY);
    gsl_matrix_set(&(A.matrix),0,8,-p2->normX);
    gsl_matrix_set(&(A.matrix),1,6,-p2->normY*p1->normX);
    gsl_matrix_set(&(A.matrix),1,7,-p2->normY*p1->normY);
    gsl_matrix_set(&(A.matrix),1,8,-p2->normY);
}

double Transformer::checkHypothesis(gsl_vector_view h)
{
    gsl_vector *x = gsl_vector_alloc(2);
    gsl_matrix *A = gsl_matrix_alloc(2,9);
    gsl_matrix_set_zero(A);
    int positives = 0;
    int used = 0;
    for(Descriptor &d:*descFrom)
    {
        if (!d.used) continue;
        used++;
        Point *p1 = d.getPoint();
        Point *p2 = d.getClothest()->getPoint();
        formMatrix(p1, p2, gsl_matrix_submatrix (A, 0, 0, 2, 9));
        for (int i=0;i<2;i++)
        {
            for (int j=0;j<9;j++)
                 cout<<gsl_matrix_get(A,i,j)<<" ";
            cout<<endl;
        }
        gsl_blas_dgemv(CblasNoTrans, 1.0, A, &(h.vector), 0.0, x);
        cout<<gsl_vector_get(x,0)<<" "<<gsl_vector_get(x,1)<<endl;
        if (gsl_vector_get(x,0) < treshold && gsl_vector_get(x,1) < treshold)
            positives++;
    }
    gsl_matrix_free(A);
    gsl_vector_free(x);
    return positives*1.0/used;
}

int Transformer::getRandomPoint()
{
     int n = rand() % descFrom->size();
     while (!(*descFrom)[n].used)
         n = rand() % descFrom->size();
     return n;
}

gsl_vector *Transformer::ransac()
{
    double positives = 0;
    gsl_vector *result = gsl_vector_alloc(9);
    gsl_matrix *AT = gsl_matrix_alloc(9,8);
    gsl_matrix *B = gsl_matrix_alloc(9,9);
    gsl_matrix *V = gsl_matrix_alloc(9,9);
    gsl_vector *S = gsl_vector_alloc(9);
    gsl_vector *work = gsl_vector_alloc(9);
    gsl_matrix *A = gsl_matrix_alloc(8,9);
    gsl_matrix_set_zero(A);
    for(int i=0; i<descFrom->size()/10; i++)
    {
        int n = getRandomPoint();
        Point *p1 = (*descFrom)[n].getPoint();
        Point *p2 = (*descFrom)[n].getClothest()->getPoint();
        formMatrix(p1, p2, gsl_matrix_submatrix (A, 0, 0, 2, 9));
        n = getRandomPoint();
        p1 = (*descFrom)[n].getPoint();
        p2 = (*descTo)[n].getPoint();
        formMatrix(p1, p2, gsl_matrix_submatrix (A, 2, 0, 2, 9));
        n = getRandomPoint();
        p1 = (*descFrom)[n].getPoint();
        p2 = (*descTo)[n].getPoint();
        formMatrix(p1, p2, gsl_matrix_submatrix (A, 4, 0, 2, 9));
        n = getRandomPoint();
        p1 = (*descFrom)[n].getPoint();
        p2 = (*descTo)[n].getPoint();
        formMatrix(p1, p2, gsl_matrix_submatrix (A, 6, 0, 2, 9));

        gsl_matrix_transpose_memcpy (AT, A);

        gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1, AT, A, 0, B);

        gsl_linalg_SV_decomp(B,V,S,work);

        int min = gsl_vector_min_index(S);
        gsl_vector_view h = gsl_matrix_column(B,min);
        for (int a=0;a<9;a++)
            cout<<gsl_vector_get(&(h.vector),a)<<endl;
        double curPositives = checkHypothesis(h);
        if (positives < curPositives)
        {
            positives = curPositives;
            gsl_vector_memcpy(result,&(h.vector));
        }
        if (positives>0.3)
            break;
    }
    gsl_matrix_free(A);
    gsl_matrix_free(AT);
    gsl_matrix_free(B);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);

    return result;
}

QTransform * Transformer::getTransform()
{
    normalize(descFrom);
    normalize(descTo);
    gsl_vector *result = ransac();
    QTransform * transform = new QTransform(
                gsl_vector_get(result,0),
                gsl_vector_get(result,1),
                gsl_vector_get(result,2),
                gsl_vector_get(result,3),
                gsl_vector_get(result,4),
                gsl_vector_get(result,5),
                gsl_vector_get(result,6),
                gsl_vector_get(result,7),
                gsl_vector_get(result,8)
                );
}
