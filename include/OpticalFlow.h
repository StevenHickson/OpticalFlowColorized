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

#ifndef FLOW_H
#define FLOW_H

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/gpu/gpu.hpp"

template <typename T> inline T Clamp(T a, T minn, T maxx)
{ return (a < minn) ? minn : ( (a > maxx) ? maxx : a ); }

inline int Round (float a)  
{
	assert( !_isnan( a ) );
	return static_cast<int>(a>=0 ? a+0.5f : a-0.5f); 
} 


void ComputeOpticalFlow(const cv::Mat &past, const cv::Mat &current, cv::Mat &flow);
void ComputeOpticalFlowGPU(const cv::Mat &past, const cv::Mat &current, cv::Mat &flow);
void Downsample2x2(const cv::Mat &in, cv::Mat &out);

#endif //FLOW_H