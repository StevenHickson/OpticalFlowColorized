#include "OpticalFlow.h"

using namespace cv;
using namespace cv::gpu;
using namespace std;

void ColorizeFlow(const Mat &mag, const Mat &angle, Mat &flow) {
	Mat filler = cv::Mat(mag.rows, mag.cols, CV_8UC1, Scalar(255));
	vector<Mat> merging(3);
	merging[0] = angle;
	merging[1] = filler;
	merging[2] = mag;
	Mat hsv;
	merge(merging, hsv);
	cvtColor(hsv, flow, CV_HSV2BGR);
}

void ComputeOpticalFlow(const Mat &past, const Mat &current, Mat &flow) {
	Mat in1, in2, flow2d;
	cvtColor(past, in1, CV_BGR2GRAY);
	cvtColor(current, in2, CV_BGR2GRAY);
	calcOpticalFlowFarneback(in1, in2, flow2d, 0.5f, 2, 5, 2, 7, 1.5, 0);
	vector<Mat> channels(2);
	split(flow2d, channels);
	Mat mag, angle;
	cartToPolar(channels[0], channels[1], mag, angle);
	float mulVal = 90 / 3.131592763f;
	angle *= mulVal;
	normalize(mag, mag, 0, 255, CV_MINMAX);
	mag.convertTo(mag, CV_8UC1);
	angle.convertTo(angle, CV_8UC1);
	ColorizeFlow(mag, angle, flow);
}

void ComputeOpticalFlowGPU(const Mat &past, const Mat &current, Mat &flow) {
	try {
		Mat in1, in2;
		cvtColor(past, in1, CV_BGR2GRAY);
		cvtColor(current, in2, CV_BGR2GRAY);
		GpuMat d_frameL(in1), d_frameR(in2);
		GpuMat d_flowx, d_flowy, d_mag, d_angle, d_magNorm;
		cv::gpu::FarnebackOpticalFlow calc_flow;
		//Mat flowx, flowy;
		Mat mag, angle;
		calc_flow(d_frameL, d_frameR, d_flowx, d_flowy);
		//d_flowx.download(flowx);
		//d_flowy.download(flowy);
		gpu::cartToPolar(d_flowx, d_flowy, d_mag, d_angle);
		float mulVal = 90 / 3.131592763f;
		gpu::multiply(d_angle, mulVal, d_angle);
		gpu::normalize(d_mag, d_magNorm, 0, 255, CV_MINMAX);

		/*Mat_<float>::iterator pInX = flowx.begin<float>();
		Mat_<float>::iterator pInY = flowy.begin<float>();
		for (int j = 0; j < flowx.rows; j++) {
			for (int i = 0; i < flowx.cols; i++) {
				++pInX; ++pInY;
			}
		}*/
		d_magNorm.download(mag);
		d_angle.download(angle);
		mag.convertTo(mag, CV_8UC1);
		angle.convertTo(angle, CV_8UC1);
		ColorizeFlow(mag, angle, flow);
	}
	catch (cv::Exception &e) {
		cout << e.what() << endl;
	}
	catch (std::exception &e) {
		cout << e.what() << endl;
	}
	catch (...) {

	}
}


void Downsample2x2(const Mat &in, Mat &out) { resize(in, out, Size(), 0.5f, 0.5f); }