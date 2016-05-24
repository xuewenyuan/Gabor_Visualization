#include <stdio.h>
#include <tchar.h>
#include <limits>

// OpenCV
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


int m_FrameNr;



float GaborFilterValue(int x,
	int y,
	float lambda,
	float theta,
	float phi,
	float sigma,
	float gamma) {
	float xx = x * cos(theta) + y * sin(theta);
	float yy = -x * sin(theta) + y * cos(theta);

	float envelopeVal = exp(-((xx*xx + gamma*gamma* yy*yy) / (2.0f * sigma*sigma)));

	float carrierVal = cos(2.0f * (float)M_PI * xx / lambda + phi);

	float g = envelopeVal * carrierVal;

	return g;
}


double round(double d) {
	return floor(d + 0.5);
}



void CreateGaborFilterImage(char* demoName, float wavelength, float orientation, float phaseoffset, float gaussvar, float aspectratio) {
	// Create an empty image
	int ImgWidth = 725;
	int ImgHeight = 725;
	IplImage* img = cvCreateImage(cvSize(ImgWidth, ImgHeight), IPL_DEPTH_8U, 3);
	for (int y = 0; y<ImgHeight; y++)
	for (int x = 0; x<ImgWidth; x++) {
		((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 0] = 0; // B
		((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 1] = 0; // G
		((uchar *)(img->imageData + y*img->widthStep))[x*img->nChannels + 2] = 0; // R
	}


	int winSize = 300;
	int my = ImgHeight / 2;
	int mx = ImgWidth / 2;


	// Display gabor filter values
	float MinGaborValue = std::numeric_limits<float>::max();
	float MaxGaborValue = std::numeric_limits<float>::min();
	for (int dy = -winSize; dy <= winSize; dy++) {
		for (int dx = -winSize; dx <= winSize; dx++) {
			float val = GaborFilterValue(dx, dy, wavelength, orientation, phaseoffset, gaussvar, aspectratio);

			int R = 0;
			int G = 0;
			int B = 0;
			CvScalar s;
			if (val>0.0f)
				R = (int)round(val * 255.0f);
			else
				B = (int)round(-val * 255.0f);
			s.val[0] = B;
			s.val[1] = G;
			s.val[2] = R;
			cvSet2D(img, my + dy, mx + dx, s);

			if (val<MinGaborValue)
				MinGaborValue = val;
			if (val>MaxGaborValue)
				MaxGaborValue = val;

		}
	}



	// Display gabor filter parameters
	CvFont font;
	double hScale = 1.0;
	double vScale = 1.0;
	int    lineWidth = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, hScale, vScale, 0, lineWidth);
	char txt[100];
	cvPutText(img, "Gabor filter values visualization by Juergen Mueller.", cvPoint(10, 20), &font, cvScalar(0, 255, 255));
	cvPutText(img, "variable naming conventions as used by http://en.wikipedia.org/wiki/Gabor_filter.", cvPoint(10, 50), &font, cvScalar(0, 255, 255));
	cvPutText(img, "Red: positive values. Blue: negative values", cvPoint(10, 70), &font, cvScalar(0, 255, 255));


	cvPutText(img, demoName, cvPoint(10, 100), &font, cvScalar(0, 255, 0));
	sprintf_s(txt, "1. wavelength  (lambda) : %.2f", wavelength);    cvPutText(img, txt, cvPoint(10, 120), &font, cvScalar(0, 255, 255));
	sprintf_s(txt, "2. orientation (theta)  : %.2f", orientation);   cvPutText(img, txt, cvPoint(10, 140), &font, cvScalar(0, 255, 255));
	sprintf_s(txt, "3. gaussvar    (sigma)  : %.2f", gaussvar);      cvPutText(img, txt, cvPoint(10, 160), &font, cvScalar(0, 255, 255));
	sprintf_s(txt, "4. phaseoffset (phi)    : %.2f", phaseoffset);   cvPutText(img, txt, cvPoint(10, 180), &font, cvScalar(0, 255, 255));
	sprintf_s(txt, "5. aspectratio (gamma)  : %.2f", aspectratio);   cvPutText(img, txt, cvPoint(10, 200), &font, cvScalar(0, 255, 255));

	sprintf_s(txt, "min gabor val : %.2f", MinGaborValue);   cvPutText(img, txt, cvPoint(10, 230), &font, cvScalar(0, 255, 255));
	sprintf_s(txt, "max gabor val : %.2f", MaxGaborValue);   cvPutText(img, txt, cvPoint(10, 250), &font, cvScalar(0, 255, 255));



	// Show gabor filter image
	cvShowImage("Gaborfilter", img);



	// Wait for a key press 
	int key = cvWaitKey(1);


	// Save current image to create an animation based on the sequence of such images
	char filename[500];
	sprintf_s(filename, "D:\\tmp\\frame_%05d.png", m_FrameNr);
	cvSaveImage(filename, img);
	m_FrameNr++;



	cvReleaseImage(&img);
}



int _tmain(int argc, _TCHAR* argv[]) {
	int key = 0;
	m_FrameNr = 0;

	// 1. Create display window
	cvNamedWindow("Gaborfilter", CV_WINDOW_AUTOSIZE);


	// 2. Gabor parameter sweep

	float wavelength;
	float orientation;
	float gaussvar;
	float phaseoffset;
	float aspectratio;

	wavelength = 40.0f;
	orientation = (float)M_PI / 4.0f;
	gaussvar = 20.0f;
	phaseoffset = 0.0f;
	aspectratio = 0.5f;
	for (wavelength = 5.0f; wavelength <= 100.0f; wavelength += 5.0f) {
		CreateGaborFilterImage("1. Wavelength is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
	}


	wavelength = 40.0f;
	orientation = (float)M_PI / 4.0f;
	gaussvar = 20.0f;
	phaseoffset = 0.0f;
	aspectratio = 0.5f;
	for (orientation = 0.0f; orientation <= 2 * (float)M_PI; orientation += 2 * (float)M_PI / 8.0f) {
		CreateGaborFilterImage("2. Orientation is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
	}


	wavelength = 40.0f;
	orientation = (float)M_PI / 4.0f;
	gaussvar = 20.0f;
	phaseoffset = 0.0f;
	aspectratio = 0.5f;
	for (gaussvar = 10.0f; gaussvar <= 60; gaussvar += 5) {
		CreateGaborFilterImage("3. Gaussvariance is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
	}



	wavelength = 40.0f;
	orientation = (float)M_PI / 4.0f;
	gaussvar = 20.0f;
	phaseoffset = 0.0f;
	aspectratio = 0.5f;
	for (phaseoffset = 0.0f; phaseoffset <= 2 * (float)M_PI; phaseoffset += 2 * (float)M_PI / 16.0f) {
		CreateGaborFilterImage("4. Phase offset is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
	}



	wavelength = 40.0f;
	orientation = (float)M_PI / 4.0f;
	gaussvar = 20.0f;
	phaseoffset = 0.0f;
	aspectratio = 0.5f;
	for (aspectratio = 0.1f; aspectratio <= 1.0f; aspectratio += .05f) {
		CreateGaborFilterImage("5. Aspect ratio is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
	}


	return 0;
}