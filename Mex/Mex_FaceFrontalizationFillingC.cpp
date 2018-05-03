#include <mex.h>
#include <matrix.h>
#include "FaceFrontalization.h"
void mexFunction(int nlhs, mxArray *plhs[], int nrhs,const mxArray *prhs[])
{
	double* img = mxGetPr(prhs[0]);
	int width = (int)(*mxGetPr(prhs[1]));
	int height = (int)(*mxGetPr(prhs[2]));
	int nChannels = (int)(*mxGetPr(prhs[3]));

	///// For meshed image
	// the map providing the corresponding tri of each pixel
	double* corres_map = mxGetPr(prhs[4]);
	
    
	const mwSize dims[3]={height, width, nChannels};
    plhs[0] = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
	double* result = mxGetPr(plhs[0]);

    
	FaceFrontalization FF(img, width, height, nChannels, corres_map);
	FF.frontalization_filling(result);

}