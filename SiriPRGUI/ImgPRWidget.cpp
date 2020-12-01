#include "ImgPRWidget.h"

#include <time.h>

#include <qsettings.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qinputdialog.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qpen.h>
#include <qfont.h>
#include <qfontmetrics.h>

#include "MainWindow.h"
#include "ImgFileWidget.h"
#include "ImgOutPortWidget.h"
#include "tools.hpp"

#include <qdebug.h>

ImgPRWidget::ImgPRWidget(MainWindow* parent)
{
	this->m_fWindow = parent;
	
	this->m_filelist = new vector<ImgFileWidget*>();
	this->m_fileset = new set<QString>();
	this->m_checkedlist = new vector<int>();
	this->m_fileindex = -1;
	this->m_plateindex = -1;
	
	this->m_isCheckedAll = true;

	this->initUI();
}

void ImgPRWidget::initUI()
{
	this->setStyleSheet(TOOLS::ReadQSS(this->MAIN_STYLE));

	this->m_selectAllImgCheckBox = new QCheckBox(u8"全选 (0/0)");
	this->m_selectAllImgCheckBox->setTristate(false);// 取消半选中状态
	this->m_selectAllImgCheckBox->setEnabled(false);
	connect(this->m_selectAllImgCheckBox, &QCheckBox::clicked, this, &ImgPRWidget::selectAllImgCheckBoxClicked);

	this->m_inportBtn = new QPushButton("+");
	this->m_inportBtn->setFixedSize(25, 25);
	this->m_inportBtn->setStatusTip(u8"导入图片文件");
	connect(this->m_inportBtn, &QPushButton::clicked, this, &ImgPRWidget::on_btn_inport_clicked);

	this->m_removeBtn = new QPushButton("-");
	this->m_removeBtn->setFixedSize(25, 25);
	this->m_removeBtn->setStatusTip(u8"移除图片文件");
	connect(this->m_removeBtn, &QPushButton::clicked, this, &ImgPRWidget::on_btn_remove_clicked);

	this->m_outportBtn = new QPushButton(u8"导出");
	this->m_outportBtn->setFixedSize(40, 25);
	this->m_outportBtn->setStatusTip(u8"导出图片识别结果");
	connect(this->m_outportBtn, &QPushButton::clicked, this, &ImgPRWidget::on_btn_outport_clicked);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->setContentsMargins(0, 0, 0, 0);
	topLayout->setSpacing(0);
	topLayout->addWidget(this->m_selectAllImgCheckBox);
	topLayout->addWidget(this->m_inportBtn);
	topLayout->addWidget(this->m_removeBtn);
	topLayout->addWidget(this->m_outportBtn);

	this->m_imgListContentLayout = new QVBoxLayout();
	this->m_imgListContentLayout->setContentsMargins(0, 0, 0, 0);
	this->m_imgListContentLayout->setSpacing(2);

	this->m_imgListScrollContent = new QWidget();
	this->m_imgListScrollContent->setFixedWidth(190);
	this->m_imgListScrollContent->setStyleSheet("background-color:#f1f1f1");
	this->m_imgListScrollContent->setContentsMargins(0, 0, 0, 0);
	this->m_imgListScrollContent->setLayout(this->m_imgListContentLayout);

	this->m_imgListScrollArea = new QScrollArea();
	this->m_imgListScrollArea->setContentsMargins(0, 0, 0, 0);
	this->m_imgListScrollArea->setStyleSheet(TOOLS::ReadQSS(this->SCROLLAREA_STYLE));
	this->m_imgListScrollArea->setFixedWidth(200);
	this->m_imgListScrollArea->setWidget(this->m_imgListScrollContent);

	QHBoxLayout* bottomLayout = new QHBoxLayout();
	bottomLayout->setContentsMargins(0, 0, 0, 0);
	bottomLayout->setSpacing(0);
	bottomLayout->addWidget(this->m_imgListScrollArea);

	//--------------------sourceLayout------------------------
	QVBoxLayout* sourceLayout = new QVBoxLayout();
	sourceLayout->setContentsMargins(5, 0, 0, 0);
	sourceLayout->setSpacing(0);
	sourceLayout->addLayout(topLayout);
	sourceLayout->addLayout(bottomLayout);

	//-------------------- topLayout------------------------
	this->m_label_RunningMSG = new QLabel();
	this->m_label_RunningMSG->setAlignment(Qt::AlignCenter);
	this->m_label_RunningMSG->setFixedHeight(25);

	QHBoxLayout* topLayout2 = new QHBoxLayout();
	topLayout2->addWidget(this->m_label_RunningMSG);

	this->m_imgLabel = new QLabel(u8"导入图片 ");
	this->m_imgLabel->setAlignment(Qt::AlignCenter);

	this->m_sa_img = new QScrollArea();
	this->m_sa_img->setStyleSheet(TOOLS::ReadQSS(this->SCROLLAREA_STYLE));
	this->m_sa_img->setWidget(this->m_imgLabel);
	this->m_sa_img->setAlignment(Qt::AlignCenter);
	//--------------------currentFilePathLayout------------------------
	
	this->m_currentFilePathLabel = new QLabel();
	this->m_currentFilePathLabel->setAlignment(Qt::AlignCenter);
	this->m_currentFilePathLabel->setFixedHeight(30);

	this->m_btn_last_img = new QPushButton("<");
	this->m_btn_last_img->setFixedSize(30, 30);
	this->m_btn_last_img->setStatusTip(u8"加载上一张图片");
	connect(this->m_btn_last_img, &QPushButton::clicked, this, &ImgPRWidget::on_btn_last_img_clicked);

	this->m_btn_next_img = new QPushButton(">");
	this->m_btn_next_img->setFixedSize(30, 30);
	this->m_btn_next_img->setStatusTip(u8"加载下一张图片");
	connect(this->m_btn_next_img, &QPushButton::clicked, this, &ImgPRWidget::on_btn_next_img_clicked);

	QHBoxLayout* currentFilePathLayout = new QHBoxLayout();
	currentFilePathLayout->setContentsMargins(0, 2, 0, 0);
	currentFilePathLayout->setSpacing(5);
	currentFilePathLayout->addWidget(this->m_btn_last_img);
	currentFilePathLayout->addWidget(this->m_currentFilePathLabel);
	currentFilePathLayout->addWidget(this->m_btn_next_img);

	//--------------------prOPlayout------------------------
	this->m_btn_recognize = new QPushButton(u8"识别");
	this->m_btn_recognize->setFixedSize(100, 30);
	this->m_btn_recognize->setStatusTip(u8"识别当前图片");
	connect(this->m_btn_recognize, &QPushButton::clicked, this, &ImgPRWidget::on_btn_recognize_clicked);

	this->m_combobox_Plate = new MyComboBox();
	this->m_combobox_Plate->setFixedHeight(30);
	connect(this->m_combobox_Plate, QOverload<int>::of(&MyComboBox::currentIndexChanged), this, &ImgPRWidget::on_combobox_Plate_clicked);
	connect(this->m_combobox_Plate, &MyComboBox::popupAboutToBeShown, this, &ImgPRWidget::on_combobox_Plate_clicked);

	this->m_btn_last_plate = new QPushButton("<");
	this->m_btn_last_plate->setFixedSize(30, 30);
	this->m_btn_last_plate->setStatusTip(u8"上一张车牌");
	connect(this->m_btn_last_plate, &QPushButton::clicked, this, &ImgPRWidget::on_btn_last_plate_clicked);

	this->m_btn_next_plate = new QPushButton(">");
	this->m_btn_next_plate->setFixedSize(30, 30);
	this->m_btn_next_plate->setStatusTip(u8"下一张车牌");
	connect(this->m_btn_next_plate, &QPushButton::clicked, this, &ImgPRWidget::on_btn_next_plate_clicked);

	QHBoxLayout* prOPlayout = new QHBoxLayout();
	prOPlayout->setContentsMargins(0, 2, 0, 0);
	prOPlayout->setSpacing(5);
	prOPlayout->addWidget(this->m_btn_recognize);
	prOPlayout->addWidget(this->m_btn_last_plate);
	prOPlayout->addWidget(this->m_combobox_Plate);
	prOPlayout->addWidget(this->m_btn_next_plate);

	//--------------------setinglayout------------------------
	this->m_cb_debug = new QCheckBox(u8"调试");
	this->m_cb_debug->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));
	connect(this->m_cb_debug, &QCheckBox::clicked, this, &ImgPRWidget::on_cb_debug_clicked);

	this->m_cb_label = new QCheckBox(u8"标注结果");
	this->m_cb_label->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));
	connect(this->m_cb_label, &QCheckBox::clicked, this, &ImgPRWidget::on_cb_label_clicked);

	this->m_label_DetectType = new QLabel(u8"检测类型");
	this->m_combobox_DetectType = new QComboBox();
	this->m_combobox_DetectType->setFixedSize(120, 20);
	this->m_combobox_DetectType->addItems(
		{ "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" });
	this->m_combobox_DetectType->setStatusTip(u8"设置检测类型");
	connect(this->m_combobox_DetectType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImgPRWidget::on_combobox_DetectType_currentIndexChanged);

	this->m_label_MaxPlates = new QLabel(u8"最大车牌上限");
	this->m_spinbox_MaxPlates = new QSpinBox();
	this->m_spinbox_MaxPlates->setFixedSize(50, 22);
	this->m_spinbox_MaxPlates->setAlignment(Qt::AlignCenter);
	this->m_spinbox_MaxPlates->setMinimum(1);
	this->m_spinbox_MaxPlates->setMaximum(10);
	this->m_spinbox_MaxPlates->setStatusTip(u8"设置单张图片检测车牌最大数量");
	connect(this->m_spinbox_MaxPlates, QOverload<int>::of(&QSpinBox::valueChanged), this, &ImgPRWidget::on_spinbox_MaxPlates_valueChanged);

	QHBoxLayout* settingLayout = new QHBoxLayout();
	settingLayout->setContentsMargins(5, 5, 5, 0);
	settingLayout->setSpacing(5);
	settingLayout->addWidget(this->m_cb_debug);
	settingLayout->addWidget(this->m_cb_label);
	settingLayout->addWidget(this->m_label_DetectType);
	settingLayout->addWidget(this->m_combobox_DetectType);
	settingLayout->addWidget(this->m_label_MaxPlates);
	settingLayout->addWidget(this->m_spinbox_MaxPlates);
	settingLayout->addStretch(1);

	//--------------------recognizeLayout------------------------
	QVBoxLayout* recognizeLayout = new QVBoxLayout();
	recognizeLayout->setContentsMargins(5, 0, 5, 0);
	recognizeLayout->setSpacing(0);
	recognizeLayout->addLayout(topLayout2);
	recognizeLayout->addWidget(this->m_sa_img);
	recognizeLayout->addLayout(currentFilePathLayout);
	recognizeLayout->addLayout(prOPlayout);
	recognizeLayout->addLayout(settingLayout);
	
	//--------------------mainLayout------------------------
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addLayout(sourceLayout);
	mainLayout->addLayout(recognizeLayout);

	this->setLayout(mainLayout);
}

