#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

#define PI 3.14159265358979323846

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	
	namedWindow("gabor");

	int sigma_bar = 40;
	createTrackbar("sigma", "gabor", &sigma_bar, 1000, nullptr, nullptr);

	int theta_bar = 40;
	createTrackbar("theta", "gabor", &theta_bar, 1000, nullptr, nullptr);

	int lambda_bar = 40;
	createTrackbar("lambda", "gabor", &lambda_bar, 1000, nullptr, nullptr);

	int gamma_bar = 40;
	createTrackbar("gamma", "gabor", &gamma_bar, 1000, nullptr, nullptr);

	int psi_bar = 40;
	createTrackbar("psi", "gabor", &psi_bar, 1000, nullptr, nullptr);

	const int kernel_size = 128;

	for (; ; ) {
		auto gabor_mat = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sigma_bar/10. ,theta_bar/180.*PI, lambda_bar, gamma_bar/100., psi_bar/180.*PI);

		normalize(gabor_mat, gabor_mat, 1, 0, cv::NORM_MINMAX);

		Mat image;
		resize(gabor_mat, image, cv::Size(512, 512));

		imshow("gabor", image);
		int key = cv::waitKey(20);
		if (key == 'q') {
			break;
		}
	}
	
	return 0;
}