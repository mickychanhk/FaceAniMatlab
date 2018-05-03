#ifndef IMAGEINPAINTING_H
#define IMAGEINPAINTING_H

#define INF 100000000

#ifndef MAXNCHANNELS
#define MAXNCHANNELS 5
#endif MAXNCHANNELS

#ifndef PI
#define PI 3.1415926
#endif

#include"DebugFunc.h"
class ImageInpainting
{
public:
	ImageInpainting(double* img, double* ref, double* mask, double* face_mask, int width, int height, int nChannels);
	~ImageInpainting();

	// Part 2. Poisson Editing methods
	void Inpainting_PE(double* result);

	// Part 3. Template
	void Run(double* result, double blend_width);
	void PE_BorderBlending(double* result, int blend_width);
	void PE_BorderBlending(double* result, int blend_level, int blend_width);
	
private:
	const double* img_input;
	const double* ref_img_input;

	const double* mask_input;
	const double* face_mask_input;

	bool boundary_ready;
	double* boundary;
	double* face_boundary;
	double* blend_boundary;

	int width;
	int height;
	int nChannels;

	// Part 2. Poisson Editing methods
	void PoissonEditing_byChannel(const double* img, const double* mask, const double* div, int width, int height, double* result);
	void Laplacian(const double* img, int width, int height, double* result);
	void PE_Borderblend_Laplacian(double* div, const double* img_src);
	void PE_Borderblend_Bordermean(double* border_mean, const double* div_src);

	// Part 3. Boundary Blending
	void BoundaryDiv(double* div, double* blend_mask, const double* img_src, int blend_width);
	double Laplacian_point(const double* img, int width, int height, int x, int y);
	void Blurring4Detail(double* des, const double* src, double blend_width);

	// Part 3. Auxiliary Functions
	void getBoundary();
	void dilate(double* des, const double* src, double flag, int width, int height, int dilate_width);

};

enum flag{BAND, KNOWN, INSIDE};

struct Point
{
	int x;
	int y;
	double T;
};


#endif