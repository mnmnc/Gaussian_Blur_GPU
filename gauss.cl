// GAUSSIAN BLUR
__kernel void gaussian_blur(__global const unsigned char *image, 
							__global const float* G, 
							const int W,
							const int H,
							const int size, 
							__global unsigned char* newImg) 
{ 
	// VARIABLES
	unsigned int x, y, imgLineSize;
	float value;
	int i, xOff, yOff, center;

    // GET INDEX OF CURRENT ELEMENT
    i = get_global_id(0);

    // CALCULATE VARIABLES
	imgLineSize = W * 4;
	center = size / 2;

	// PROCESS THE PIXEL
	if(i >= imgLineSize * (size-center) + center * 4 &&
	   i <  W * H * 4 - imgLineSize * (size-center) - center * 4)
	{
		value=0;
		for(y = 0; y < size; y++)
        {
           	yOff = imgLineSize * (y-center);
           	for(x = 0; x < size; x++)
           	{
           	    xOff = 4 * (x-center);
           	    value += G[y * size + x] * image[i + xOff + yOff];
           	}
        }
        newImg[i] = value;
	}
	else // VALUE REMAINS IF WE ARE NEAR THE EDGE
	{
		newImg[i] = image[i];
	}
}