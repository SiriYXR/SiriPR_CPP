#ifndef SIRIPR_TRAIN_TRAIN_H_
#define SIRIPR_TRAIN_TRAIN_H_

#include <opencv2/opencv.hpp>

namespace siripr {

	class ITrain {
	public:
		ITrain();

		virtual ~ITrain();

		virtual void train() = 0;

		virtual void test() = 0;

	private:
		virtual cv::Ptr<cv::ml::TrainData> tdata() = 0;
	};
}

#endif  // SIRIPR_TRAIN_TRAIN_H_