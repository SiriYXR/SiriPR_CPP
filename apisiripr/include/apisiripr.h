#ifndef SIRIPR_APISIRIPR_H_
#define SIRIPR_APISIRIPR_H_

#include "opencv2\opencv.hpp"
#include <string>

namespace siripr {
	namespace api {
		void apiInitPR(bool lifemod = true, int maxplates = 1);
		std::string apiImgRecognize(cv::Mat src, int& x, int& y, int& w, int& h, bool isDebug = false);
	}
}

extern "C" {
	char* apiImgRecognize_C(IplImage src, int& x, int& y, int& w, int& h, int isDebug = false);
}
#endif	//SIRIPR_APISIRIPR_H_