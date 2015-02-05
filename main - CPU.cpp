#include <iostream>
#include <string>
#include "lodepng.h"
#include <math.h>
#include <ctime>

#define DIM 5

using namespace std;

std::vector<unsigned char> decodeOneStep(const char* filename)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return image;
}

vector<double> create_convolution_matrix(double sigma){
	/*
	Creates convolution matrix for Gaussian blur given sigma and dimention of the filter.
	*/


	int W = DIM;
	double kernel[DIM][DIM];
	vector<double> result;
	double mean = W / 2;
	double sum = 0.0; // For accumulating the kernel values
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < W; ++y) {
		kernel[x][y] =
			exp(-0.5 * (pow((x - mean) / sigma, 2.0) +
			pow((y - mean) / sigma, 2.0))) /
			(2 * 3.14159 * sigma * sigma);

		// Accumulate the kernel values
		sum += kernel[x][y];
		}

	// Normalize the kernel
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < W; ++y)
			kernel[x][y] /= sum;

	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < W; ++y) {
			result.push_back(kernel[x][y]);
		}
	}
	return result;
}

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

std::vector<unsigned char> paint_pixel_white(std::vector<unsigned char> image, int x){
	// Function helpful for debugig
	std::vector<unsigned char> nimage = image;
	int ref = x * 4;

	for (int i = 0; i < 3; ++i){
		nimage[ref + i] = 255;
	}
	return nimage;
}

std::vector<unsigned char> get_rgb_values_by_mask(std::vector<unsigned char> image, int width, int height, int x, int mask_size, vector<double> matrix){
	/*
	Calculates RGB values of given pixel based on gaussian convolution matrix

	Example for mask_size = 5

	ref_a . . . .
	. . . . .
	. . x . .
	. . . . .
	. . . . .
	*/

	// Will be returned
	std::vector<unsigned char> rgb;

	// Calculate top left corner
	int index = x - (((int)(mask_size / 2)) * width) - ((int)(mask_size / 2));
	int ref_a = index * 4;

	// partial solutions variables
	double r = 0;
	double g = 0;
	double b = 0;

	// Calculate new RGB values based on pixel surroundings
	for (int row = 0; row < mask_size; ++row){
		for (int mask_position = 0; mask_position < mask_size; ++mask_position){

			// Get common multiplier from convolution matrix
			double mul = matrix[row * mask_size + mask_position];

			// RED
			if (ref_a + ((width * 4) * row) + (mask_position * 4) > 0 && ref_a + ((width * 4) * row) + (mask_position * 4) < width * height * 4){
				r += image[ref_a + ((width * 4) * row) + (mask_position * 4)] * mul;
			}

			// GREEN
			if (ref_a + ((width * 4) * row) + (mask_position * 4) + 1 > 0 && ref_a + ((width * 4) * row) + (mask_position * 4) + 1 < width * height * 4){
				g += image[ref_a + ((width * 4) * row) + (mask_position * 4) + 1] * mul;
			}

			// BLUE
			if (ref_a + ((width * 4) * row) + (mask_position * 4) + 2 > 0 && ref_a + ((width * 4) * row) + (mask_position * 4) + 2 < width * height * 4){
				b += image[ref_a + ((width * 4) * row) + (mask_position * 4) + 2] * mul;
			}
		}
	}

	rgb.push_back((unsigned char)r);
	rgb.push_back((unsigned char)g);
	rgb.push_back((unsigned char)b);

	return rgb;
}


int main(){

	// VARIABLES
	char * filename = "3.png";
	char * filename_out = "4.png";
	int width = 678;
	int height = 353;

	// GAUSSIAN VARIABLES
	double gauss_sigma = 1;
	int gauss_filter_dimention = DIM;

	// ORIGINAL IMAGE
	std::vector<unsigned char> image = decodeOneStep(filename);

	cout << "[NFO] IMAGE READ." << endl;

	// IMAGE AFTER PROCESSING
	std::vector<unsigned char> nimage = image;

	// CREATE CONVOLUTION MATRIX
	vector<double> matrix = create_convolution_matrix(gauss_sigma);

	cout << "[NFO] CONVOLUTION MATRIX CREATED." << endl;

	// CAPTURE TIME
	clock_t begin_pt = clock();

	// PROCESS IMAGE
	for (int i = 0; i < width * height; ++i){
		std::vector<unsigned char> rgb = get_rgb_values_by_mask(image, width, height, i,
			gauss_filter_dimention,
			matrix);
		// ALTER IMAGE
		nimage[i * 4 + 0] = rgb.at(0);
		nimage[i * 4 + 1] = rgb.at(1);
		nimage[i * 4 + 2] = rgb.at(2);

		if (i % 10000 == 0){
			cout << i << endl;
		}
	}
	std::cout << "Time spent on bluring: " << double(clock() - begin_pt) / CLOCKS_PER_SEC << endl;

	// SAVE IMAGE
	encodeOneStep(filename_out, nimage, width, height);


	system("PAUSE");
	return 0;
}
