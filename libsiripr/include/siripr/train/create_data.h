#ifndef SIRIPR_CREATE_DATA_H_
#define SIRIPR_CREATE_DATA_H_

#include "opencv2/opencv.hpp"
#include "siripr/config.h"

using namespace cv;
using namespace std;

/*! \namespace siripr
Namespace where all the C++ SiriPR functionality resides
*/
namespace siripr {

	// shift an image
	Mat translateImg(Mat img, int offsetx, int offsety, int bk = 0);
	// rotate an image
	Mat rotateImg(Mat source, float angle, int bk = 0);

	// crop the image
	Mat cropImg(Mat src, int x, int y, int shift, int bk = 0);

	Mat generateSyntheticImage(const Mat& image, int use_swap = 1);

} /*! \namespace siripr*/

#endif  // SIRIPR_CREATE_DATA_H_