#include <math.h>
#include "stdlib.h"
#include "linsys.h"


void sprstx(double *sa, int *ija, double *x, double *b, int n)
{
	int i,j,k;
	if (ija[0] != (n+1)){
		return;
	}
	for (i=0;i<n;i++) b[i]=sa[i]*x[i];
	for (i=0;i<n;i++) {
		for (k=ija[i];k<ija[i+1];k++) {
			j=ija[k];
			b[j] += sa[k]*x[i];
		}
	}
}

void sprsax(double *sa, int *ija, double *x, double *b, int n)
{
	int i,k;
	if (ija[0] != n+1){
		return;
	}
	for (i=0;i<n;i++) {
		b[i]=sa[i]*x[i];
		for (k=ija[i];k<ija[i+1];k++) {
			b[i] += sa[k]*x[ija[k]];
		}
	}
}

void atimes(int* ija_p, double* sa_p, double *x, double *r, const int itrnsp, int n)
{
	if (itrnsp) sprstx(sa_p,ija_p,x,r,n);
	else sprsax(sa_p,ija_p,x,r,n);
}

double snrm(double *sx, const int itol, int n)
{
	int i,isamax;
	double ans;

	if (itol <= 3) {
		ans = 0.0;
		for (i=0;i<n;i++) ans += sx[i]*sx[i];
		return sqrt(ans);
	} else {
		isamax=0;
		for (i=0;i<n;i++) {
			if (fabs(sx[i]) > fabs(sx[isamax])) isamax=i;
		}
		return fabs(sx[isamax]);
	}
}

void asolve(int* ija_p, double* sa_p, double *b, double *x, const int itrnsp, int n)
{
	int i;
	for(i=0;i<n;i++) x[i]=(sa_p[i] != 0.0 ? b[i]/sa_p[i] : b[i]);
}

void linbcg(int* ija_p, double* sa_p, double *b, double *x, const int itol, const double tol,
			const int itmax, int &iter, double &err, int n)
{
	double ak,akden,bk,bkden=1.0,bknum,bnrm,dxnrm,xnrm,zm1nrm,znrm;
	const double EPS=1.0e-14;
	int j;

	double *p,*pp,*r,*rr,*z,*zz;
	try {
		p = new double[n];
		pp = new double[n];
		r = new double[n];
		rr = new double[n];
		z = new double[n];
		zz = new double[n];
	} catch (...) {
		j = 0;
	}

	iter=0;
	atimes(ija_p, sa_p, x,r,0,n);
	for (j=0;j<n;j++) {
		r[j]=b[j]-r[j];
		rr[j]=r[j];
	}
	if (itol == 1) {
		bnrm=snrm(b,itol,n);
		asolve(ija_p, sa_p, r,z,0,n);
	}
	else if (itol == 2) {
		asolve(ija_p, sa_p,b,z,0,n);
		bnrm=snrm(z,itol,n);
		asolve(ija_p, sa_p,r,z,0,n);
	}
	else if (itol == 3 || itol == 4) {
		asolve(ija_p, sa_p,b,z,0,n);
		bnrm=snrm(z,itol,n);
		asolve(ija_p, sa_p,r,z,0,n);
		znrm=snrm(z,itol,n);
	} else {
		return;
	}
	while (iter < itmax) {
		++iter;
		asolve(ija_p, sa_p,rr,zz,1,n);
		for (bknum=0.0,j=0;j<n;j++) bknum += z[j]*rr[j];
		if (iter == 1) {
			for (j=0;j<n;j++) {
				p[j]=z[j];
				pp[j]=zz[j];
			}
		} else {
			bk=bknum/bkden;
			for (j=0;j<n;j++) {
				p[j]=bk*p[j]+z[j];
				pp[j]=bk*pp[j]+zz[j];
			}
		}
		bkden=bknum;
		atimes(ija_p, sa_p,p,z,0,n);
		for (akden=0.0,j=0;j<n;j++) akden += z[j]*pp[j];
		ak=bknum/akden;
		atimes(ija_p, sa_p, pp,zz,1,n);
		for (j=0;j<n;j++) {
			x[j] += ak*p[j];
			r[j] -= ak*z[j];
			rr[j] -= ak*zz[j];
		}
		asolve(ija_p, sa_p,r,z,0,n);
		if (itol == 1)
			err=snrm(r,itol,n)/bnrm;
		else if (itol == 2)
			err=snrm(z,itol,n)/bnrm;
		else if (itol == 3 || itol == 4) {
			zm1nrm=znrm;
			znrm=snrm(z,itol,n);
			if (fabs(zm1nrm-znrm) > EPS*znrm) {
				dxnrm=fabs(ak)*snrm(p,itol,n);
				err=znrm/fabs(zm1nrm-znrm)*dxnrm;
			} else {
				err=znrm/bnrm;
				continue;
			}
			xnrm=snrm(x,itol,n);
			if (err <= 0.5*xnrm) err /= xnrm;
			else {
				err=znrm/bnrm;
				continue;
			}
		}
		if (err <= tol) break;
	}

	delete[] p;
	delete[] pp;
	delete[] r;
	delete[] rr;
	delete[] z;
	delete[] zz;
}

void sprsin(double *a, const double thresh, double *sa, int *ija, int n, int nmax)
{
	int i,j,k;
	for (j=0;j<n;j++) sa[j]=a[j*n+j];
	ija[0]=n+1;
	k=n;
	for (i=0;i<n;i++) {
		for (j=0;j<n;j++) {
			if (fabs(a[i*n+j]) >= thresh && i != j) {
				if (++k > nmax){
					return;
				}
				sa[k]=a[i*n+j];
				ija[k]=j;
			}
		}
		ija[i+1]=k+1;
	}
}
