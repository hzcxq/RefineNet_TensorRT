#include <opencv2/opencv.hpp>
#include "image.hpp"

static const float kMean[3] = { 0.485f, 0.456f, 0.406f };
static const float kStdDev[3] = { 0.229f, 0.224f, 0.225f };
static const int map_[11][3] = { {0,0,0} ,
							{128,0,0},
							{0,128,0},
							{0,0,128},
							{128,128,0},
							{128,0,128},
                            {0,128,0},
                            {0,128,128},
                            {0,255,0},
                            {0,0,255},
                            {255,0,0} };


float* normal(cv::Mat img) {
	//cv::Mat image(img.rows, img.cols, CV_32FC3);
	float * data;
	data = (float*)calloc(img.rows*img.cols * 3, sizeof(float));

	for (int c = 0; c < 3; ++c)
	{
		for (int i = 0; i < img.rows; ++i)
		{ //获取第i行首像素指针 
			cv::Vec3b *p1 = img.ptr<cv::Vec3b>(i);
			//cv::Vec3b *p2 = image.ptr<cv::Vec3b>(i);
			for (int j = 0; j < img.cols; ++j)
			{
				data[c * img.cols * img.rows + i * img.cols + j] = (p1[j][c] / 255. - kMean[c]) / kStdDev[c];
			}
		}
	}
	return data;
}



cv::Mat read2mat(float * prob,cv::Mat out)
{
	for (int i = 0; i < 128; ++i)
	{
        cv::Vec<float, 11> *p1 = out.ptr<cv::Vec<float, 11>>(i);
		for (int j = 0; j < 128; ++j)
		{
            for (int c = 0; c < 11; ++c)
			{
				p1[j][c] = prob[c * 128 * 128 + i * 128 + j];
			}
		}
	}
	return out;
}


cv::Mat map2threeunchar(cv::Mat real_out,cv::Mat real_out_)
{
	for (int i = 0; i < 512; ++i)
	{
        cv::Vec<float, 11> *p1 = real_out.ptr<cv::Vec<float, 11>>(i);
		cv::Vec3b *p2 = real_out_.ptr<cv::Vec3b>(i);
		for (int j = 0; j < 512; ++j)
		{
			int index = 0;
			float swap;
            for (int c = 0; c < 11; ++c)
			{
				if (p1[j][0] < p1[j][c])
				{
					swap = p1[j][0];
					p1[j][0] = p1[j][c];
					p1[j][c] = swap;
					index = c;
				}
			}
			p2[j][0] = map_[index][2];
			p2[j][1] = map_[index][1];
			p2[j][2] = map_[index][0];

		}
	}
	return real_out_;
}

