#include <mex.h>
#include <matrix.h>
#include "FaceFrontalization.h"
void mexFunction(int nlhs, mxArray *plhs[], int nrhs,const mxArray *prhs[])
{
	double* mask = mxGetPr(prhs[0]);
	int width = (int)(*mxGetPr(prhs[1]));
	int height = (int)(*mxGetPr(prhs[2]));
	int nChannels = (int)(*mxGetPr(prhs[3]));

	///// For meshed image
	// the map providing the corresponding tri of each pixel
	double* tri_ind = mxGetPr(prhs[4]);

	// the meshed src and des image
	double* all_vertex_src = mxGetPr(prhs[5]);
	double* all_vertex_ref = mxGetPr(prhs[6]);
	int all_ver_dim = (int)*mxGetPr(prhs[7]);
	int all_ver_length = (int)*mxGetPr(prhs[8]);

	double* all_tri = mxGetPr(prhs[9]);
	int all_tri_dim = (int)*mxGetPr(prhs[10]);
	int all_tri_length = (int)*mxGetPr(prhs[11]);

	int bg_tri_num = (int)*mxGetPr(prhs[12]); // the number of background tri

	//// For face symmetry
	double* valid_tri_half = mxGetPr(prhs[13]); // the visible half (set to 1) and invisible half (set to 0)
	int vertex_length = (int)*mxGetPr(prhs[14]);
	int tri_length = (int)*mxGetPr(prhs[15]);

	double* sym_tri_list = mxGetPr(prhs[16]);
	int symlist_length = (int)*mxGetPr(prhs[17]);

	
	const mwSize dims[3]={height, width, 2};
	plhs[0] = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
	double* corres_map = mxGetPr(plhs[0]);

	plhs[1] = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
	double* corres_map_sym = mxGetPr(plhs[1]);
    
	FaceFrontalization FF(mask, tri_ind, width, height, nChannels, 
						all_vertex_src, all_vertex_ref, all_ver_dim, all_ver_length,
						all_tri, all_tri_dim, all_tri_length, bg_tri_num,
						valid_tri_half, vertex_length, tri_length,
						sym_tri_list, symlist_length);
	FF.frontalization_mapping(corres_map, corres_map_sym);

}