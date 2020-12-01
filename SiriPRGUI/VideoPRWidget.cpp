#include "VideoPRWidget.h"

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
#include "tools.hpp"

#include <qdebug.h>

VideoPRWidget::VideoPRWidget(MainWindow* parent)
{
	this->m_fWindow = parent;

	this->m_cap = NULL;
	this->m_timer = nullptr;
	this->m_playState = PlayState::STOP;
	this->m_videoType = VideoType::None;
	this->m_videoFile = "";
	this->m_fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

	this->m_cap_name = "";
	this->m_cap_frames_count = 0;
	this->m_cap_currentframe = 0;
	this->m_cap_fps = 0;
	this->m_cap_width = 0;
	this->m_cap_height = 0;

	this->m_resultlist_record = new QList<VideoPRResult>();

	this->initUI();
	this->initData();
}

void VideoPRWidget::initData()
{
	this->m_cap = NULL;
	this->m_timer = nullptr;
	this->m_playState = PlayState::STOP;
	this->m_videoType = VideoType::None;
	this->m_videoFile = "";

	this->m_cap_name = "";
	this->m_cap_frames_count = 0;
	this->m_cap_currentframe = 0;
	this->m_cap_fps = 0;
	this->m_cap_width = 0;
	this->m_cap_height = 0;

	this->m_resultlist_record->clear();

	QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);

	if (config.value("VIDEOPR/recognize").toString() == "True") {
		this->m_cb_Recognize->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_Recognize->setCheckState(Qt::Unchecked);
	}
	if (config.value("VIDEOPR/record").toString() == "True") {
		this->m_cb_Record->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_Record->setCheckState(Qt::Unchecked);
	}
	if (config.value("VIDEOPR/debug").toString() == "True") {
		this->m_cb_debug->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_debug->setCheckState(Qt::Unchecked);
	}
	if (config.value("VIDEOPR/label").toString() == "True") {
		this->m_cb_label->setCheckState(Qt::Checked);
	}
	else {
		this->m_cb_label->setCheckState(Qt::Unchecked);
	}
	this->m_combobox_DetectType->setCurrentIndex(config.value("VIDEOPR/detecttype").toInt());
	this->m_spinbox_MaxPlates->setValue(config.value("VIDEOPR/maxplates").toInt());

}

