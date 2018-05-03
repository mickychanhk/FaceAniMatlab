#include "ImageInpainting.h"
#include <cstring>

ImageInpainting::ImageInpainting(double* img, double* ref, double* mask, double* face_mask, int width, int height, int nChannels)
{
	this->boundary = new double[width * height];
	this->face_boundary = new double[width * height];
	this->blend_boundary = new double[width * height];
	
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;

	this->img_input = img;
	this->mask_input = mask;
	this->ref_img_input = ref;
	this->face_mask_input = face_mask;

	boundary_ready = false;
}


ImageInpainting::~ImageInpainting()
{
	delete[] boundary;
	delete[] face_boundary;
	delete[] blend_boundary;
}



void ImageInpainting::Run(double* result, double blend_width)
{
	double* temp_result = new double[width * height * nChannels];

	// 1. Poisson Editing to fill invisible pixel
	Inpainting_PE(temp_result);

	/*************************************************************************************/
	// 2. Blurring the Boundary of Face and Background
	if(!boundary_ready)
		getBoundary();

	Blurring4Detail(result, temp_result, blend_width);
	
	delete[] temp_result;
}