#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\core.hpp>

using namespace cv;

/***
Apply Wiener filter to the input image.
Input: Source image; rows and cols of the kernel used to estimate local mean and variance.
Output: Result filtered image.
***/
Mat WienerFilter(Mat img, int kernel_rows, int kernel_cols)
{
	Mat dst;
	Scalar noise_power;

	// weighted average kernel
	Mat kernel(kernel_rows, kernel_cols, CV_32F);
	kernel = 1.0 / (double)(kernel_rows * kernel_cols);
	
	Mat src_img_f;
	img.convertTo(src_img_f, CV_32F);
	std::vector<Mat> rgbChannels(3);
	std::vector<Mat> rgbDest(3);
	split(src_img_f, rgbChannels);

	for (int i = 0; i < 3; i++)
	{
		Mat tmp_mat_1(img.rows, img.cols, img.type());
		Mat tmp_mat_2 = tmp_mat_1.clone();
		Mat tmp_mat_3 = tmp_mat_1.clone();
		Mat tmp_mat_4 = tmp_mat_1.clone();

		// local mean
		filter2D(rgbChannels[i], tmp_mat_1, rgbChannels[i].depth(), kernel, cvPoint(-1, -1), 0, BORDER_CONSTANT);
		
		// squared filtered input
		multiply(rgbChannels[i], rgbChannels[i], tmp_mat_2);
		filter2D(tmp_mat_2, tmp_mat_3, tmp_mat_2.depth(), kernel, cvPoint(-1, -1), 0, BORDER_CONSTANT);
		
		// estimate local variance as the difference between squared filtered input and squared local mean
		multiply(tmp_mat_1, tmp_mat_1, tmp_mat_4);
		subtract(tmp_mat_3, tmp_mat_4, tmp_mat_3);

		// noise power is estimated as the average of all the local variances
		noise_power = mean(tmp_mat_3);

		// apply Wiener algorithm
		// output_pixel = local_mean  + (input_pixel - local_mean) * (max(0, localVar - noise) / max(localVar, noise))
		subtract(rgbChannels[i], tmp_mat_1, rgbDest[i]);
		max(tmp_mat_3, noise_power, tmp_mat_2);
		subtract(tmp_mat_3, noise_power, tmp_mat_3);
		max(tmp_mat_3, 0, tmp_mat_3);
		divide(tmp_mat_3, tmp_mat_2, tmp_mat_3);
		multiply(tmp_mat_3, rgbDest[i], rgbDest[i]);
		add(rgbDest[i], tmp_mat_1, rgbDest[i]);
	}
	merge(rgbDest, dst);
	dst.convertTo(dst, CV_8U);

	return dst;
}



int main()
{
	Mat input_image = imread("..\\apple.jpg");
	
	if (input_image.empty())
	{
		std::cout << "Could not read the image." << std::endl;
		return 1;
	}
	
	Mat filtered_image = WienerFilter(input_image, 5, 5);

	imshow("Original", input_image);
	imshow("Wiener Filter", filtered_image);
	waitKey();

	return 0;
}

