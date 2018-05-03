#include <mex.h>
#include <matrix.h>
#include "ImageInpainting.h"
void mexFunction(int nlhs, mxArray *plhs[], int nrhs,const mxArray *prhs[])
{
	double* img = mxGetPr(prhs[0]);
	double* ref = mxGetPr(prhs[1]);
	double* mask = mxGetPr(prhs[2]);
	double* facemask = mxGetPr(prhs[3]);
	int width = (int)(*mxGetPr(prhs[4]));
	int height = (int)(*mxGetPr(prhs[5]));
	int nChannels = (int)(*mxGetPr(prhs[6]));
	double blend_width = (double)(*mxGetPr(prhs[7]));
	
    
	const mwSize dims[3]={height, width, nChannels};
    plhs[0] = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
	double* result = mxGetPr(plhs[0]);

	ImageInpainting Inpainter(img, ref, mask, facemask, width, height, nChannels);
	Inpainter.Run(result, blend_width);
}
	