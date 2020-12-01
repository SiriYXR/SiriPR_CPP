#ifndef SIRIPR_CORE_PLATEJUDGE_H_
#define SIRIPR_CORE_PLATEJUDGE_H_

#include "siripr/core/plate.hpp"
#include "siripr/core/feature.h"

namespace siripr {

	class PlateJudge {
	public:
		static PlateJudge* instance();
		void LoadModel(std::string path);

		int plateJudgeUsingNMS(const std::vector<CPlate>&, std::vector<CPlate>&, int maxPlates = 5);
		int plateSetScore(CPlate& plate);

		int plateJudge(const Mat& plateMat);
		int plateJudge(const std::vector<Mat> &inVec,
			std::vector<Mat> &resultVec);
		int plateJudge(const std::vector<CPlate> &inVec,
			std::vector<CPlate> &resultVec);

	private:
		// singleton
		PlateJudge();

		static PlateJudge* instance_;

		svmCallback extractFeature;

		cv::Ptr<ml::SVM> svm_;

	};
}

#endif  // SIRIPR_CORE_PLATEJUDGE_H_