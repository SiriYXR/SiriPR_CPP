#ifndef SIRIPR_API_H_
#define SIRIPR_API_H_


#include "siripr/core/plate_recognize.h"

#include <string>

namespace siripr {
	namespace api {

		/*
  		实现车牌识别对象的初始化。该函数主要实现了一些基本参数的设定和机器学习模型的加载。

		参数：
			lifemod：识别的模式（是否为生活模式）
			maxplates：单张图像中识别车牌数量的上限。

		返回值：
			空

		*/
		void initPR(bool lifemod = true, int maxplates = 1);
		
		/*
		实现对单张图像的车牌识别。不过识别结果最多只有一张车牌。

		参数：
			src：图像矩阵
			x：识别到的车牌的X坐标
			y：识别到的车牌的y坐标
			w：识别到的车牌的宽
			h：识别到的车牌的高
			isDebug：是否开启调试模式

		返回值：
			车牌识别结果信息
		*/
		string ImgRecognize(Mat src, int& x, int& y, int& w, int& h, bool isDebug = false);
	}
}

#endif //SIRIPR_API_H_