void VideoPRWidget::initUI()
{
	this->setStyleSheet(TOOLS::ReadQSS(this->MAIN_STYLE));

	//-------------------- topLayout------------------------

	this->m_label_RunningMSG = new QLabel();
	this->m_label_RunningMSG->setAlignment(Qt::AlignCenter);

	QHBoxLayout* topLayout =new QHBoxLayout();
	topLayout->addWidget(this->m_label_RunningMSG);

	//-------------------imgLabel-------------------------
	this->m_openLabel = new QLabel();
	this->m_openLabel->setFixedSize(160, 52);

	this->m_btn_Open = new QPushButton(u8"打开", this->m_openLabel);
	this->m_btn_Open->setStyleSheet(TOOLS::ReadQSS(this->BTN_STYLE));
	this->m_btn_Open->setFixedSize(80, 40);
	this->m_btn_Open->move(0, 0);
	connect(this->m_btn_Open, &QPushButton::clicked, this, &VideoPRWidget::on_btn_Open_clicked);

	this->m_combobox_Open = new QComboBox(this->m_openLabel);
	this->m_combobox_Open->addItems({ u8"本地文件", "URL", u8"摄像头" });
	this->m_combobox_Open->setFixedSize(80, 40);
	this->m_combobox_Open->move(79, 0);
	this->m_combobox_Open->setStatusTip(u8"设置视频类型");

	QHBoxLayout* imglayout = new QHBoxLayout();
	imglayout->setAlignment(Qt::AlignCenter);
	imglayout->addWidget(this->m_openLabel);

	this->m_imgLabel = new QLabel();
	this->m_imgLabel->setAlignment(Qt::AlignCenter);
	this->m_imgLabel->setMaximumSize(10000, 10000);
	this->m_imgLabel->setStyleSheet("QLabel{background-color:black;}");
	this->m_imgLabel->setLayout(imglayout);

	//--------------------videoOPlayout------------------------

	this->m_btn_Play = new QPushButton(u8"播放");
	this->m_btn_Play->setFixedSize(60, 30);
	connect(this->m_btn_Play,&QPushButton::clicked,this, &VideoPRWidget::playPlayer);

	this->m_btn_Pause = new QPushButton(u8"暂停");
	this->m_btn_Pause->setFixedSize(60, 30);
	connect(this->m_btn_Pause, &QPushButton::clicked, this, &VideoPRWidget::pausePlayer);

	this->m_btn_Stop = new QPushButton(u8"停止");
	this->m_btn_Stop->setFixedSize(60, 30);
	connect(this->m_btn_Stop, &QPushButton::clicked, this, &VideoPRWidget::stopPlayer);

	this->m_btn_LastFrame = new QPushButton("<");
	this->m_btn_LastFrame->setFixedSize(30, 30);
	connect(this->m_btn_LastFrame, &QPushButton::clicked, this, &VideoPRWidget::lastFrame);

	this->m_btn_NextFrame = new QPushButton(">");
	this->m_btn_NextFrame->setFixedSize(30, 30);
	connect(this->m_btn_NextFrame, &QPushButton::clicked, this, &VideoPRWidget::nextFrame);

	this->m_sld_Frame = new QSlider(Qt::Horizontal);
	this->m_sld_Frame->setFixedHeight(30);
	connect(this->m_sld_Frame, &QSlider::valueChanged, this, &VideoPRWidget::frameSlidValueChange);

	QHBoxLayout* videoOPlayout = new QHBoxLayout();
	videoOPlayout->addWidget(this->m_btn_Play);
	videoOPlayout->addWidget(this->m_btn_Pause);
	videoOPlayout->addWidget(this->m_btn_Stop);
	videoOPlayout->addWidget(this->m_btn_LastFrame);
	videoOPlayout->addWidget(this->m_sld_Frame);
	videoOPlayout->addWidget(this->m_btn_NextFrame);

	videoOPlayout->setAlignment(Qt::AlignLeft);

	//-------------------- prOPlayout------------------------
	this->m_cb_Recognize = new QCheckBox(u8"识别");
	this->m_cb_Recognize->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));

	this->m_cb_Record = new QCheckBox(u8"录制");
	this->m_cb_Record->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));

	this->m_label_RecognizeMSG = new QLabel();
	this->m_label_RecognizeMSG->setAlignment(Qt::AlignCenter);

	QHBoxLayout* prOPlayout = new QHBoxLayout();
	prOPlayout->setAlignment(Qt::AlignLeft);

	prOPlayout->addWidget(this->m_cb_Recognize);
	prOPlayout->addWidget(this->m_cb_Record);
	prOPlayout->addWidget(this->m_label_RecognizeMSG);
	prOPlayout->setStretchFactor(this->m_label_RecognizeMSG, 1);

	//-------------------- prSetlayout-----------------------
	this->m_cb_debug = new QCheckBox(u8"调试");
	this->m_cb_debug->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));
	this->m_cb_label = new QCheckBox(u8"标注结果");
	this->m_cb_label->setStyleSheet(TOOLS::ReadQSS(this->IOS_CHECKBOX_STYLE));

	this->m_label_DetectType = new QLabel(u8"检测类型");
	this->m_combobox_DetectType = new QComboBox();
	this->m_combobox_DetectType->setFixedSize(120, 20);
	this->m_combobox_DetectType->addItems(
		{ "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" });
	this->m_combobox_DetectType->setStatusTip(u8"设置检测类型");

	this->m_label_MaxPlates = new QLabel(u8"最大车牌数");
	this->m_spinbox_MaxPlates = new QSpinBox();
	this->m_spinbox_MaxPlates->setFixedSize(50, 22);
	this->m_spinbox_MaxPlates->setAlignment(Qt::AlignCenter);
	this->m_spinbox_MaxPlates->setMinimum(1);
	this->m_spinbox_MaxPlates->setMaximum(10);
	this->m_spinbox_MaxPlates->setStatusTip(u8"设置单帧检测车牌最大数量");

	QHBoxLayout* prSetlayout = new QHBoxLayout();
	prSetlayout->setAlignment(Qt::AlignLeft);

	prSetlayout->addWidget(this->m_cb_debug);
	prSetlayout->addWidget(this->m_cb_label);
	prSetlayout->addWidget(this->m_label_DetectType);
	prSetlayout->addWidget(this->m_combobox_DetectType);
	prSetlayout->addWidget(this->m_label_MaxPlates);
	prSetlayout->addWidget(this->m_spinbox_MaxPlates);
	prSetlayout->addStretch(1);

	//-------------------- vlayout------------------------
	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->addLayout(topLayout);
	vlayout->addWidget(this->m_imgLabel);
	vlayout->setStretchFactor(this->m_imgLabel, 1);
	vlayout->addLayout(videoOPlayout);
	vlayout->addLayout(prOPlayout);
	vlayout->addLayout(prSetlayout);

	this->setLayout(vlayout);

	this->initButton();
}

