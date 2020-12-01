#pragma once

#include <vector>
#include <qstring.h>
#include <qsettings.h>
#include <qlist.h>
#include <qimage.h>

#include "siripr/core/plate_recognize.h"
#include "opencv2/opencv.hpp"


class CPRecognize
{
public:

	CPRecognize();

	void LoadModel();

	vector<siripr::CPlate> PlateRecognize(cv::Mat img);

	void SetDetectType(int type);

	void SetMaxPlates(int param);

	void SetDebug(int debug);

private:

	siripr::CPlateRecognize* m_pr;
};