void ImgPRWidget::initPRWidget()
{
	this->m_label_RunningMSG->clear();
	this->m_imgLabel->clear();
	this->m_imgLabel->setText(u8"导入图片");
	this->m_imgLabel->resize(200, 200);
	this->m_currentFilePathLabel->setText("");
	this->m_combobox_Plate->clear();
	this->m_combobox_DetectType->setCurrentIndex(0);
	this->m_spinbox_MaxPlates->setValue(1);
	this->m_cb_debug->setCheckState(Qt::Unchecked);
	this->m_cb_label->setCheckState(Qt::Unchecked);
	this->m_plateindex = -1;
}

void ImgPRWidget::show()
{
	if (this->m_fileindex == -1)
		this->initPRWidget();
	this->QWidget::show();
}

void ImgPRWidget::showImageFile(QString file_path)
{
	if (this->m_currentFilePathLabel->text() == file_path) {
		this->m_fWindow->statusBar()->showMessage(u8"文件已打开！");
		return;
	}
	this->m_label_RunningMSG->clear();
	this->openIMG(file_path);
	this->m_currentFilePathLabel->setText(file_path);
	this->updateFileIndex();
	this->m_combobox_DetectType->setCurrentIndex(this->m_filelist->at(this->m_fileindex)->getDetecttype());
	this->m_spinbox_MaxPlates->setValue(this->m_filelist->at(this->m_fileindex)->getMaxPlates());
	if (this->m_filelist->at(this->m_fileindex)->getDebug()) {
		this->m_cb_debug->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_debug->setCheckState(Qt::Unchecked);
	}
	if (this->m_filelist->at(this->m_fileindex)->getLabel()) {
		this->m_cb_label->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_label->setCheckState(Qt::Unchecked);
	}
	this->m_combobox_Plate->clear();

	vector<siripr::CPlate> plates = this->m_filelist->at(this->m_fileindex)->getPlates();
	for (int i = 0; i < plates.size(); i++) {
		
		cv::RotatedRect rrect = plates[i].getPlatePos();
		cv::Rect rect = rrect.boundingRect();
		this->m_combobox_Plate->insertItem(
			i,
			QString(u8"%1\tX: %2\tY: %3\tW: %4\tH: %5")
			.arg(TOOLS::StdStringToQString(plates[i].getPlateStr()))
			.arg(rect.x)
			.arg(rect.y)
			.arg(rect.width)
			.arg(rect.height)
		);
	}
	if (plates.size()>0) {
		this->m_plateindex = 0;
		this->m_combobox_Plate->setCurrentIndex(0);
	}
	this->setShowPlate();

	if (plates.size() > 0) {
		this->m_plateindex = 0;
	}
	else {
		this->m_plateindex = -1;
	}
}

