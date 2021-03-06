#include "siripr\api\api.h"


namespace siripr {
	namespace api {

		CPlateRecognize pr;

		void initPR(bool lifemod,int maxplates) {
			// set the parameters of CPlateRecognize
			pr.setDebug(false);
			pr.setLifemode(lifemod);
			pr.setDetectType(PR_DETECT_COLOR);
			pr.setMaxPlates(maxplates);

			// load the maching learning model
			pr.LoadSVM("resources/model/svm_hist.xml");
			pr.LoadANN("resources/model/ann.xml");
			pr.LoadChineseANN("resources/model/ann_chinese.xml");

			// new in v1.6
			pr.LoadGrayChANN("resources/model/annCh.xml");
			pr.LoadChineseMapping("resources/model/province_mapping");
		}


		string ImgRecognize(Mat src, int& x, int& y, int& w, int& h, bool isDebug) {

			pr.setDebug(isDebug);

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
						RotatedRect rrect = plate.getPlatePos();
						x = rrect.boundingRect().x;
						y = rrect.boundingRect().y;
						w = rrect.boundingRect().width;
						h = rrect.boundingRect().height;


						if (isDebug) {
							Mat plateMat = plate.getPlateMat();
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
	}
}