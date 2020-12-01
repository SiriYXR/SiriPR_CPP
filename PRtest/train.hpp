#pragma once

#include "siripr\train\svm_train.h"
#include "siripr\train\ann_train.h"

using namespace cv;

using namespace siripr;

using namespace std;

namespace Train {

	void TrainSVM() {
		SvmTrain svm("resources/train/svm", "resources/train/svm.xml");
		svm.train();
		svm.test();
	}

	void TrainANN() {
		AnnTrain ann("resources/train/ann", "resources/train/ann.xml");
		ann.train();
		ann.test();
	}

}
