#ifndef MM3D_H
#define MM3D_H

#include <math.h>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <algorithm>

using namespace std;
using namespace cv;

void ZBuffer(double* vertex, double* tri, double* texture, int nver, int ntri, double* src_img, int width, int height, int nChannels, double* img, double* tri_ind);
void ZBufferTri(double* vertex, double* tri, double* texture_tri, int nver, int ntri, double* src_img, int width, int height, int nChannels, double* img, double* tri_ind);
void OcclusionQuery(double* vertex, double* tri, int ntri, int nver, int width, int height, double* visibility, double threshold);



bool PointInTri(Mat* point, Mat* pt1, Mat* pt2, Mat* pt3);

#endif