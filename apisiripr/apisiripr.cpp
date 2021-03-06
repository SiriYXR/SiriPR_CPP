// apisiripr.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "include\apisiripr.h"
#include "siripr\api\api.h"

namespace siripr {
	namespace api {
		void apiInitPR(bool lifemod , int maxplates ) {
			initPR(lifemod, maxplates);
		}

		string apiImgRecognize(Mat src, int& x, int& y, int& w, int& h, bool isDebug) {
			return ImgRecognize(src, x, y, w, h, isDebug);
		}
	}
}

char* apiImgRecognize_C(IplImage src, int& x, int& y, int& w, int& h, int isDebug) {
	Mat t_mat = cvarrToMat(&src);
	siripr::api::initPR();
	string t_string= siripr::api::ImgRecognize(t_mat, x, y, w, h, isDebug);
	char t_char[256];
	strcpy(t_char, t_string.c_str());
	return t_char;
}