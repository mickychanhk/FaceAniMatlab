#ifndef LINSYS_H
#define LINSYS_H


extern void linbcg(int* ija_p, double* sa_p, double *b, double *x, const int itol, const double tol,
			const int itmax, int &iter, double &err, int n);
extern void sprsin(double *a, const double thresh, double *sa, int *ija, int n, int nmax);

#endif