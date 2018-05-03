#include <math.h>
#include <algorithm>
#include"ImageInpainting.h"
using namespace std;

void ImageInpainting::getBoundary()
{
	int x,y;
	int i,j;
	const double* mask = mask_input;
	const double* face_mask = face_mask_input;

	memset(blend_boundary, 0, width * height * sizeof(double));
	memset(face_boundary, 0, width * height * sizeof(double));
	memset(boundary, 0, width * height * sizeof(double));


	bool flag_facebound = false;

	// Get the boundary point
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			flag_facebound = false;
			// get 4 neighbors
			int left = max(x - 1, 0);
			int right = min(x + 1, width - 1);
			int up = max(y - 1, 0);
			int down = min(y + 1, height - 1);

			// 1. face boundary
			if( !(face_mask[x * height + y] == face_mask[left * height + y] &&
				face_mask[left * height + y] == face_mask[right * height + y] &&
				face_mask[right * height + y] == face_mask[x * height + up] &&
				face_mask[x * height + up] == face_mask[x * height + down]) )
			{
				face_boundary[x * height + y] = 1;
				flag_facebound = true;
			}


			// 2. mask boundary
			if( !(mask[x * height + y] == mask[left * height + y] &&
				mask[left * height + y] == mask[right * height + y] &&
				mask[right * height + y] == mask[x * height + up] &&
				mask[x * height + up] == mask[x * height + down]) )
			{
				// the 4 neighbors have different sign, it is a boundary
				if(mask[x * height + y] > 0)
					// the boundary
					boundary[x * height + y] = 1;
			}
		}
	}
}