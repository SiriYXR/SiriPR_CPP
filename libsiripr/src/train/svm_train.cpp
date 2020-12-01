#include "siripr/train/svm_train.h"
#include "siripr/util/util.h"
#include "siripr/config.h"

#ifdef OS_WINDOWS
#include <ctime>
#endif

#include <iostream>

using namespace cv;
using namespace cv::ml;

namespace siripr {

	SvmTrain::SvmTrain(const std::string plates_folder, const std::string xml)
		: plates_folder_(plates_folder), svm_xml_(xml) {
		assert(plates_folder.c_str());
		assert(xml.c_str());
		
		//默认训练轮数
		m_rounds = 20000;

		//设置特征提取回调函数
		extractFeature = getHistomPlusColoFeatures;
	}

	void SvmTrain::train() {
		
		svm_ = cv::ml::SVM::create();
		svm_->setType(cv::ml::SVM::C_SVC);
		svm_->setKernel(cv::ml::SVM::RBF);
		svm_->setDegree(0.1);
		// 1.4 bug fix: old 1.4 ver gamma is 1
		svm_->setGamma(0.1);
		svm_->setCoef0(0.1);
		svm_->setC(1);
		svm_->setNu(0.1);
		svm_->setP(0.1);
		svm_->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, m_rounds, 0.0001));
		
		//获取训练数据
		this->prepare();
		
		if (train_file_list_.size() == 0) {
			std::cout<< "No file found in the train folder!"<<std::endl;
			std::cout << "You should create a folder named \"train\" in SiriPR resources folder." << std::endl;
			std::cout << "Copy train data folder(like \"SVM\") under \"resources/train\". " << std::endl;
			return;
		}
		auto train_data = tdata();

		std::cout << ">> Training SVM model, please wait..." << std::endl;
		long start = utils::getTimestamp();
		//开始训练
		svm_->trainAuto(train_data, 10, SVM::getDefaultGrid(SVM::C),
			SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
			SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
			SVM::getDefaultGrid(SVM::DEGREE), true);
		//svm_->train(train_data);

		long end = utils::getTimestamp();
		std::cout << ">> Training done. Time elapse:"<< end - start <<"ms"<<std::endl;
		std::cout << ">> Saving model file..." << std::endl;
		//保存训练模型
		svm_->save(svm_xml_);

		std::cout << ">> Your SVM Model was saved to "<< svm_xml_ << std::endl;
		std::cout << ">> Testing..." << std::endl;

		this->test();

	}

	void SvmTrain::test() {
		// 1.4 bug fix: old 1.4 ver there is no null judge
		// if (NULL == svm_)
		LOAD_SVM_MODEL(svm_, svm_xml_);

		if (test_file_list_.empty()) {
			this->prepare();
		}

		double count_all = test_file_list_.size();
		double ptrue_rtrue = 0;
		double ptrue_rfalse = 0;
		double pfalse_rtrue = 0;
		double pfalse_rfalse = 0;

		for (auto item : test_file_list_) {
			auto image = cv::imread(item.file);
			if (!image.data) {
				std::cout << "no" << std::endl;
				continue;
			}
			cv::Mat feature;
			extractFeature(image, feature);

			auto predict = int(svm_->predict(feature));
			//std::cout << "predict: " << predict << std::endl;

			auto real = item.label;
			if (predict == kForward && real == kForward) ptrue_rtrue++;
			if (predict == kForward && real == kInverse) ptrue_rfalse++;
			if (predict == kInverse && real == kForward) pfalse_rtrue++;
			if (predict == kInverse && real == kInverse) pfalse_rfalse++;
		}

		std::cout << "count_all: " << count_all << std::endl;
		std::cout << "ptrue_rtrue: " << ptrue_rtrue << std::endl;
		std::cout << "ptrue_rfalse: " << ptrue_rfalse << std::endl;
		std::cout << "pfalse_rtrue: " << pfalse_rtrue << std::endl;
		std::cout << "pfalse_rfalse: " << pfalse_rfalse << std::endl;

		double precise = 0;
		if (ptrue_rtrue + ptrue_rfalse != 0) {
			precise = ptrue_rtrue / (ptrue_rtrue + ptrue_rfalse);
			std::cout << "precise: " << precise << std::endl;
		}
		else {
			std::cout << "precise: "
				<< "NA" << std::endl;
		}

		double recall = 0;
		if (ptrue_rtrue + pfalse_rtrue != 0) {
			recall = ptrue_rtrue / (ptrue_rtrue + pfalse_rtrue);
			std::cout << "recall: " << recall << std::endl;
		}
		else {
			std::cout << "recall: "
				<< "NA" << std::endl;
		}

		double Fsocre = 0;
		if (precise + recall != 0) {
			Fsocre = 2 * (precise * recall) / (precise + recall);
			std::cout << "Fsocre: " << Fsocre << std::endl;
		}
		else {
			std::cout << "Fsocre: "
				<< "NA" << std::endl;
		}
	}

	void SvmTrain::setRounds(int param)
	{
		m_rounds = param>0?param:0;
	}

	void SvmTrain::prepare() {
		srand(unsigned(time(NULL)));
		
		char buffer[260] = { 0 };
		
		//获取所有训练、测试数据的路径，并随机打乱
		sprintf(buffer, "%s/has/train", plates_folder_.c_str()); 
		auto has_file_train_list = utils::getFiles(buffer);
		std::random_shuffle(has_file_train_list.begin(), has_file_train_list.end());

		sprintf(buffer, "%s/has/test", plates_folder_.c_str());
		auto has_file_test_list = utils::getFiles(buffer);
		std::random_shuffle(has_file_test_list.begin(), has_file_test_list.end());

		sprintf(buffer, "%s/no/train", plates_folder_.c_str());
		auto no_file_train_list = utils::getFiles(buffer);
		std::random_shuffle(no_file_train_list.begin(), no_file_train_list.end());

		sprintf(buffer, "%s/no/test", plates_folder_.c_str());
		auto no_file_test_list = utils::getFiles(buffer);
		std::random_shuffle(no_file_test_list.begin(), no_file_test_list.end());

		std::cout << ">> Collecting train data..." << std::endl;

		//对数据进行打标
		for (auto file : has_file_train_list)
			train_file_list_.push_back({ file, kForward });

		for (auto file : no_file_train_list)
			train_file_list_.push_back({ file, kInverse });

		std::cout << ">> Collecting test data..." << std::endl;
		
		for (auto file : has_file_test_list)
			test_file_list_.push_back({ file, kForward });
		
		for (auto file : no_file_test_list)
			test_file_list_.push_back({ file, kInverse });
		
	}

	cv::Ptr<cv::ml::TrainData> SvmTrain::tdata() {
		cv::Mat samples;
		std::vector<int> responses;

		for (auto f : train_file_list_) {
			auto image = cv::imread(f.file);
			if (!image.data) {
				std::cout << ">> Invalid image: "<<f.file.c_str() <<"  ignore."<< std::endl;
				continue;
			}
			cv::Mat feature;
			extractFeature(image, feature);
			feature = feature.reshape(1, 1);

			samples.push_back(feature);
			responses.push_back(int(f.label));
		}

		cv::Mat samples_, responses_;
		samples.convertTo(samples_, CV_32FC1);
		cv::Mat(responses).copyTo(responses_);

		return cv::ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE, responses_);
	}

}  // namespace siripr
