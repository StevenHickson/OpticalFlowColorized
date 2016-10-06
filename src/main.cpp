/*
Copyright (C) 2014 Steven Hickson

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
// TestVideoSegmentation.cpp : Defines the entry point for the console application.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include "OpticalFlow.h"


using namespace std;
using namespace cv;

void Display2dOF(Mat flow) {
	cv::Mat xy[2]; //X,Y
	cv::split(flow, xy);

	//calculate angle and magnitude
	cv::Mat magnitude, angle;
	cv::cartToPolar(xy[0], xy[1], magnitude, angle, true);

	//translate magnitude to range [0;1]
	double mag_min, mag_max;
	cv::minMaxLoc(magnitude, &mag_min, &mag_max);
	magnitude.convertTo(magnitude, -1, 1.0/mag_max);

	//build hsv image
	cv::Mat _hsv[3], hsv;
	_hsv[0] = angle;
	_hsv[1] = Mat::ones(angle.size(), CV_32F);
	_hsv[2] = magnitude;
	cv::merge(_hsv, 3, hsv);

	//convert to BGR and show
	Mat bgr;//CV_32FC3 matrix
	cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
	imshow("Optical flow",bgr);
}

void ComputeAllFlow(string fileName) {

}

int main (int argc, char** argv) {
	try {
		Mat img1 = imread(argv[1]);
		Mat img2 = imread(argv[2]);
		Mat flow;
		int run = atoi(argv[3]);
		if (run == 0)
			ComputeOpticalFlowGPU(img1, img2, flow);
		else if (run == 1)
			ComputeOpticalFlow(img1, img2, flow);
		else {

		}
		//imshow("img", img1);
		//imshow("flow", flow);
		//waitKey(0);
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	cout << "Done" << endl;
	cin.get();
	return 0;
}