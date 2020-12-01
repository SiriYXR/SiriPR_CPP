#ifndef SIRIPR_CORE_PLATERECOGNIZE_H_
#define SIRIPR_CORE_PLATERECOGNIZE_H_

#include "siripr/core/plate_detect.h"
#include "siripr/core/chars_recognise.h"

/*! \namespace siripr
Namespace where all the C++ SiriPR functionality resides
*/
namespace siripr {

	class CPlateRecognize : public CPlateDetect, public CCharsRecognise {
	public:
		CPlateRecognize();
		~CPlateRecognize();

		int plateRecognize(const Mat& src, std::vector<CPlate> &plateVec, int img_index = 0);
		int plateRecognize(const Mat& src, std::vector<std::string> &licenseVec);

		inline void setLifemode(bool param) { CPlateDetect::setPDLifemode(param); }
		inline void setDetectType(int param) { CPlateDetect::setDetectType(param); }

		inline void setResultShow(bool param) { m_showResult = param; }
		inline bool getResultShow() const { return m_showResult; }
		inline void setDetectShow(bool param) { CPlateDetect::setDetectShow(param); }

		inline void setUniformSizes(bool param) { m_uniformSizes = param; }
		inline bool getUniformSizes() const { return m_uniformSizes; }
		
		inline void setDebug(bool param) {
			m_debug = param;
			CPlateDetect::setDebug(param);
			CCharsRecognise::setDebug(param);
		}
		inline bool getDebug() const { return m_debug; }


		void LoadSVM(std::string path);
		void LoadANN(std::string path);
		void LoadChineseANN(std::string path);

		//v1.6 added
		void LoadGrayChANN(std::string path);
		void LoadChineseMapping(std::string path);

	private:
		// show the detect and recognition result image
		bool m_showResult;

		bool m_uniformSizes;

		bool m_debug;

		DISABLE_ASSIGN_AND_COPY(CPlateRecognize);
	};

} /* \namespace siripr  */

#endif  // SIRIPR_CORE_PLATERECOGNIZE_H_