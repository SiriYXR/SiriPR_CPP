#ifndef SIRIPR_TRAIN_SVMTRAIN_H_
#define SIRIPR_TRAIN_SVMTRAIN_H_

#include "siripr/train/train.h"
#include <vector>
#include "siripr/config.h"
#include "siripr/core/feature.h"

namespace siripr {

	//int svmTrain(bool dividePrepared, bool trainPrepared);

	class SvmTrain : public ITrain {
	public:
		typedef struct {
			std::string file;
			SvmLabel label;
		} TrainItem;

		/*
		初始化训练对象。

		参数：
			plates_folder：训练数据存放的总路径
			xml：训练生成的模型存放的路径

		返回值：无
		*/
		SvmTrain(const std::string plates_folder, const std::string xml);

		virtual void train();

		virtual void test();

		/*
		设置训练轮数。

		参数：
			param：训练轮数

		返回值：无
		*/
		void setRounds(int param);

	private:
		/*
		从训练数据存放的总路径下分别读取训练、测试数据路径，并随打乱再打上相应标签。

		参数：无

		返回值：无
		*/
		void prepare();

		/*
		对每一个数据进行特征提取，然后构造成SVM支持的训练数据格式。

		参数：无

		返回值：
			训练数据
		*/
		virtual cv::Ptr<cv::ml::TrainData> tdata();

		cv::Ptr<cv::ml::SVM> svm_;
		const std::string plates_folder_;
		const std::string svm_xml_;
		std::vector<TrainItem> train_file_list_;
		std::vector<TrainItem> test_file_list_;

		int m_rounds;

		svmCallback extractFeature;
		bool isPrepared = true;
	};
}

#endif  // SIRIPR_TRAIN_SVMTRAIN_H_