void VideoPRWidget::initButton()
{
	this->m_btn_Play->setEnabled(false);
	this->m_btn_Pause->setEnabled(false);
	this->m_btn_Stop->setEnabled(false);
	this->m_btn_LastFrame->setEnabled(false);
	this->m_btn_NextFrame->setEnabled(false);
	this->m_sld_Frame->setEnabled(false);
	this->m_sld_Frame->setValue(0);
}

bool VideoPRWidget::OpenVideo()
{
	this->m_cap.open(TOOLS::QStringToStdString(this->m_videoFile));
	if (!this->m_cap.isOpened()) {
		return false;
	}
	return true;
}

void VideoPRWidget::initPlayer()
{
	this->m_cap_width = this->m_cap.get(3);
	this->m_cap_height = this->m_cap.get(4);
	this->m_cap_fps = 30;

	this->m_btn_Pause->setEnabled(true);
	this->m_btn_Stop->setEnabled(true);

	if (this->m_videoType == VideoType::VIDEOFILE) {
		this->m_cap_fps = this->m_cap.get(5);
		this->m_cap_frames_count = this->m_cap.get(7);
		this->m_btn_LastFrame->setEnabled(true);
		this->m_btn_NextFrame->setEnabled(true);
		this->m_sld_Frame->setEnabled(true);
		this->m_sld_Frame->setMaximum(this->m_cap_frames_count);
	}

	QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);
	QString path = config.value("VIDEOPR/outputpath").toString() + "/" + this->m_cap_name + "_record.avi";
	this->m_videoWriter.open(TOOLS::QStringToStdString(path), this->m_fourcc, this->m_cap_fps, cv::Size(this->m_cap_width, this->m_cap_height),true);
	
	this->m_timer = new QBasicTimer();
	this->m_timer->start(1000 / m_cap_fps, this);

	this->m_playState = PlayState::PLAY;
	this->m_openLabel->hide();
}

void VideoPRWidget::updateFrame()
{
	Mat cvtemp;
	QImage qtemp;
	if (this->m_cap.read(cvtemp)) {
		qtemp = TOOLS::cvMat2QImage(cvtemp);
		vector<siripr::CPlate> platelist;

		if (this->m_cb_Recognize->isChecked()) {
			this->m_fWindow->m_plateRecognize->SetDebug(this->m_cb_debug->isChecked());
			this->m_fWindow->m_plateRecognize->SetDetectType(this->m_combobox_DetectType->currentIndex());
			this->m_fWindow->m_plateRecognize->SetMaxPlates(this->m_spinbox_MaxPlates->value());

			clock_t begin, end;
			begin = clock();
			platelist = this->m_fWindow->m_plateRecognize->PlateRecognize(cvtemp);
			end = clock();
			double runtime = (end - begin) / 1000.0;

			if (this->m_cb_label->isChecked()) {
				qtemp = this->drawImgLabel(qtemp, platelist);
			}

			this->m_label_RecognizeMSG->setText(
				QString(u8"发现 %1 张车牌   运行时间:%2s")
				.arg(platelist.size())
				.arg(QString::number(runtime,'f',3))
			);
		}

		if (this->m_cb_Record->isChecked()) {
			this->m_videoWriter.write(TOOLS::QImage2cvMat(qtemp));

			if (!platelist.empty()) {
				this->m_resultlist_record->append(
					VideoPRResult(
					this->m_cap_currentframe,
					this->m_combobox_DetectType->currentIndex(),
					this->m_spinbox_MaxPlates->value(),
					platelist)
				);
			}
		}
		
		//直接在QImage上绘图
		this->m_imgLabel->setPixmap(QPixmap::fromImage(qtemp));

		if (this->m_videoType == VideoType::VIDEOFILE) {
			this->m_cap_currentframe = this->m_cap.get(1);
			this->m_sld_Frame->setValue(this->m_cap_currentframe);
		}

		this->m_label_RunningMSG->setText(
			QString(u8"帧数:%1/%2  FPS:%3   宽:%4    高:%5")
			.arg(this->m_cap_currentframe)
			.arg(this->m_cap_frames_count)
			.arg(this->m_cap_fps)
			.arg(this->m_cap_width)
			.arg(this->m_cap_height)
		);
	}
}

