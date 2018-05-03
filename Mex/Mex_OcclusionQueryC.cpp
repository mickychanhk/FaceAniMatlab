#include <mex.h>
#include <matrix.h>
#include "3DMM.h"
void mexFunction(int nlhs,mxArray *plhs[], int nrhs,const mxArray *prhs[])
{
	double* vertex;
	double* tri;
	int nver;
	int ntri;
	int width;
	int height;
	double* visibility;
	double threshold;

	vertex = mxGetPr(prhs[0]);
	tri = mxGetPr(prhs[1]);
	nver = *mxGetPr(prhs[2]);
	ntri = *mxGetPr(prhs[3]);
	width = *mxGetPr(prhs[4]);
	height = *mxGetPr(prhs[5]);
	threshold = *mxGetPr(prhs[6]);

	plhs[0] = mxCreateDoubleMatrix(nver, 1, mxREAL);
	visibility = mxGetPr(plhs[0]);
    
	OcclusionQuery(vertex, tri, ntri, nver, width, height, visibility, threshold);
}