#include <math.h>
#include <algorithm>
#include "ImageInpainting.h"
#include "linsys.h"
using namespace std;
#define NEI_LIST_NUM 9

void ImageInpainting::Blurring4Detail(double* des, const double* src, double blend_width)
{
	int x,y,n;
	int i,j;
	double sum[MAXNCHANNELS],num;
	memcpy(des, src, width * height * nChannels * sizeof(double));

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(boundary[x * height + y] > 0 && face_mask_input[x * height + y] > 0)
			{
				// Blur
				memset(sum, 0, MAXNCHANNELS * sizeof(double));
				num = 0;
				for(i = max(x - (int)blend_width,0); i <= min(x + (int)blend_width,width-1); i++)
				{
					for(j = max(y - (int)blend_width,0); j <= min(y + (int)blend_width,height-1); j++)
					{
						if(face_mask_input[i * height + j] > 0)
						{
							for(n = 0; n < nChannels; n++)
								sum[n] += src[n * width * height + i * height + j];
							num++;
						}
					}
				}
				for(n = 0; n < nChannels; n++)
					des[n * width * height + x * height + y] = sum[n] / num;
			}
		}
	}
}
