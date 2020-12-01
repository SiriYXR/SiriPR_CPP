#pragma once

#include "siripr/core/plate_recognize.h"

#include "opencv2/opencv.hpp"

#include <time.h>

using namespace cv;
using namespace siripr;

int Webcam() {

	string WINDOW_NAME = "PRtest";
	CPlateRecognize pr;

	int res = 0;
	int count = 0;

	namedWindow(WINDOW_NAME);

	VideoCapture capture("http://webvideo4.suianbo.cn:13880/hls_sab/api-0020AE000020-1.m3u8?dev_token=8ec9737aa1537c449c94d0160964b090");
	//VideoCapture capture("http://webvideo4.suianbo.cn:13880/hls_sab/api-0020AE000013-1.m3u8?dev_token=ba7649b53918701d916364bdfd6c3455");

	system("cls");
	cout << "\n" << endl;
	cout << "Camera" << endl;
	cout << "---------------" << endl;
	cout << "C: 录制开始/暂停" << endl;
	cout << "SPACE: 播放/暂停" << endl;
	cout << "ESC: 退出" << endl;
	cout << "---------------\n" << endl;
	cout << "录制在退出时停止" << endl;
	cout << "\n---------------\n" << endl;

	if (capture.isOpened())
		cout << "摄像头成功打开!\n" << endl;
	else {
		destroyWindow(WINDOW_NAME);
		cout << "摄像头打开失败!" << endl;
		return 0;
	}

	int h = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	int w = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int f = capture.get(CV_CAP_PROP_FPS);
	cout << w << " " << h << " " << f << endl;

	//VideoWriter writer("resources/video/monitor/test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(w, h), true);

	Mat frame, edges;
	bool isEnd, isPause, isWrite, isRec;
	isEnd = false;
	isPause = false;
	isWrite = true;
	isRec = true;

	// set the parameters of CPlateRecognize
	pr.setLifemode(true);
	pr.setDetectType(PR_DETECT_COLOR);
	pr.setMaxPlates(4);

	// load the maching learning model
	pr.LoadSVM("resources/model/svm_hist.xml");
	pr.LoadANN("resources/model/ann.xml");
	pr.LoadChineseANN("resources/model/ann_chinese.xml");

	// new in v1.6
	pr.LoadGrayChANN("resources/model/annCh.xml");
	pr.LoadChineseMapping("resources/model/province_mapping");

	char c = 0;
	while (true) {

		if (!isPause)
			capture >> frame;

		if (frame.data) {
			count = 0;
			if (!isPause) {

				if (isRec) {
					clock_t begin, end;
					begin = clock();

					vector<CPlate> plateVec;
					if (pr.plateRecognize(frame, plateVec) == 0) {
						if (plateVec.size()) {
							if (isWrite) {
								//writer << frame;
								char str[255];
								time_t timep;
								struct tm *p;
								time(&timep);
								p = localtime(&timep);
								sprintf(str, "resources/video/monitor/jpg/%d%02d%02d%02d%02d%02d.jpg", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
								imwrite(str, frame);
							}
						}
						for (int i = 0; i < plateVec.size(); i++) {
							CPlate plate = plateVec.at(i);
							Mat plateMat = plate.getPlateMat();
							RotatedRect rrect = plate.getPlatePos();
							string license = plate.getPlateStr();

							rectangle(frame, rrect.boundingRect(), CV_RGB(0, 255, 0));

							Mat imageROI = frame(Rect(0, plateMat.rows*i, plateMat.cols, plateMat.rows));
							plateMat.copyTo(imageROI);

							cout << license << endl;

						}


					}
					end = clock();
					cout << "time:" << (end - begin) / 1000.0 << "s" << endl;
					cout << "---------------" << endl;
				}
			}
			imshow(WINDOW_NAME, frame);
		}
		else {
			count++;
		}

		if (count > 10) {
			res = -1;
			break;
		}

		c = waitKey(1000 / 30);

		switch (c) {
		case '1':
			pr.setDetectType(PR_DETECT_COLOR);
			break;
		case '2':
			pr.setDetectType(PR_DETECT_SOBEL);
			break;
		case '3':
			pr.setDetectType(PR_DETECT_CMSER);
			break;
		case 'R':
		case 'r':
			isRec = !isRec;
			if (isRec)
				cout << "开始识别" << endl;
			else
				cout << "暂停识别" << endl;
			break;
		case 'C':
		case 'c':
			isWrite = !isWrite;
			if (isWrite)
				cout << "录制开始" << endl;
			else
				cout << "录制暂停" << endl;
			break;
		case 32:
			isPause = !isPause;
			if (isPause)
				cout << "开始" << endl;
			else
				cout << "暂停" << endl;
			break;
			break;
		case 27:
			isEnd = true;
			break;
		default:
			break;
		}

		if (isEnd)
			break;
	}

	destroyWindow(WINDOW_NAME);

	return res;
}