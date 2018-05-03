#include <math.h>
#include <algorithm>
#include "ImageInpainting.h"
#include "linsys.h"
using namespace std;

#define NEI_LIST_NUM 9
#define BLEND_REGION_NUM 4

void ImageInpainting::Inpainting_PE(double* result)
{
	int x, y, n;
	double* img_n = new double[width * height];
	double* img_src_n = new double[width * height];
	double* ref_img_n = new double[width * height];
	double* result_n = new double[width * height];
	double* div = new double[width * height];


	for(n = 0; n < nChannels; n++)
	{
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				img_n[x * height + y] = img_input[n * width * height + x * height + y];
				ref_img_n[x * height + y] = ref_img_input[n * width * height + x * height + y];
			}
		}

		// Get Laplacian Image
		Laplacian(ref_img_n, width, height, div);		

		// PoissonEditing
		PoissonEditing_byChannel(img_n, mask_input, div, width, height, result_n);

		//ImageShow(result_n, width, height, 1);

		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				result[n * width * height + x * height + y] = result_n[x * height + y];
			}
		}

	}

	delete[] img_n;
	delete[] img_src_n;
	delete[] ref_img_n;
	delete[] result_n;
	delete[] div;

}


void ImageInpainting::PoissonEditing_byChannel(const double* img, const double* mask, const double* div, int width, int height, double* result)
{
	// We refer the implementation of Poisson Editing from
	// http://www.pudn.com/downloads488/sourcecode/graph/texture_mapping/detail2035185.html
	int x,y;

	int* mask_ind = new int[width * height];
	int mask_num = 0;

	// Init
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(mask[x * height + y] <= 0)
				mask_ind[x * height + y] = -1;
			else
				mask_ind[x * height + y] = mask_num++;

			result[x * height + y] = img[x * height + y];
		}
	}
		
	
	///////////////////////////////////////////////
	// do the Poisson Editing
	int* ija_p = new int[mask_num * 5 + 1]; // the index of coefficients of each equation
	double* coefficients = new double[mask_num * 5 + 1]; // coefficients of each equation
	double* edit_result = new double[mask_num + 1];
	double* constants = new double[mask_num + 1];


	//ija_p£º
	//ija_p[1:mask_no] the equation of the pth point
	//ija_p[mask_no+1:end] neighborhood


	/////////////////////////////////////////////////////////
	// 1. Constructing the Equations
	int ijacount = mask_num + 1;
	int mask_count = 0;

	int t, l, b, r, xc, yc, tm, bm, lm, rm;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(mask_ind[x * height + y] < 0)
				continue; // invalid point

			//the 4 neighbors of (x,y) on the mask
			tm = (y>0)? (y-1): y;
			bm = (y<height-1)? (y+1): y;
			lm = (x>0)? (x-1): x;
			rm = (x<width-1)? (x+1): x;

			// the positions on the img
			xc = x;
			yc = y;
			// the neighbors on the img
			t = (yc>0)? (yc-1): 0;
			b = (yc<height-1)? (yc+1): yc;
			l = (xc>0)? (xc-1): 0;
			r = (xc<width-1)? (xc+1): xc;

			// for the mask_count equation:
			// the constrant include the Laplacian item
			constants[mask_count] = div[x * height + y];

			// the result is inited to 0
			edit_result[mask_count] = 0;

			ija_p[mask_count] = ijacount; // point to the variable index container
			coefficients[mask_count] = 4; // the coefficients of point (x,y)

			if (mask[x * height + tm] > 0) { // UP
				// the point in INSIDE the editing region
				ija_p[ijacount] = mask_ind[x * height + tm]; // the pointer to index
				coefficients[ijacount]  = -1; // the coefficients of variable
				ijacount++; } 
			else 
				// the point is outside the editing region
				constants[mask_count] += img[xc * height + t];

			if (mask[lm * height + y] > 0) { // LEFT
				ija_p[ijacount] = mask_ind[lm * height + y]; // the pointer to index
				coefficients[ijacount]  = -1; // the coefficients of variable
				ijacount++; } 
			else 
				constants[mask_count] += img[l * height + yc];

			if (mask[rm * height + y] > 0) { // RIGHT
				ija_p[ijacount] = mask_ind[rm * height + y]; // the pointer to index
				coefficients[ijacount]  = -1; // the coefficients of variable
				ijacount++; } 
			else 
				constants[mask_count] += img[r * height + yc];

			if (mask[x * height + bm] > 0) { // BOTTOM
				ija_p[ijacount] = mask_ind[x * height + bm]; // the pointer to index
				coefficients[ijacount]  = -1; // the coefficients of variable
				ijacount++; } 
			else 
				constants[mask_count] += img[xc * height + b];
			
			mask_count++;
		}
	}

	ija_p[mask_num] = ijacount;
	coefficients[mask_num] = 0.0;

	/////////////////////////////////////////////////////////
	// 2. Poisson Solver
	int iter = 0;
	double err = 0.0;

	linbcg(ija_p, coefficients, constants, edit_result, 1, 1E-8, 1000, iter, err, mask_num);

	/*for(x = 0; x < mask_num + 1; x++)
		cout << edit_result[x] << endl;*/

	////////////////////////////////////////////////////////
	// 3. Copy the result
	mask_count = 0;
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(mask[x * height + y] > 0)
			{
				xc = x;
				yc = y;
				result[xc * height + yc] = edit_result[mask_count];
				mask_count++;
			}
		}
	}


	delete[] constants;
	delete[] edit_result;
	delete[] coefficients;
	delete[] ija_p;
	delete[] mask_ind;
}


void ImageInpainting::Laplacian(const double* img, int width, int height, double* result)
{
	int x,y;
	int left, right, up, down;
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			left = (x-1<0)? 0: x-1;
			right = (x+1>width-1)? width-1: x+1;
			up = (y-1<0)? 0: y-1;
			down = (y+1>height-1)? height-1: y+1;

			result[x * height + y] = -img[right * height + y] - img[left * height + y] - 
				img[x * height + down] - img[x * height + up] + 4 * img[x * height + y];
		}
	}

}
