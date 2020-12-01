#include "siripr/core/plate_recognize.h"
#include "siripr/config.h"
#include "thirdparty/textDetect/erfilter.hpp"

namespace siripr {

	CPlateRecognize::CPlateRecognize() {
		m_showResult = false;
		m_uniformSizes = false;
	}

	CPlateRecognize::~CPlateRecognize()
	{
	}

	// main method, plate recognize, contain two parts
	// 1. plate detect
	// 2. chars recognize
	int CPlateRecognize::plateRecognize(const Mat& src, std::vector<CPlate> &plateVecOut, int img_index) {
		Mat img;
		float scale = 1.f;
		if (m_uniformSizes) {
			// resize to uniform sizes
			img = uniformResize(src, scale);
		}
		else
			src.copyTo(img);
		
		// 1. plate detect
		std::vector<CPlate> plateVec;
		int resultPD = plateDetect(img, plateVec, img_index);
		if (resultPD == 0) {
			size_t num = plateVec.size();
			for (size_t j = 0; j < num; j++) {
				CPlate& item = plateVec.at(j);
				Mat plateMat = item.getPlateMat();
				
				if (kDebug)
					Utils::imwrite(kTempImgOutputPath, "origin_plate.jpg", plateMat);

				// scale the rect to src;
				item.setPlateScale(scale);
				RotatedRect rect = item.getPlatePos();
				item.setPlatePos(scaleBackRRect(rect, 1.f / scale));

				// get plate color
				Color color = item.getPlateColor();
				if (color == UNKNOWN) {
					color = getPlateType(plateMat, true);
					item.setPlateColor(color);
				}
				std::string plateColor = getPlateColor(color);

				// 2. chars recognize
				std::string plateIdentify = "";
				int resultCR = charsRecognise(item, plateIdentify);
				if (resultCR == 0) {
					std::string license = plateColor + ":" + plateIdentify;
					item.setPlateStr(license);
					plateVecOut.push_back(item);
				}
				else {
					std::string license = plateColor;
					item.setPlateStr(license);
					plateVecOut.push_back(item);
				}
			}

		}
		return resultPD;
	}

	void CPlateRecognize::LoadSVM(std::string path) {
		PlateJudge::instance()->LoadModel(path);
	}

	void CPlateRecognize::LoadANN(std::string path) {
		CharsIdentify::instance()->LoadModel(path);
	}

	void CPlateRecognize::LoadChineseANN(std::string path) {
		CharsIdentify::instance()->LoadChineseModel(path);
	}

	void CPlateRecognize::LoadGrayChANN(std::string path) {
		CharsIdentify::instance()->LoadGrayChANN(path);
	}

	void CPlateRecognize::LoadChineseMapping(std::string path) {
		CharsIdentify::instance()->LoadChineseMapping(path);
	}

	int CPlateRecognize::plateRecognize(const Mat& src, std::vector<std::string> &licenseVec) {
		vector<CPlate> plates;
		int resultPR = plateRecognize(src, plates, 0);

		for (auto plate : plates) {
			licenseVec.push_back(plate.getPlateStr());
		}
		return resultPR;
	}

}