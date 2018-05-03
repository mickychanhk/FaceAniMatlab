#include "DebugFunc.h"

void ImageShow(const double* img, int width, int height, int nChannels)
{
	IplImage* im = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, nChannels);

	int x,y,n;
	// Show Results
	for(y = 0; y < height; y++)
	{
		unsigned char* ptr = (unsigned char*)(im->imageData + y * im->widthStep);
		for(x = 0; x < width; x++)
		{
			if(nChannels == 1)
			{
				n = 0;
				int pixel = (int)img[n * width * height + x * height + y];
				pixel = (pixel >= 0)? pixel: 0;
				pixel = (pixel <= 255)? pixel: 255;
				ptr[nChannels * x + n] = pixel;
			}
			else
			{
				for(n = 0; n < nChannels; n++)
				{
					int pixel = (int)img[n * width * height + x * height + y] ;
					pixel = (pixel >= 0)? pixel: 0;
					pixel = (pixel <= 255)? pixel: 255;
					ptr[nChannels * x + (2-n)] = pixel;
				}	
			}

			
		}
	}

	cvNamedWindow("show");
	cvShowImage("show", im);
	cvWaitKey(0);

	cvSaveImage("temp.bmp", im);

	cvDestroyWindow("show");
	cvReleaseImage(&im);
}