vector<int>* ImgPRWidget::getCheckedList()
{
	return this->m_checkedlist;
}

vector<ImgFileWidget*>* ImgPRWidget::getFileList()
{
	return this->m_filelist;
}

void ImgPRWidget::keyPressEvent(QKeyEvent* event)
{
	int key = event->key();

	this->QWidget::keyPressEvent(event);
}

void ImgPRWidget::on_btn_last_img_clicked()
{
	if (this->m_fileindex == -1) {
		this->m_fWindow->statusBar()->showMessage(u8"请先导入图片!");
		return;
	}
	if (this->m_fileindex == 0) {
		this->m_fWindow->statusBar()->showMessage(u8"这是第一张图片!");
		return;
	}

	this->m_fileindex -= 1;
	this->showImageFile(this->m_filelist->at(this->m_fileindex)->getFilePath());
}

void ImgPRWidget::on_btn_next_img_clicked()
{
	if (this->m_fileindex == -1) {
		this->m_fWindow->statusBar()->showMessage(u8"请先导入图片!");
		return;
	}
	if (this->m_fileindex == this->m_filelist->size()-1) {
		this->m_fWindow->statusBar()->showMessage(u8"这是最后一张图片!");
		return;
	}

	this->m_fileindex += 1;
	this->showImageFile(this->m_filelist->at(this->m_fileindex)->getFilePath());
}

