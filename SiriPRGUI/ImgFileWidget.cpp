#include "ImgFileWidget.h"

#include <qsettings.h>
#include <qlayout.h>

#include "ImgPRWidget.h"
#include "tools.hpp"

ImgFileWidget::ImgFileWidget(ImgPRWidget* parent, QString path)
{
	this->m_fWindow = parent;

	this->loadData(path);
	this->initUI();
}

void ImgFileWidget::initUI()
{
	this->setFixedHeight(25);
	this->setStyleSheet(TOOLS::ReadQSS(this->MAIN_STYLE));

	this->m_checkbox = new QCheckBox();
	this->m_checkbox->setFixedSize(15, 25);
	this->m_checkbox->setTristate(false);// 取消半选中状态
	connect(this->m_checkbox, &QCheckBox::clicked, this->m_fWindow, &ImgPRWidget::updateCheckedList);

	this->m_fileNameBtn = new QPushButton(this->m_file_name);
	this->m_fileNameBtn->setFixedHeight(25);
	connect(this->m_fileNameBtn, &QPushButton::clicked, this, &ImgFileWidget::showImg);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(this->m_checkbox);
	layout->addWidget(this->m_fileNameBtn);

	this->setLayout(layout);
}

void ImgFileWidget::loadData(QString path)
{
	this->m_file_path = path;
	this->m_file_name = path.split("/").back();
	this->m_plates.clear();

	QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);

	this->m_debug_value = (config.value("IMGPR/debug").toString() == "True");
	this->m_label_value = (config.value("IMGPR/label").toString() == "True");
	this->m_detecttype_value = config.value("IMGPR/detecttype_value").toInt();
	this->m_maxplates_value = config.value("IMGPR/maxplates_value").toInt();
}

void ImgFileWidget::showImg()
{
	this->m_fWindow->showImageFile(this->m_file_path);
}

QString ImgFileWidget::toStr()
{
	QStringList detectTypeList = { "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" };

	QString strOut = u8"";
	strOut += QString(u8"file_path：%1\nfile_name：%2\ndetecttype：%3\nmaxplates：%4\n")
		.arg(this->m_file_path)
		.arg(this->m_file_name)
		.arg(detectTypeList[this->m_detecttype_value])
		.arg(this->m_maxplates_value);

	for (auto j : this->m_plates) {
		cv::RotatedRect rrect = j.getPlatePos();
		cv::Rect rect = rrect.boundingRect();
		strOut += QString(u8"plate_license：%1\tx：%2\ty：%3\tw：%4\th：%5\n")
			.arg(TOOLS::StdStringToQString(std::string(j.getPlateStr())))
			.arg(rect.x)
			.arg(rect.y)
			.arg(rect.width)
			.arg(rect.height);
	}
	return strOut;
}

QString ImgFileWidget::getFilePath()
{
	return this->m_file_path;
}

QString ImgFileWidget::getFileName()
{
	return this->m_file_name;
}

vector<siripr::CPlate>& ImgFileWidget::getPlates()
{
	return this->m_plates;
}

void ImgFileWidget::setPlates(vector<siripr::CPlate> plates)
{
	this->m_plates = plates;
}

bool ImgFileWidget::getDebug()
{
	return this->m_debug_value;
}

void ImgFileWidget::setDebug(bool debug)
{
	this->m_debug_value = debug;
}

bool ImgFileWidget::getLabel()
{
	return this->m_label_value;
}

void ImgFileWidget::setLabel(bool label)
{
	this->m_label_value = label;
}

int ImgFileWidget::getDetecttype()
{
	return this->m_detecttype_value;
}

void ImgFileWidget::setDetecttype(int detecttype)
{
	this->m_detecttype_value = detecttype;
}

int ImgFileWidget::getMaxPlates()
{
	return this->m_maxplates_value;
}

void ImgFileWidget::setMaxPlates(int maxplates)
{
	this->m_maxplates_value = maxplates;
}

bool ImgFileWidget::isChecked()
{
	return this->m_checkbox->isChecked();
}

void ImgFileWidget::setCheckeState(Qt::CheckState state)
{
	this->m_checkbox->setCheckState(state);
}
