#pragma once
#include "siripr/core/plate_recognize.h"

#include "apisiripr.h"

#include "opencv2/opencv.hpp"

#include <string>

using namespace cv;

using namespace siripr;

using namespace std;

namespace ImgRec {

	string ImgRecognize(Mat src, bool isDebug = false) {

		CPlateRecognize pr;

		// set the parameters of CPlateRecognize
		pr.setDebug(isDebug);
		pr.setLifemode(true);
		pr.setDetectType(PR_DETECT_COLOR);
		pr.setMaxPlates(1);
		//pr.setUniformSizes(true);

		// load the maching learning model
		pr.LoadSVM("resources/model/svm_hist.xml");
		pr.LoadANN("resources/model/ann.xml");
		pr.LoadChineseANN("resources/model/ann_chinese.xml");

		// new in v1.6
		pr.LoadGrayChANN("resources/model/annCh.xml");
		pr.LoadChineseMapping("resources/model/province_mapping");

		clock_t begin, end;

		if (isDebug) {
			begin = clock();
		}
		
		vector<CPlate> plateVec;
		string plateLicense = "Not found plate";
		if (pr.plateRecognize(src, plateVec) == 0) {
			if (plateVec.size())
				for (int i = 0; i < plateVec.size(); i++) {
					CPlate plate = plateVec.at(i);
					plateLicense = plate.getPlateStr();

					if (isDebug) {
						Mat plateMat = plate.getPlateMat();
						RotatedRect rrect = plate.getPlatePos();
						rectangle(src, rrect.boundingRect(), CV_RGB(0, 255, 0));
						Utils::imwrite(kTempImgOutputPath, "src_rrect.jpg", src);
						Mat imageROI = src(Rect(0, plateMat.rows*i, plateMat.cols, plateMat.rows));

						plateMat.copyTo(imageROI);
					}
				}
		}

		if (isDebug) {
			end = clock();
			cout << "time:" << (end - begin) / 1000.0 << "s" << endl;
		}

		return plateLicense;
	}

	void test() {
		siripr::api::apiInitPR();
		string test_path = "resources/image/test3";

		auto files = Utils::getFiles(test_path);

		int size = files.size();
		if (0 == size) {
			cout << "No File Found" << endl;
			return;
		}

		for (int i = 0; i < files.size(); i++) {

			string filepath = files[i].c_str();
			Mat img = imread(filepath);

			if (!img.data) {
				cout << filepath + ":Í¼Æ¬´ò¿ªÊ§°Ü!" << endl;
				continue;
			}

			//namedWindow(filepath);
			//imshow(filepath, img);

			//cout << ImgRecognize(img, NULL, 1) << endl;
			int x = 0;
			int y = 0;
			int w = 0;
			int h = 0;
			
			//cout << siripr::api::apiImgRecognize(img, x, y, w, h,true) << " " << x << " " << y << " " << w << " " << h << endl;
			cout << ImgRecognize(img, true) << endl;
			cout << "----------------------" << endl;
			
			waitKey(0);

			destroyAllWindows();
		}

		return;
	}

}