void ImgPRWidget::on_btn_inport_clicked()
{
	QStringList pathlist = QFileDialog::getOpenFileNames(this,
		u8"打开文件",
		"./resources/image",
		u8"图片文件(*.jpg *.jpeg *.png)"
	);

	int redundant = 0;
	for (auto i : pathlist) {
		if (this->m_fileset->find(i) != this->m_fileset->end()) {
			redundant++;
			continue;
		}
		this->m_fileset->insert(i);
		ImgFileWidget* w = new ImgFileWidget(this, i);
		this->m_filelist->push_back(w);
		this->m_imgListContentLayout->addWidget(w);
	}
	this->m_imgListScrollContent->setFixedSize(196, this->m_filelist->size() * 27);
	this->m_fWindow->statusBar()->showMessage(
		QString(u8"成功添加%1张图片，去除冗余图片%2张。")
		.arg(this->m_filelist->size() - redundant)
		.arg(redundant)
	);
	this->m_selectAllImgCheckBox->setEnabled(true);
	this->updateCheckedList();
}

void ImgPRWidget::on_btn_remove_clicked()
{
	vector<ImgFileWidget*>* tmplist=new vector<ImgFileWidget*>();
	QString currentImgPath="";
	if(this->m_fileindex>=0)
		currentImgPath = this->m_filelist->at(this->m_fileindex)->getFilePath();
	for (int i = 0; i < this->m_filelist->size(); i++) {
		int f = 1;
		for (int j = 0; j < this->m_checkedlist->size(); j++) {
			if (i == this->m_checkedlist->at(j)) {
				this->m_imgListContentLayout->removeWidget(this->m_filelist->at(i));
				this->m_fileset->erase(this->m_filelist->at(i)->getFilePath());
				f = 0;
				break;
			}
		}

		if (f) {
			tmplist->push_back(this->m_filelist->at(i));
		}
	}
	delete this->m_filelist;
	this->m_filelist = tmplist;

	int f2 = 1;
	for (int j = 0; j < this->m_checkedlist->size(); j++) {
		if (this->m_fileindex == this->m_checkedlist->at(j)) {
			if (this->m_filelist->empty()) {
				this->m_fileindex = -1;
				this->m_qimg=QImage();
			}
			else {
				this->m_fileindex = 0;
				this->showImageFile(this->m_filelist->front()->getFilePath());
			}
			f2 = 0;
			break;
		}
	}
	if (f2) {
		for (int i = 0; i < this->m_filelist->size(); i++) {
			if (this->m_filelist->at(i)->getFilePath() == currentImgPath) {
				this->m_fileindex = i;
				break;
			}
		}
	}

	this->m_imgListScrollContent->setFixedHeight(this->m_filelist->size() * 27);
	this->m_fWindow->statusBar()->showMessage(
		QString(u8"成功移除%1张图片。")
		.arg(this->m_checkedlist->size())
	);
	this->updateCheckedList();
}

