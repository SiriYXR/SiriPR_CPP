#pragma once

#include <string>

#include <qstring.h>
#include <qfile.h>
#include <qtextcodec.h>
#include <qimage.h>
#include <qdebug>
#include "opencv2/opencv.hpp"

namespace TOOLS {

	//快速读取QSS文件
	static QString ReadQSS(QString path) {
		QFile f(path);
		QString qss("");
		f.open(QFile::ReadOnly);
		qss = f.readAll();
		f.close();
		return qss;
	}

	//将QString转换为std::string
	static std::string QStringToStdString(QString str) {
		QTextCodec* code = QTextCodec::codecForName("gb18030");
		std::string stdstr = code->fromUnicode(str).data();
		return stdstr;
	}

	//将std::string转换为QString
	static QString StdStringToQString(std::string str) {
		QString qstr = QTextCodec::codecForLocale()->toUnicode((str).c_str());

		return qstr;
	}

	//QImage转cvMat
	/*static cv::Mat QImage2cvMat(QImage image)
	{
		cv::Mat mat;
		switch (image.format())
		{
		case QImage::Format_ARGB32:
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32_Premultiplied:
			mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
			break;
		case QImage::Format_RGB888:
			mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
			cv::cvtColor(mat, mat, CV_BGR2RGB);
			break;
		case QImage::Format_Indexed8:
			mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
			break;
		}
		return mat;
	}*/

	//QImage转cvMat
	static cv::Mat QImage2cvMat(const QImage& image)
	{
		switch (image.format())
		{
			// 8-bit, 4 channel
		case QImage::Format_ARGB32:
			break;
		case QImage::Format_ARGB32_Premultiplied:
		{
			cv::Mat mat(image.height(), image.width(),
				CV_8UC4,
				(void*)image.constBits(),
				image.bytesPerLine());
			return mat.clone();
		}

		// 8-bit, 3 channel
		case QImage::Format_RGB32:
		{
			cv::Mat mat(image.height(), image.width(),
				CV_8UC4,
				(void*)image.constBits(),
				image.bytesPerLine());

			// drop the all-white alpha channel
			cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
			return mat.clone();
		}
		case QImage::Format_RGB888:
		{
			QImage swapped = image.rgbSwapped();
			cv::Mat mat(swapped.height(), swapped.width(),
				CV_8UC3,
				(void*)image.constBits(),
				image.bytesPerLine());
			return mat.clone();
		}

		// 8-bit, 1 channel   
		case QImage::Format_Indexed8:
		{
			cv::Mat mat(image.height(), image.width(),
				CV_8UC1,
				(void*)image.constBits(),
				image.bytesPerLine());
			return mat.clone();
		}

		default:
			qDebug() << "ERROR: QImage could not be converted to Mat.";
			break;
		}
		return cv::Mat();
	}

	//cvMat转QImage
	static QImage cvMat2QImage(const cv::Mat& mat)
	{
		// 8-bits unsigned, NO. OF CHANNELS = 1
		if (mat.type() == CV_8UC1)
		{
			QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
			// Set the color table (used to translate colour indexes to qRgb values)
			image.setColorCount(256);
			for (int i = 0; i < 256; i++)
			{
				image.setColor(i, qRgb(i, i, i));
			}
			// Copy input Mat
			uchar* pSrc = mat.data;
			for (int row = 0; row < mat.rows; row++)
			{
				uchar* pDest = image.scanLine(row);
				memcpy(pDest, pSrc, mat.cols);
				pSrc += mat.step;
			}
			return image;
		}
		// 8-bits unsigned, NO. OF CHANNELS = 3
		else if (mat.type() == CV_8UC3)
		{
			// Copy input Mat
			const uchar* pSrc = (const uchar*)mat.data;
			// Create QImage with same dimensions as input Mat
			QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
			return image.rgbSwapped();
		}
		else if (mat.type() == CV_8UC4)
		{
			// Copy input Mat
			const uchar* pSrc = (const uchar*)mat.data;
			// Create QImage with same dimensions as input Mat
			QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
			return image.copy();
		}
		else
		{
			return QImage();
		}
	}

	static QString TickTimeProcess(int t) {
		if (t < 60 * 1000) {
			return QString(u8"%1秒").arg(QString::number(t/1000.0,'f',2));
		}
		else if (t < 60 * 60 * 1000) {
			int i = t / 1000;
			int m = i / 60;
			double s = t / 1000.0 - m * 60;
			return QString(u8"%1分 %2秒")
				.arg(QString::number(m))
				.arg(QString::number(s, 'f', 2));
		}
		else {
			int i = t / 1000;
			int h = i / (60*60);
			int m = (i - h * 60 * 60) / 60;
			double s = t / 1000.0 - m * 60 - h * 60 * 60;
			return QString(u8"%1小时 %2分 %3秒")
				.arg(QString::number(h))
				.arg(QString::number(m))
				.arg(QString::number(s, 'f', 2));
		}
	}
	
}

