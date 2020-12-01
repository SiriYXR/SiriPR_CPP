// pydsiripr.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include<string>
#include<boost/python.hpp>

#include "conversion.h"
#include "siripr/api/api.h"
#include "siripr/train/svm_train.h"====
#include "siripr/train/svm_train.h"
#include "siripr/util/util.h"
#include "siripr/config.h"

using namespace std;
using namespace boost::python;

namespace siripr {
	PyObject* apiImgRecognize(PyObject * src, int isDebug = 0) {

		NDArrayConverter cvt;
		cv::Mat cvmat(cvt.toMat(src));
		int x, y, w, h;
		x = y = w = h = 0;

		string str = Utils::string_To_UTF8(api::ImgRecognize(cvmat, x, y, w, h, isDebug));

		PyObject *reList = PyList_New(0);
		PyList_Append(reList, PyUnicode_FromString(str.c_str()));
		PyList_Append(reList, PyLong_FromLong(x));
		PyList_Append(reList, PyLong_FromLong(y));
		PyList_Append(reList, PyLong_FromLong(w));
		PyList_Append(reList, PyLong_FromLong(h));

		return reList;
	}

	class apiCPlateRecognize :public CPlateRecognize {
	public:

		apiCPlateRecognize() {};
		apiCPlateRecognize(const siripr::apiCPlateRecognize & apiCPR) {};
		~apiCPlateRecognize() {};

		PyObject * plateRecognize_py(PyObject * src) {

			NDArrayConverter cvt;
			cv::Mat cvmat(cvt.toMat(src));

			std::vector<CPlate> plateVec;

			CPlateRecognize::plateRecognize(cvmat, plateVec);

			PyObject *reList = PyList_New(0);

			for (auto ite : plateVec) {
				PyObject *iteList = PyList_New(0);

				string plateLicense = Utils::string_To_UTF8((ite.getPlateStr()));
				RotatedRect rrect = ite.getPlatePos();
				int x = rrect.boundingRect().x;
				int y = rrect.boundingRect().y;
				int w = rrect.boundingRect().width;
				int h = rrect.boundingRect().height;

				PyList_Append(iteList, PyUnicode_FromString(plateLicense.c_str()));
				PyList_Append(iteList, PyLong_FromLong(x));
				PyList_Append(iteList, PyLong_FromLong(y));
				PyList_Append(iteList, PyLong_FromLong(w));
				PyList_Append(iteList, PyLong_FromLong(h));

				PyList_Append(reList, iteList);
			}

			return reList;
		}

		void setDetectType(int param) {

			switch (param)
			{
			case 0:
				CPlateRecognize::setDetectType(PR_DETECT_SOBEL);
				break;
			case 1:
				CPlateRecognize::setDetectType(PR_DETECT_COLOR);
				break;
			case 2:
				CPlateRecognize::setDetectType(PR_DETECT_CMSER);
				break;
			case 3:
				CPlateRecognize::setDetectType(PR_DETECT_SOBEL | PR_DETECT_COLOR);
				break;
			case 4:
				CPlateRecognize::setDetectType(PR_DETECT_SOBEL | PR_DETECT_CMSER);
				break;
			case 5:
				CPlateRecognize::setDetectType(PR_DETECT_COLOR | PR_DETECT_CMSER);
				break;
			case 6:
				CPlateRecognize::setDetectType(PR_DETECT_SOBEL | PR_DETECT_COLOR | PR_DETECT_CMSER);
				break;
			default:
				break;
			}

		}

		void setMaxPlates(int param) {
			CPlateRecognize::setMaxPlates(param);
		}

	};

	class apiSvmTrain :public SvmTrain {

	public:
		apiSvmTrain():SvmTrain(nullptr, nullptr) {}
		apiSvmTrain(std::string plates_folder, std::string xml) :SvmTrain(plates_folder, xml) {}

		void train() {
			Py_BEGIN_ALLOW_THREADS
			SvmTrain::train();
			Py_END_ALLOW_THREADS
		}

		void setRounds(int param) {
			SvmTrain::setRounds(param);
		}
	};
}

BOOST_PYTHON_FUNCTION_OVERLOADS(apiImgRecognize, siripr::apiImgRecognize, 1, 2);

BOOST_PYTHON_MODULE(siripr)
{
	def("ImgRecognize", siripr::apiImgRecognize, apiImgRecognize());

	class_<siripr::apiCPlateRecognize>("CPlateRecognize")
		.def("plateRecognize", &siripr::apiCPlateRecognize::plateRecognize_py)
		.def("setLifemode", &siripr::apiCPlateRecognize::setLifemode)
		.def("setDetectType", &siripr::apiCPlateRecognize::setDetectType)
		.def("setMaxPlates", &siripr::apiCPlateRecognize::setMaxPlates)
		.def("setDebug", &siripr::apiCPlateRecognize::setDebug)
		.def("LoadSVM", &siripr::apiCPlateRecognize::LoadSVM)
		.def("LoadANN", &siripr::apiCPlateRecognize::LoadANN)
		.def("LoadChineseANN", &siripr::apiCPlateRecognize::LoadChineseANN)
		.def("LoadGrayChANN", &siripr::apiCPlateRecognize::LoadGrayChANN)
		.def("LoadChineseMapping", &siripr::apiCPlateRecognize::LoadChineseMapping);

	class_<siripr::apiSvmTrain>("SvmTrain")
		.def(init<std::string,std::string>())
		.def("train",&siripr::apiSvmTrain::train)
		.def("setRounds", &siripr::apiSvmTrain::setRounds);
}