void ImgPRWidget::on_btn_outport_clicked()
{
	if (this->m_checkedlist->empty()) {
		this->m_fWindow->statusBar()->showMessage(u8"当前没有被选中的图片。");
		return;
	}

	ImgOutPortWidget(this);
}

void ImgPRWidget::on_btn_recognize_clicked()
{
	if (this->m_fileindex>=0 && !this->m_qimg.isNull()) {
		this->m_label_RunningMSG->setText(u8"图片识别中...");

		this->m_fWindow->m_plateRecognize->SetDebug(this->m_cb_debug->isChecked());
		this->m_fWindow->m_plateRecognize->SetDetectType(this->m_combobox_DetectType->currentIndex());
		this->m_fWindow->m_plateRecognize->SetMaxPlates(this->m_spinbox_MaxPlates->value());

		clock_t begin, end;
		begin = clock();
		vector<siripr::CPlate> plates = this->m_fWindow->m_plateRecognize->PlateRecognize(this->m_cvimg);
		this->m_filelist->at(this->m_fileindex)->setPlates(plates);
		end = clock();
		double runtime = (end - begin) / 1000.0;

		int platenum = plates.size();
		if (platenum != 0) {
			this->m_plateindex = 0;
			this->m_combobox_Plate->clear();
			for (int i = 0; i < plates.size(); i++) {
				cv::RotatedRect rrect = plates[i].getPlatePos();
				cv::Rect rect = rrect.boundingRect();
				this->m_combobox_Plate->insertItem(
					i,
					QString(u8"%1\tX: %2\tY: %3\tW: %4\tH: %5")
					.arg(TOOLS::StdStringToQString(std::string(plates[i].getPlateStr())))
					.arg(rect.x)
					.arg(rect.y)
					.arg(rect.width)
					.arg(rect.height)
				);
			}
			this->m_combobox_Plate->setCurrentIndex(0);

			this->m_label_RunningMSG->setText(
				QString(u8"图片识别完成！     发现 %1 张车牌    运行时间:%2s")
				.arg(platenum)
				.arg(QString::number(runtime, 'f', 3))
			);

			this->setShowPlate();
		}
		else {
			this->m_fWindow->statusBar()->showMessage(u8"没有发现车牌！");
		}
	}
	else {
		this->m_fWindow->statusBar()->showMessage(u8"请先导入图片！");
	}
}

void ImgPRWidget::on_combobox_Plate_clicked()
{
	if (this->m_plateindex == -1) {
		return;
	}
	this->m_plateindex = this->m_combobox_Plate->currentIndex();
	this->setShowPlate();
}

