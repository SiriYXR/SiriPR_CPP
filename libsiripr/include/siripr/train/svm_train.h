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
		��ʼ��ѵ������

		������
			plates_folder��ѵ�����ݴ�ŵ���·��
			xml��ѵ�����ɵ�ģ�ʹ�ŵ�·��

		����ֵ����
		*/
		SvmTrain(const std::string plates_folder, const std::string xml);

		virtual void train();

		virtual void test();

		/*
		����ѵ��������

		������
			param��ѵ������

		����ֵ����
		*/
		void setRounds(int param);

	private:
		/*
		��ѵ�����ݴ�ŵ���·���·ֱ��ȡѵ������������·������������ٴ�����Ӧ��ǩ��

		��������

		����ֵ����
		*/
		void prepare();

		/*
		��ÿһ�����ݽ���������ȡ��Ȼ�����SVM֧�ֵ�ѵ�����ݸ�ʽ��

		��������

		����ֵ��
			ѵ������
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
