#include"FaceFrontalization.h"

FaceFrontalization::FaceFrontalization(const double* mask, const double* tri_ind, int width, int height, int nChannels, 
						const double* all_vertex_src, const double* all_vertex_ref, int all_ver_dim, int all_ver_length,
						const double* all_tri, int all_tri_dim, int all_tri_length,  int bg_tri_num,
						const double* valid_tri_half, int vertex_length, int tri_length,
						const double* sym_tri_list, int symlist_length)

{
	this->mask = mask;
	this->tri_ind = tri_ind;
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;

	this->all_vertex_src = all_vertex_src;
	this->all_vertex_ref = all_vertex_ref;
	this->all_ver_dim = all_ver_dim;
	this->all_ver_length = all_ver_length;

	this->all_tri = all_tri;
	this->all_tri_dim = all_tri_dim;
	this->all_tri_length = all_tri_length;
	this->bg_tri_num = bg_tri_num;

	this->valid_tri_half = valid_tri_half;
	this->vertex_length = vertex_length;
	this->tri_length = tri_length;

	this->sym_tri_list = sym_tri_list;
	this->symlist_length = symlist_length;
}

FaceFrontalization::FaceFrontalization(const double* img, int width, int height, int nChannels, double* corres_map)
{
	this->img = img;
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;
	this->corres_map_input = corres_map;
}