void ImgPRWidget::on_btn_next_plate_clicked()
{
	if (this->m_plateindex == -1) {
		this->m_fWindow->statusBar()->showMessage(u8"没有车牌！");
		return;
	}

	if (this->m_plateindex == this->m_filelist->at(this->m_fileindex)->getPlates().size() - 1) {
		this->m_fWindow->statusBar()->showMessage(u8"这是最后一块车牌！");
		return;
	}

	this->m_plateindex += 1;
	this->m_combobox_Plate->setCurrentIndex(this->m_plateindex);
	this->on_combobox_Plate_clicked();
}

void ImgPRWidget::on_btn_last_plate_clicked()
{
	if (this->m_plateindex == -1) {
		this->m_fWindow->statusBar()->showMessage(u8"没有车牌！");
		return;
	}

	if (this->m_plateindex == 0) {
		this->m_fWindow->statusBar()->showMessage(u8"这是第一张车牌！");
		return;
	}

	this->m_plateindex -= 1;
	this->m_combobox_Plate->setCurrentIndex(this->m_plateindex);
	this->on_combobox_Plate_clicked();
}

void ImgPRWidget::on_cb_debug_clicked()
{
	if (this->m_fileindex >= 0) {
		if (this->m_cb_debug->isChecked()) {
			this->m_filelist->at(this->m_fileindex)->setDebug(true);
		}
		else {
			this->m_filelist->at(this->m_fileindex)->setDebug(false);
		}
	}
}

void ImgPRWidget::on_cb_label_clicked()
{
	if (this->m_fileindex >= 0) {
		if (this->m_cb_label->isChecked()) {
			this->m_filelist->at(this->m_fileindex)->setLabel(true);
		}
		else {
			this->m_filelist->at(this->m_fileindex)->setLabel(false);
		}
	}
}

void ImgPRWidget::on_combobox_DetectType_currentIndexChanged()
{
	if (this->m_fileindex >= 0) {
		this->m_filelist->at(this->m_fileindex)->setDetecttype(this->m_combobox_DetectType->currentIndex());
	}
}

void ImgPRWidget::on_spinbox_MaxPlates_valueChanged()
{
	if (this->m_fileindex >= 0) {
		this->m_filelist->at(this->m_fileindex)->setMaxPlates(this->m_spinbox_MaxPlates->value());
	}
}

void ImgPRWidget::updateCheckedList()
{
	this->m_checkedlist->clear();
	for (int i = 0; i < this->m_filelist->size(); i++) {
		if (this->m_filelist->at(i)->isChecked()) {
			this->m_checkedlist->push_back(i);
		}
	}

	if (this->m_checkedlist->size() == this->m_filelist->size()
		&& this->m_filelist != 0) {
		this->m_selectAllImgCheckBox->setCheckState(Qt::Checked);
	}
	else {
		this->m_selectAllImgCheckBox->setCheckState(Qt::Unchecked);
	}

	this->updateFileIndex();

	this->m_selectAllImgCheckBox->setText(
		QString(u8"全选 (%1/%2)")
		.arg(this->m_checkedlist->size())
		.arg(this->m_filelist->size())
	);
}

void ImgPRWidget::updateFileIndex()
{
	if (this->m_filelist->empty()) {
		this->initPRWidget();
		this->m_fileindex = -1;
		this->m_selectAllImgCheckBox->setEnabled(false);
	}
	else {
		this->m_selectAllImgCheckBox->setEnabled(true);

		if (this->m_fileset->find(this->m_currentFilePathLabel->text()) == this->m_fileset->end()) {
			this->m_fileindex = -1;
			this->initPRWidget();
		}
		else {
			for (int i = 0; i < this->m_filelist->size(); i++) {
				if (this->m_filelist->at(i)->getFilePath() == this->m_currentFilePathLabel->text()) {
					this->m_fileindex = i;
					break;
				}
			}
		}
	}
}

void ImgPRWidget::selectAllImgCheckBoxClicked()
{
	if (this->m_selectAllImgCheckBox->isChecked()) {
		for (int i = 0; i < this->m_filelist->size(); i++) {
			this->m_filelist->at(i)->setCheckeState(Qt::Checked);
		}
	}
	else {
		for (int i = 0; i < this->m_filelist->size(); i++) {
			this->m_filelist->at(i)->setCheckeState(Qt::Unchecked);
		}
	}
	this->updateCheckedList();
}

