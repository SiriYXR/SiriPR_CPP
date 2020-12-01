#include "SettingWidget.h"

#include <qgridlayout.h>
#include <qlayout.h>
#include <qfiledialog.h>

#include "tools.hpp"
#include "MainWindow.h"

SettingWidget::SettingWidget(MainWindow* parent)
{
	this->m_fWindow = parent;

	this->m_config = new QSettings("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);

	this->initUI();
	this->resetData();
}

void SettingWidget::initUI()
{
	//------------------模型路径设置------------------
	this->m_label_ModelPath = new QLabel(u8"模型路径设置");
	this->m_label_ModelPath->setFixedHeight(20);
	this->m_label_ModelPath->setAlignment(Qt::AlignCenter);

	this->m_label_SVM = new QLabel(u8"SVM模型路径:");
	this->m_label_SVM->setFixedSize(200, 20);
	this->m_label_SVM->setAlignment(Qt::AlignLeft);
	this->m_lineedit_SVM = new QLineEdit();
	this->m_lineedit_SVM->setFixedHeight(30);
	this->m_btn_SVM = new QPushButton("...");
	this->m_btn_SVM->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_SVM->setFixedSize(30, 30);
	connect(this->m_btn_SVM, &QPushButton::clicked, this, &SettingWidget::on_btn_SVM_clicked);

	QGridLayout* layout_SVM = new QGridLayout();
	layout_SVM->setContentsMargins(0, 5, 0, 0);
	layout_SVM->setSpacing(5);
	layout_SVM->addWidget(this->m_label_SVM, 0, 0);
	layout_SVM->addWidget(this->m_lineedit_SVM, 1, 0);
	layout_SVM->addWidget(this->m_btn_SVM, 1, 1);

	this->m_label_ANN = new QLabel(u8"ANN模型路径:");
	this->m_label_ANN->setFixedSize(200, 20);
	this->m_label_ANN->setAlignment(Qt::AlignLeft);
	this->m_lineedit_ANN = new QLineEdit();
	this->m_lineedit_ANN->setFixedHeight(30);
	this->m_btn_ANN = new QPushButton("...");
	this->m_btn_ANN->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_ANN->setFixedSize(30, 30);
	connect(this->m_btn_ANN, &QPushButton::clicked, this, &SettingWidget::on_btn_ANN_clicked);

	QGridLayout* layout_ANN = new QGridLayout();
	layout_ANN->setContentsMargins(0, 5, 0, 0);
	layout_ANN->setSpacing(5);
	layout_ANN->addWidget(this->m_label_ANN, 0, 0);
	layout_ANN->addWidget(this->m_lineedit_ANN, 1, 0);
	layout_ANN->addWidget(this->m_btn_ANN, 1, 1);

	this->m_label_ChineseANN = new QLabel(u8"ChineseANN模型路径:");
	this->m_label_ChineseANN->setFixedSize(200, 20);
	this->m_label_ChineseANN->setAlignment(Qt::AlignLeft);
	this->m_lineedit_ChineseANN = new QLineEdit();
	this->m_lineedit_ChineseANN->setFixedHeight(30);
	this->m_btn_ChineseANN = new QPushButton("...");
	this->m_btn_ChineseANN->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_ChineseANN->setFixedSize(30, 30);
	connect(this->m_btn_ChineseANN, &QPushButton::clicked, this, &SettingWidget::on_btn_ChineseANN_clicked);

	QGridLayout* layout_ChineseANN = new QGridLayout();
	layout_ChineseANN->setContentsMargins(0, 5, 0, 0);
	layout_ChineseANN->setSpacing(5);
	layout_ChineseANN->addWidget(this->m_label_ChineseANN, 0, 0);
	layout_ChineseANN->addWidget(this->m_lineedit_ChineseANN, 1, 0);
	layout_ChineseANN->addWidget(this->m_btn_ChineseANN, 1, 1);

	this->m_label_GrayChANN = new QLabel(u8"GrayANN模型路径:");
	this->m_label_GrayChANN->setFixedSize(200, 20);
	this->m_label_GrayChANN->setAlignment(Qt::AlignLeft);
	this->m_lineedit_GrayChANN = new QLineEdit();
	this->m_lineedit_GrayChANN->setFixedHeight(30);
	this->m_btn_GrayChANN = new QPushButton("...");
	this->m_btn_GrayChANN->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_GrayChANN->setFixedSize(30, 30);
	connect(this->m_btn_GrayChANN, &QPushButton::clicked, this, &SettingWidget::on_btn_GrayChANN_clicked);

	QGridLayout* layout_GrayChANN = new QGridLayout();
	layout_GrayChANN->setContentsMargins(0, 5, 0, 0);
	layout_GrayChANN->setSpacing(5);
	layout_GrayChANN->addWidget(this->m_label_GrayChANN, 0, 0);
	layout_GrayChANN->addWidget(this->m_lineedit_GrayChANN, 1, 0);
	layout_GrayChANN->addWidget(this->m_btn_GrayChANN, 1, 1);

	this->m_label_ChineseMapping = new QLabel(u8"ChineseMapping:");
	this->m_label_ChineseMapping->setFixedSize(200, 20);
	this->m_label_ChineseMapping->setAlignment(Qt::AlignLeft);
	this->m_lineedit_ChineseMapping = new QLineEdit();
	this->m_lineedit_ChineseMapping->setFixedHeight(30);
	this->m_btn_ChineseMapping = new QPushButton("...");
	this->m_btn_ChineseMapping->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_ChineseMapping->setFixedSize(30, 30);
	connect(this->m_btn_ChineseMapping, &QPushButton::clicked, this, &SettingWidget::on_btn_ChineseMapping_clicked);

	QGridLayout* layout_ChineseMapping = new QGridLayout();
	layout_ChineseMapping->setContentsMargins(0, 5, 0, 0);
	layout_ChineseMapping->setSpacing(5);
	layout_ChineseMapping->addWidget(this->m_label_ChineseMapping, 0, 0);
	layout_ChineseMapping->addWidget(this->m_lineedit_ChineseMapping, 1, 0);
	layout_ChineseMapping->addWidget(this->m_btn_ChineseMapping, 1, 1);

	QVBoxLayout* layout_ModelPath = new QVBoxLayout();
	layout_ModelPath->setContentsMargins(5, 10, 5, 0);
	layout_ModelPath->setSpacing(10);
	layout_ModelPath->addWidget(this->m_label_ModelPath);
	layout_ModelPath->addLayout(layout_SVM);
	layout_ModelPath->addLayout(layout_ANN);
	layout_ModelPath->addLayout(layout_ChineseANN);
	layout_ModelPath->addLayout(layout_GrayChANN);
	layout_ModelPath->addLayout(layout_ChineseMapping);

	//------------------图片识别设置------------------
	this->m_label_ImgPR = new QLabel(u8"图片识别设置");
	this->m_label_ImgPR->setFixedHeight(20);
	this->m_label_ImgPR->setAlignment(Qt::AlignCenter);

	this->m_label_ImgPR_debug = new QLabel(u8"调试:");
	this->m_label_ImgPR_debug->setFixedSize(40, 30);
	this->m_label_ImgPR_debug->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_ImgPR_debug = new QCheckBox();
	this->m_checkbox_ImgPR_debug->setFixedSize(100, 30);

	QHBoxLayout* layout_ImgPR_debug = new QHBoxLayout();
	layout_ImgPR_debug->setContentsMargins(0, 5, 0, 0);
	layout_ImgPR_debug->setSpacing(5);
	layout_ImgPR_debug->addWidget(this->m_label_ImgPR_debug);
	layout_ImgPR_debug->addWidget(this->m_checkbox_ImgPR_debug);
	layout_ImgPR_debug->addStretch();

	this->m_label_ImgPR_label = new QLabel(u8"标注结果:");
	this->m_label_ImgPR_label->setFixedSize(65, 30);
	this->m_label_ImgPR_label->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_ImgPR_label = new QCheckBox();
	this->m_checkbox_ImgPR_label->setFixedSize(100, 30);

	QHBoxLayout* layout_ImgPR_label = new QHBoxLayout();
	layout_ImgPR_label->setContentsMargins(0, 5, 0, 0);
	layout_ImgPR_label->setSpacing(5);
	layout_ImgPR_label->addWidget(this->m_label_ImgPR_label);
	layout_ImgPR_label->addWidget(this->m_checkbox_ImgPR_label);
	layout_ImgPR_label->addStretch();

	this->m_label_ImgPR_detecttype = new QLabel(u8"检测类型:");
	this->m_label_ImgPR_detecttype->setFixedSize(65, 30);
	this->m_label_ImgPR_detecttype->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_combobox_ImgPR_detecttype = new QComboBox();
	this->m_combobox_ImgPR_detecttype->setFixedSize(120, 20);
	this->m_combobox_ImgPR_detecttype->addItems(
		{ "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" });

	QHBoxLayout* layout_ImgPR_detecttype = new QHBoxLayout();
	layout_ImgPR_detecttype->setContentsMargins(0, 5, 0, 0);
	layout_ImgPR_detecttype->setSpacing(5);
	layout_ImgPR_detecttype->addWidget(this->m_label_ImgPR_detecttype);
	layout_ImgPR_detecttype->addWidget(this->m_combobox_ImgPR_detecttype);
	layout_ImgPR_detecttype->addStretch();

	this->m_label_ImgPR_maxplates = new QLabel(u8"最大车牌上限:");
	this->m_label_ImgPR_maxplates->setFixedSize(90, 30);
	this->m_label_ImgPR_maxplates->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_spinbox_ImgPR_maxplates = new QSpinBox();
	this->m_spinbox_ImgPR_maxplates->setFixedSize(50, 22);
	this->m_spinbox_ImgPR_maxplates->setAlignment(Qt::AlignCenter);
	this->m_spinbox_ImgPR_maxplates->setMinimum(1);
	this->m_spinbox_ImgPR_maxplates->setMaximum(10);

	QHBoxLayout* layout_ImgPR_maxplates = new QHBoxLayout();
	layout_ImgPR_maxplates->setContentsMargins(0, 5, 0, 0);
	layout_ImgPR_maxplates->setSpacing(5);
	layout_ImgPR_maxplates->addWidget(this->m_label_ImgPR_maxplates);
	layout_ImgPR_maxplates->addWidget(this->m_spinbox_ImgPR_maxplates);
	layout_ImgPR_maxplates->addStretch();

	this->m_label_ImgPR_OutPutPath = new QLabel(u8"识别结果保存路径:");
	this->m_label_ImgPR_OutPutPath->setFixedSize(200, 20);
	this->m_label_ImgPR_OutPutPath->setAlignment(Qt::AlignLeft);
	this->m_lineedit_ImgPR_OutPutPath = new  QLineEdit();
	this->m_lineedit_ImgPR_OutPutPath->setFixedHeight(30);
	this->m_btn_ImgPR_OutPutPath = new  QPushButton("...");
	this->m_btn_ImgPR_OutPutPath->setStyleSheet(BTN_STYLE);
	this->m_btn_ImgPR_OutPutPath->setFixedSize(30, 30);
	connect(this->m_btn_ImgPR_OutPutPath, &QPushButton::clicked, this, &SettingWidget::on_btn_ImgPR_OutPutPath_clicked);

	QGridLayout* layout_ImgPR_OutPutPath = new QGridLayout();
	layout_ImgPR_OutPutPath->setContentsMargins(0, 5, 0, 0);
	layout_ImgPR_OutPutPath->setSpacing(5);
	layout_ImgPR_OutPutPath->addWidget(this->m_label_ImgPR_OutPutPath, 0, 0);
	layout_ImgPR_OutPutPath->addWidget(this->m_lineedit_ImgPR_OutPutPath, 1, 0);
	layout_ImgPR_OutPutPath->addWidget(this->m_btn_ImgPR_OutPutPath, 1, 1);

	QVBoxLayout* layout_ImgPR = new QVBoxLayout();
	layout_ImgPR->setContentsMargins(5, 10, 5, 0);
	layout_ImgPR->setSpacing(10);
	layout_ImgPR->addWidget(this->m_label_ImgPR);
	layout_ImgPR->addLayout(layout_ImgPR_debug);
	layout_ImgPR->addLayout(layout_ImgPR_label);
	layout_ImgPR->addLayout(layout_ImgPR_detecttype);
	layout_ImgPR->addLayout(layout_ImgPR_maxplates);
	layout_ImgPR->addLayout(layout_ImgPR_OutPutPath);

	//------------------视频识别设置------------------
	this->m_label_VideoPR = new QLabel(u8"视频识别设置");
	this->m_label_VideoPR->setFixedHeight(20);
	this->m_label_VideoPR->setAlignment(Qt::AlignCenter);

	this->m_label_VideoPR_recognize = new QLabel(u8"识别:");
	this->m_label_VideoPR_recognize->setFixedSize(40, 30);
	this->m_label_VideoPR_recognize->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_VideoPR_recognize = new QCheckBox();
	this->m_checkbox_VideoPR_recognize->setFixedSize(100, 30);

	QHBoxLayout* layout_VideoPR_recognize = new QHBoxLayout();
	layout_VideoPR_recognize->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_recognize->setSpacing(5);
	layout_VideoPR_recognize->addWidget(this->m_label_VideoPR_recognize);
	layout_VideoPR_recognize->addWidget(this->m_checkbox_VideoPR_recognize);
	layout_VideoPR_recognize->addStretch();

	this->m_label_VideoPR_record = new QLabel(u8"录制:");
	this->m_label_VideoPR_record->setFixedSize(40, 30);
	this->m_label_VideoPR_record->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_VideoPR_record = new QCheckBox();
	this->m_checkbox_VideoPR_record->setFixedSize(100, 30);

	QHBoxLayout* layout_VideoPR_record = new QHBoxLayout();
	layout_VideoPR_record->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_record->setSpacing(5);
	layout_VideoPR_record->addWidget(this->m_label_VideoPR_record);
	layout_VideoPR_record->addWidget(this->m_checkbox_VideoPR_record);
	layout_VideoPR_record->addStretch();

	this->m_label_VideoPR_debug = new QLabel(u8"调试:");
	this->m_label_VideoPR_debug->setFixedSize(40, 30);
	this->m_label_VideoPR_debug->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_VideoPR_debug = new QCheckBox();
	this->m_checkbox_VideoPR_debug->setFixedSize(100, 30);

	QHBoxLayout* layout_VideoPR_debug = new QHBoxLayout();
	layout_VideoPR_debug->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_debug->setSpacing(5);
	layout_VideoPR_debug->addWidget(this->m_label_VideoPR_debug);
	layout_VideoPR_debug->addWidget(this->m_checkbox_VideoPR_debug);
	layout_VideoPR_debug->addStretch();

	this->m_label_VideoPR_label = new QLabel(u8"标注结果:");
	this->m_label_VideoPR_label->setFixedSize(65, 30);
	this->m_label_VideoPR_label->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_checkbox_VideoPR_label = new QCheckBox();
	this->m_checkbox_VideoPR_label->setFixedSize(100, 30);

	QHBoxLayout* layout_VideoPR_label = new QHBoxLayout();
	layout_VideoPR_label->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_label->setSpacing(5);
	layout_VideoPR_label->addWidget(this->m_label_VideoPR_label);
	layout_VideoPR_label->addWidget(this->m_checkbox_VideoPR_label);
	layout_VideoPR_label->addStretch();

	this->m_label_VideoPR_detecttype = new QLabel(u8"检测类型:");
	this->m_label_VideoPR_detecttype->setFixedSize(65, 30);
	this->m_label_VideoPR_detecttype->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_combobox_VideoPR_detecttype = new QComboBox();
	this->m_combobox_VideoPR_detecttype->setFixedSize(120, 20);
	this->m_combobox_VideoPR_detecttype->addItems(
			{ "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" });

	QHBoxLayout* layout_VideoPR_detecttype = new QHBoxLayout();
	layout_VideoPR_detecttype->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_detecttype->setSpacing(5);
	layout_VideoPR_detecttype->addWidget(this->m_label_VideoPR_detecttype);
	layout_VideoPR_detecttype->addWidget(this->m_combobox_VideoPR_detecttype);
	layout_VideoPR_detecttype->addStretch();

	this->m_label_VideoPR_maxplates = new QLabel(u8"最大车牌上限:");
	this->m_label_VideoPR_maxplates->setFixedSize(90, 30);
	this->m_label_VideoPR_maxplates->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	this->m_spinbox_VideoPR_maxplates = new QSpinBox();
	this->m_spinbox_VideoPR_maxplates->setFixedSize(50, 22);
	this->m_spinbox_VideoPR_maxplates->setAlignment(Qt::AlignCenter);
	this->m_spinbox_VideoPR_maxplates->setMinimum(1);
	this->m_spinbox_VideoPR_maxplates->setMaximum(10);

	QHBoxLayout* layout_VideoPR_maxplates = new QHBoxLayout();
	layout_VideoPR_maxplates->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_maxplates->setSpacing(5);
	layout_VideoPR_maxplates->addWidget(this->m_label_VideoPR_maxplates);
	layout_VideoPR_maxplates->addWidget(this->m_spinbox_VideoPR_maxplates);
	layout_VideoPR_maxplates->addStretch();

	this->m_label_VideoPR_OutPutPath = new QLabel(u8"识别结果保存路径:");
	this->m_label_VideoPR_OutPutPath->setFixedSize(200, 20);
	this->m_label_VideoPR_OutPutPath->setAlignment(Qt::AlignLeft);
	this->m_lineedit_VideoPR_OutPutPath = new QLineEdit();
	this->m_lineedit_VideoPR_OutPutPath->setFixedHeight(30);
	this->m_btn_VideoPR_OutPutPath = new QPushButton("...");
	this->m_btn_VideoPR_OutPutPath->setStyleSheet(BTN_STYLE);
	this->m_btn_VideoPR_OutPutPath->setFixedSize(30, 30);
	connect(this->m_btn_VideoPR_OutPutPath, &QPushButton::clicked, this, &SettingWidget::on_btn_VideoPR_OutPutPath_clicked);

	QGridLayout* layout_VideoPR_OutPutPath = new QGridLayout();
	layout_VideoPR_OutPutPath->setContentsMargins(0, 5, 0, 0);
	layout_VideoPR_OutPutPath->setSpacing(5);
	layout_VideoPR_OutPutPath->addWidget(this->m_label_VideoPR_OutPutPath, 0, 0);
	layout_VideoPR_OutPutPath->addWidget(this->m_lineedit_VideoPR_OutPutPath, 1, 0);
	layout_VideoPR_OutPutPath->addWidget(this->m_btn_VideoPR_OutPutPath, 1, 1);

	QVBoxLayout* layout_VideoPR = new QVBoxLayout();
	layout_VideoPR->setContentsMargins(5, 10, 5, 0);
	layout_VideoPR->setSpacing(10);
	layout_VideoPR->addWidget(this->m_label_VideoPR);
	layout_VideoPR->addLayout(layout_VideoPR_recognize);
	layout_VideoPR->addLayout(layout_VideoPR_record);
	layout_VideoPR->addLayout(layout_VideoPR_debug);
	layout_VideoPR->addLayout(layout_VideoPR_label);
	layout_VideoPR->addLayout(layout_VideoPR_detecttype);
	layout_VideoPR->addLayout(layout_VideoPR_maxplates);
	layout_VideoPR->addLayout(layout_VideoPR_OutPutPath);

	//------------------QScrollArea------------------
	QVBoxLayout* sa_contentLayout = new QVBoxLayout();
	sa_contentLayout->setContentsMargins(0, 0, 0, 0);
	sa_contentLayout->setSpacing(10);
	sa_contentLayout->addLayout(layout_ModelPath);
	sa_contentLayout->addLayout(layout_ImgPR);
	sa_contentLayout->addLayout(layout_VideoPR);
	sa_contentLayout->addStretch();

	this->m_sa_contentWidget = new QWidget();
	this->m_sa_contentWidget->setFixedSize(600, 1100);
	this->m_sa_contentWidget->setLayout(sa_contentLayout);

	this->m_sa_Settings = new QScrollArea();
	this->m_sa_Settings->setStyleSheet(TOOLS::ReadQSS(this->SCROLLAREA_STYLE));
	this->m_sa_Settings->setWidget(this->m_sa_contentWidget);
	this->m_sa_Settings->setAlignment(Qt::AlignHCenter);

	//------------------------------------------------------
	this->m_btn_defualt = new QPushButton(u8"默认");
	this->m_btn_defualt->setFixedSize(60, 30);
	this->m_btn_defualt->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_defualt->setStatusTip(u8"恢复默认设置");
	connect(this->m_btn_defualt, &QPushButton::clicked, this, &SettingWidget::on_btn_defualt_clicked);

	this->m_btn_cancel =new QPushButton(u8"取消");
	this->m_btn_cancel->setFixedSize(60, 30);
	this->m_btn_cancel->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_cancel->setStatusTip(u8"取消未保存的修改");
	connect(this->m_btn_cancel, &QPushButton::clicked, this, &SettingWidget::on_btn_cancel_clicked);

	this->m_btn_apply =new QPushButton(u8"应用");
	this->m_btn_apply->setFixedSize(60, 30);
	this->m_btn_apply->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_apply->setStatusTip(u8"保存并应用修改");
	connect(this->m_btn_apply, &QPushButton::clicked, this, &SettingWidget::on_btn_apply_clicked);

	QHBoxLayout* layout_bottom = new QHBoxLayout();
	layout_bottom->setContentsMargins(0, 0, 20, 0);
	layout_bottom->setSpacing(5);
	layout_bottom->addStretch(1);
	layout_bottom->addWidget(this->m_btn_defualt);
	layout_bottom->addWidget(this->m_btn_cancel);
	layout_bottom->addWidget(this->m_btn_apply);

	//------------------------------------------------------
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->setSpacing(3);
	mainLayout->addWidget(this->m_sa_Settings);
	mainLayout->addLayout(layout_bottom);

	this->setLayout(mainLayout);
}

void SettingWidget::resetData()
{
	this->m_lineedit_SVM->setText(this->m_config->value("Model/SVM").toString());
	this->m_lineedit_ANN->setText(this->m_config->value("Model/ANN").toString());
	this->m_lineedit_ChineseANN->setText(this->m_config->value("Model/ChineseANN").toString());
	this->m_lineedit_GrayChANN->setText(this->m_config->value("Model/GrayChANN").toString());
	this->m_lineedit_ChineseMapping->setText(this->m_config->value("Model/ChineseMapping").toString());

	if (this->m_config->value("IMGPR/debug").toString() == "True") {
		this->m_checkbox_ImgPR_debug->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_ImgPR_debug->setCheckState(Qt::Unchecked);
	}
	if (this->m_config->value("IMGPR/label").toString() == "True") {
		this->m_checkbox_ImgPR_label->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_ImgPR_label->setCheckState(Qt::Unchecked);
	}
	this->m_combobox_ImgPR_detecttype->setCurrentIndex(this->m_config->value("IMGPR/detecttype").toInt());
	this->m_spinbox_ImgPR_maxplates->setValue(this->m_config->value("IMGPR/maxplates").toInt());
	this->m_lineedit_ImgPR_OutPutPath->setText(this->m_config->value("IMGPR/outputpath").toString());

	if (this->m_config->value("VIDEOPR/recognize").toString() == "True") {
		this->m_checkbox_VideoPR_recognize->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_VideoPR_recognize->setCheckState(Qt::Unchecked);
	}
	if (this->m_config->value("VIDEOPR/record").toString() == "True") {
		this->m_checkbox_VideoPR_record->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_VideoPR_record->setCheckState(Qt::Unchecked);
	}
	if (this->m_config->value("VIDEOPR/debug").toString() == "True") {
		this->m_checkbox_VideoPR_debug->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_VideoPR_debug->setCheckState(Qt::Unchecked);
	}
	if (this->m_config->value("VIDEOPR/label").toString() == "True") {
		this->m_checkbox_VideoPR_label->setCheckState(Qt::Checked);
	}
	else {
		this->m_checkbox_VideoPR_label->setCheckState(Qt::Unchecked);
	}
	this->m_combobox_VideoPR_detecttype->setCurrentIndex(this->m_config->value("VIDEOPR/detecttype").toInt());
	this->m_spinbox_VideoPR_maxplates->setValue(this->m_config->value("VIDEOPR/maxplates").toInt());
	this->m_lineedit_VideoPR_OutPutPath->setText(this->m_config->value("VIDEOPR/outputpath").toString());
}

void SettingWidget::defualtData()
{
	this->m_config = new QSettings("./SiriPRGUI/config/defualt.ini", QSettings::IniFormat);
	this->resetData();
	this->m_config = new QSettings("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);
	this->saveConfig();
}

void SettingWidget::on_btn_SVM_clicked()
{
	QString path = QFileDialog().getOpenFileName(this, u8"设置SVM模型路径", "../resources/model", "Model File(*.xml)");

	if (path.length() > 0) {
		this->m_lineedit_SVM->setText(path);
	}
}

void SettingWidget::on_btn_ANN_clicked()
{
	QString path = QFileDialog().getOpenFileName(this, u8"设置ANN模型路径", "../resources/model", "Model File(*.xml)");

	if (path.length() > 0) {
		this->m_lineedit_ANN->setText(path);
	}
}

void SettingWidget::on_btn_ChineseANN_clicked()
{
	QString path = QFileDialog().getOpenFileName(this, u8"设置ChineseANN模型路径", "../resources/model", "Model File(*.xml)");

	if (path.length() > 0) {
		this->m_lineedit_ChineseANN->setText(path);
	}
}

void SettingWidget::on_btn_GrayChANN_clicked()
{
	QString path = QFileDialog().getOpenFileName(this, u8"设置GrayChANN模型路径", "../resources/model", "Model File(*.xml)");

	if (path.length() > 0) {
		this->m_lineedit_GrayChANN->setText(path);
	}
}

void SettingWidget::on_btn_ChineseMapping_clicked()
{
	QString path = QFileDialog().getOpenFileName(this, u8"设置ChineseMapping路径", "../resources/model", "Model File(*.xml)");

	if (path.length() > 0) {
		this->m_lineedit_ChineseMapping->setText(path);
	}
}

void SettingWidget::on_btn_ImgPR_OutPutPath_clicked()
{
	QString path = QFileDialog().getExistingDirectory(this, u8"设置图片识别结果保存路径", "../resources/image/result");

	if (path.length() > 0) {
		this->m_lineedit_ImgPR_OutPutPath->setText(path);
	}
}

void SettingWidget::on_btn_VideoPR_OutPutPath_clicked()
{
	QString path = QFileDialog().getExistingDirectory(this, u8"设置图片识别结果保存路径", "../resources/video/result");

	if (path.length() > 0) {
		this->m_lineedit_VideoPR_OutPutPath->setText(path);
	}
}

void SettingWidget::on_btn_defualt_clicked()
{
	this->defualtData();
}

void SettingWidget::on_btn_cancel_clicked()
{
	this->resetData();
}

void SettingWidget::on_btn_apply_clicked()
{
	this->saveConfig();
}

void SettingWidget::show() 
{
	this->resetData();
	this->QWidget::show();
}

void SettingWidget::saveConfig()
{
	this->m_config->setValue("Model/SVM", this->m_lineedit_SVM->text());
	this->m_config->setValue("Model/ANN", this->m_lineedit_ANN->text());
	this->m_config->setValue("Model/ChineseANN", this->m_lineedit_ChineseANN->text());
	this->m_config->setValue("Model/GrayChANN", this->m_lineedit_GrayChANN->text());
	this->m_config->setValue("Model/ChineseMapping", this->m_lineedit_ChineseMapping->text());

	if (this->m_checkbox_ImgPR_debug->isChecked()) {
		this->m_config->setValue("IMGPR/debug", "True");
	}
	else {
		this->m_config->setValue("IMGPR/debug", "False");
	}
	if (this->m_checkbox_ImgPR_label->isChecked()) {
		this->m_config->setValue("IMGPR/label", "True");
	}
	else {
		this->m_config->setValue("IMGPR/label", "False");
	}
	this->m_config->setValue("IMGPR/detecttype", this->m_combobox_ImgPR_detecttype->currentIndex());
	this->m_config->setValue("IMGPR/maxplates", this->m_spinbox_ImgPR_maxplates->value());
	this->m_config->setValue("IMGPR/outputpath", this->m_lineedit_ImgPR_OutPutPath->text());

	if (this->m_checkbox_VideoPR_recognize->isChecked()) {
		this->m_config->setValue("VideoPR/recognize", "True");
	}
	else {
		this->m_config->setValue("VideoPR/recognize", "False");
	}
	if (this->m_checkbox_VideoPR_record->isChecked()) {
		this->m_config->setValue("VideoPR/record", "True");
	}
	else {
		this->m_config->setValue("VideoPR/record", "False");
	}
	if (this->m_checkbox_VideoPR_debug->isChecked()) {
		this->m_config->setValue("VideoPR/debug", "True");
	}
	else {
		this->m_config->setValue("VideoPR/debug", "False");
	}
	if (this->m_checkbox_VideoPR_label->isChecked()) {
		this->m_config->setValue("VideoPR/label", "True");
	}
	else {
		this->m_config->setValue("VideoPR/label", "False");
	}
	this->m_config->setValue("VideoPR/detecttype", this->m_combobox_VideoPR_detecttype->currentIndex());
	this->m_config->setValue("VideoPR/maxplates", this->m_spinbox_VideoPR_maxplates->value());
	this->m_config->setValue("VideoPR/outputpath", this->m_lineedit_VideoPR_OutPutPath->text());

	this->m_fWindow->m_plateRecognize->LoadModel();
}