QImage VideoPRWidget::drawImgLabel(QImage imgIn, vector<siripr::CPlate>& plates)
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
		painter.drawRect(rect.x, rect.y-h-2, w, h+2);
		painter.restore();

		//绘制文本
		painter.setFont(font);
		painter.setPen(QPen(QColor("#000"), 3));
		painter.drawText(rect.x, rect.y-1, text);
		
	}

	imgOut = pixmap.toImage();
	return imgOut;
}

QString VideoPRWidget::resultStr()
{
	QStringList detectTypeList = { "SOBEL", "COLOR", "CMSER", "SOBEL&COLOR", "SOBEL&CMSER", "COLOR&CMSER", "All" };

	QString strOut = u8"";
	for (auto i : *this->m_resultlist_record) {

		strOut += QString(u8"frame：%1\ndetecttype：%2\tmaxplates：%3\n")
			.arg(i.frame)
			.arg(detectTypeList[i.detecttype])
			.arg(i.maxplates);

		for (auto j : i.plates) {
			cv::RotatedRect rrect = j.getPlatePos();
			cv::Rect rect=rrect.boundingRect();
			strOut += QString(u8"plate_license：%1\tx：%2\ty：%3\tw：%4\th：%5\n")
				.arg(TOOLS::StdStringToQString(std::string(j.getPlateStr())))
				.arg(rect.x)
				.arg(rect.y)
				.arg(rect.width)
				.arg(rect.height);
		}
	}
	return strOut;
}


void VideoPRWidget::on_btn_Open_clicked()
{
	int index = this->m_combobox_Open->currentIndex();

	if (index == 0) {

		this->m_videoFile = QFileDialog::getOpenFileName(this,
			u8"打开文件",
			"./resources/vidoe",
			u8"视频文件(*.mp4 *.avi *.rmvb *.mkv *.flv)");
		
		if (this->m_videoFile.isEmpty()) {
			return;
		}
		this->m_cap_name = this->m_videoFile.split("/").back();

		if (this->OpenVideo()) {
			this->m_videoType = VideoType::VIDEOFILE;
			this->initPlayer();
			this->m_fWindow->statusBar()->showMessage(u8"视频打开成功!");
		}
		else {
			this->m_fWindow->statusBar()->showMessage(u8"视频打开失败!");
		}
	}
	else if (index==1) {

		bool ok;
		QString text = QInputDialog::getText(this, "URL",
			u8"输入URL:", QLineEdit::Normal,
			"", &ok);
		if (ok && !text.isEmpty()) {
			this->m_videoFile = text;

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_date = current_date_time.toString("yyyyMMddhhmmss");
			this->m_cap_name = "URL" + current_date;

			if (this->OpenVideo()) {
				this->m_videoType = VideoType::URL;
				this->initPlayer();
				this->m_fWindow->statusBar()->showMessage(u8"URL打开成功!");
			}
			else {
				this->m_fWindow->statusBar()->showMessage(u8"URL打开失败!");
			}
		}
			
	}
	else
	{
		bool ok;
		int i = QInputDialog::getInt(this, u8"摄像头端口",
			u8"输入摄像头端口:", 0, 0, 100, 1, &ok);
		if (ok) {
			this->m_videoFile = QString(i);

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_date = current_date_time.toString("yyyyMMddhhmmss");
			this->m_cap_name = "CAM" + current_date;

			if (this->OpenVideo()) {
				this->m_videoType = VideoType::CAM;
				this->initPlayer();
				this->m_fWindow->statusBar()->showMessage(u8"摄像头打开成功!");
			}
			else {
				this->m_fWindow->statusBar()->showMessage(u8"摄像头打开失败!");
			}
		}

	}
}