void ImgPRWidget::outPutImg(int index,int pattern)
{
	QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);
	QString path = config.value("IMGPR/outputpath").toString();
	QString pureFileName = this->m_filelist->at(index)->getFileName().split(".").front();
	QImage qimg(this->m_filelist->at(index)->getFilePath());

	if (pattern == 0 || this->m_filelist->at(index)->getPlates().size() == 0) {
		this->m_fWindow->m_plateRecognize->SetDebug(false);
		this->m_fWindow->m_plateRecognize->SetDetectType(this->m_filelist->at(index)->getDetecttype());
		this->m_fWindow->m_plateRecognize->SetMaxPlates(this->m_filelist->at(index)->getMaxPlates());
		this->m_filelist->at(index)->setPlates(this->m_fWindow->m_plateRecognize->PlateRecognize(TOOLS::QImage2cvMat(qimg)));
	}

	qimg = this->drawImgLabel(qimg, this->m_filelist->at(index)->getPlates());
	qimg.save(path + "/" + pureFileName + ".jpg", "JPG",100);

	QFile f(path + "/" + pureFileName + ".txt");
	f.open(QFile::WriteOnly);
	QTextStream txtOutput(&f);
	txtOutput << this->m_filelist->at(index)->toStr() << endl;
	qDebug() << this->m_filelist->at(index)->toStr() << endl;
	f.close();

}


void ImgPRWidget::openIMG(QString file_path)
{
	this->m_qimg = QImage(file_path);
	this->m_cvimg = TOOLS::QImage2cvMat(this->m_qimg);
	if (!this->m_cvimg.data) {
		this->m_fWindow->statusBar()->showMessage(u8"图片打开成功！");
	}
	else {
		this->m_fWindow->statusBar()->showMessage(u8"图片打开失败！");
	}
}

void ImgPRWidget::setShowPIMG(QImage qimg)
{
	this->m_imgLabel->setPixmap(QPixmap::fromImage(qimg));
	this->m_imgLabel->resize(qimg.size().width(), qimg.size().height());
}

void ImgPRWidget::setShowQIMG()
{
	this->m_imgLabel->setPixmap(QPixmap::fromImage(this->m_qimg));
	this->m_imgLabel->resize(this->m_qimg.size().width(), this->m_qimg.size().height());
}

void ImgPRWidget::setShowPlate()
{

	this->m_combobox_Plate->setCurrentIndex(this->m_plateindex);
	if (this->m_cb_label->isChecked()) {
		QImage tmp = this->drawImgLabel(
			TOOLS::cvMat2QImage(this->m_cvimg),
			this->m_filelist->at(this->m_fileindex)->getPlates(),
			this->m_plateindex
		);
		this->setShowPIMG(tmp);
	}
	else
		this->setShowQIMG();
}

QImage ImgPRWidget::drawImgLabel(QImage imgIn, vector<siripr::CPlate>& plates, int index)
{
	QImage imgOut;
	QPixmap pixmap = QPixmap::fromImage(imgIn);
	QPainter painter(&pixmap);
	QFont font;
	font.setPixelSize(16);

	for (int i = 0; i < plates.size(); i++) {
		cv::RotatedRect rrect = plates[i].getPlatePos();
		cv::Rect rect = rrect.boundingRect();
		QString color = "#0f0";
		if (i == index)
			color = "#f00";
		QString text = TOOLS::StdStringToQString(std::string(plates[i].getPlateStr()));

		//绘制矩形框
		painter.setPen(QPen(QColor(color), 3));
		painter.drawRect(rect.x, rect.y, rect.width, rect.height);

		//文字背景
		painter.save();
		painter.setPen(QPen(QColor(color), 3));
		painter.setBrush(QBrush(QColor(color)));
		QFontMetrics metrics(font);
		int w = metrics.width(text); //获取显示文本的宽度
		int h = metrics.height(); //获取显示文本的高度
		painter.drawRect(rect.x, rect.y - h - 2, w, h + 2);
		painter.restore();

		//绘制文本
		painter.setFont(font);
		painter.setPen(QPen(QColor("#000"), 3));
		painter.drawText(rect.x, rect.y - 1, text);

	}

	imgOut = pixmap.toImage();
	return imgOut;
}