void VideoPRWidget::playPlayer()
{
	if (this->m_playState == PlayState::PAUSE) {
		this->m_playState = PlayState::PLAY;
		this->m_btn_Play->setEnabled(false);
		this->m_btn_Pause->setEnabled(true);
	}
}

void VideoPRWidget::pausePlayer()
{
	if (this->m_playState == PlayState::PLAY) {
		this->m_playState = PlayState::PAUSE;
		this->m_btn_Play->setEnabled(true);
		this->m_btn_Pause->setEnabled(false);
	}
}

void VideoPRWidget::stopPlayer()
{
	if (this->m_cap.isOpened()) {
		this->m_cap.release();
		if(this->m_videoWriter.isOpened())
			this->m_videoWriter.release();
	}
	
	if (this->m_cb_Record->isChecked()) {
		QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);
		QFile f(config.value("VIDEOPR/outputpath").toString() + "/" + this->m_cap_name + "_record.txt");
		f.open(QFile::Append);
		QTextStream txtOutput(&f);
		txtOutput << this->resultStr() << endl;
		qDebug() << this->resultStr() << endl;
		f.close();
	}
	
	this->m_imgLabel->clear();
	this->m_openLabel->show();
	this->initButton();
	this->m_label_RunningMSG->setText("");
	this->m_label_RecognizeMSG->setText("");
	this->m_playState = PlayState::STOP;
	this->m_timer->stop();
}

void VideoPRWidget::lastFrame()
{
	if (this->m_cap_currentframe > 0) {
		this->m_sld_Frame->setValue(this->m_cap_currentframe - 2);
		this->pausePlayer();
	}
}

void VideoPRWidget::nextFrame()
{
	if (this->m_cap_currentframe < this->m_cap_frames_count -3) {
		this->m_sld_Frame->setValue(this->m_cap_currentframe + 1);
		this->pausePlayer();
	}
}

void VideoPRWidget::frameSlidValueChange(int value)
{
	if (value != this->m_cap_currentframe) {
		this->m_cap_currentframe = value;
		this->m_cap.set(cv::CAP_PROP_POS_FRAMES, this->m_cap_currentframe);
		this->updateFrame();
	}
}

void VideoPRWidget::hide()
{
	if (this->m_playState != PlayState::STOP) {
		this->m_timer->stop();
		this->stopPlayer();
	}
	this->QWidget::hide();
}

void VideoPRWidget::show()
{
	this->m_openLabel->show();
	this->initData();
	this->QWidget::show();
}

void VideoPRWidget::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == this->m_timer->timerId()) {
		if (this->m_playState == PlayState::PLAY) {
			this->updateFrame();
		}

		if (this->m_videoType == VideoType::VIDEOFILE && this->m_cap_currentframe >= this->m_cap_frames_count - 3) {
			this->stopPlayer();
		}
	}
	else {
		this->QWidget::timerEvent(event);
	}
}

void VideoPRWidget::keyPressEvent(QKeyEvent* event)
{
	int key = event->key();

	if (key == Qt::Key_Space) {
		if (this->m_playState == PlayState::PLAY) {
			this->pausePlayer();
		}
		else if (this->m_playState == PlayState::PAUSE) {
			this->playPlayer();
		}
		return;
	}

	if (key == Qt::Key_Left) {
		if (this->m_playState != PlayState::STOP &&this->m_videoType==VideoType::VIDEOFILE){
			this->lastFrame();
		}
		return;
	}

	if (key == Qt::Key_Right) {
		if (this->m_playState != PlayState::STOP && this->m_videoType == VideoType::VIDEOFILE) {
			this->nextFrame();
		}
		return;
	}

	this->QWidget::keyPressEvent(event);
}