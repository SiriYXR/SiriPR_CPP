#include "ImgOutPortWidget.h"

#include <qlayout.h>

#include "ImgFileWidget.h"
#include "tools.hpp"

#include <qdebug.h>

ImgOutPortWidget::ImgOutPortWidget(ImgPRWidget* parent)
{
	this->m_fWindow = parent;
	this->m_thread = nullptr;
	this->m_tick = 0;

	this->initUI();
}

void ImgOutPortWidget::initUI()
{
	this->setStyleSheet(TOOLS::ReadQSS(this->MAIN_STYLE)
		+ TOOLS::ReadQSS(this->IOS_RADIOBTN_STYLE));
	this->setWindowTitle(u8"图片识别结果导出");
	this->setFixedSize(300, 200);
    this->setWindowIcon(QIcon("./SiriPRGUI/img/icon/siripr_icon_1000_1000.png"));//设置窗口图标

	//------------------layout_setting-------------------
    QVBoxLayout* layout_setting = new QVBoxLayout();
    layout_setting->setContentsMargins(0, 0, 0, 0);
    layout_setting->setSpacing(0);
    layout_setting->setAlignment(Qt::AlignCenter);

    this->m_settingLabel = new QLabel(u8"设置识别操作执行策略");
    this->m_settingLabel->setAlignment(Qt::AlignCenter);
    this->m_settingLabel->setFixedHeight(20);

    this->m_radioButton1 = new QRadioButton(u8"全部重新识别");
    this->m_radioButton1->setChecked(true);
    this->m_radioButton2 = new QRadioButton(u8"已有识别结果的直接保存");
    this->m_radioButton2->setFixedWidth(200);

    QHBoxLayout* layout_radio = new QHBoxLayout();
    layout_radio->setContentsMargins(10, 0, 0, 0);
    layout_radio->setSpacing(10);
    layout_radio->addWidget(this->m_radioButton1);
    layout_radio->addWidget(this->m_radioButton2);

    this->m_startBtn = new QPushButton(u8"开始");
    this->m_startBtn->setFixedSize(300, 30);
    connect(this->m_startBtn, &QPushButton::clicked, this, &ImgOutPortWidget::on_startBtn_clicked);

    layout_setting->addStretch(2);
    layout_setting->addWidget(this->m_settingLabel);
    layout_setting->addStretch(2);
    layout_setting->addLayout(layout_radio);
    layout_setting->addStretch(3);
    layout_setting->addWidget(this->m_startBtn);

	//------------------layout_process-------------------
    QVBoxLayout* layout_process = new QVBoxLayout();
    layout_process->setContentsMargins(0, 0, 0, 0);
    layout_process->setSpacing(0);

    this->m_processLabel = new QLabel(u8"导出：");

    this->m_progressBar = new QProgressBar();
    this->m_progressBar->setFixedSize(260, 30);
    this->m_progressBar->setValue(0);

    QVBoxLayout* layout_progress = new QVBoxLayout();
    layout_progress->setContentsMargins(20, 0, 0, 0);
    layout_progress->setSpacing(5);
    layout_progress->addWidget(this->m_processLabel);
    layout_progress->addWidget(this->m_progressBar);

    this->m_cancellBtn = new QPushButton(u8"取消");
    this->m_cancellBtn->setFixedSize(300, 30);
    connect(this->m_cancellBtn, &QPushButton::clicked, this, &ImgOutPortWidget::on_cancellBtn_clicked);

    layout_process->addStretch(3);
    layout_process->addLayout(layout_progress);
    layout_process->addStretch(3);
    layout_process->addWidget(this->m_cancellBtn);
	//------------------layout_end-------------------
    QVBoxLayout* layout_end = new QVBoxLayout();
    layout_end->setContentsMargins(0, 0, 0, 0);
    layout_end->setSpacing(0);

    this->m_endLabel = new QLabel(u8"导出结束");
    this->m_endLabel->setAlignment(Qt::AlignCenter);

    this->m_endBtn = new QPushButton(u8"完成");
    this->m_endBtn->setFixedSize(300, 30);
    connect(this->m_endBtn, &QPushButton::clicked, this, &ImgOutPortWidget::on_endBtn_clicked);

    layout_end->addWidget(this->m_endLabel);
    layout_end->addWidget(this->m_endBtn);

    this->m_settingWidget = new QWidget(this);
    this->m_settingWidget->setFixedSize(300, 200);
    this->m_settingWidget->setHidden(false);
    this->m_settingWidget->setLayout(layout_setting);

    this->m_processWidget = new QWidget(this);
    this->m_processWidget->setFixedSize(300, 200);
    this->m_processWidget->setHidden(true);
    this->m_processWidget->setLayout(layout_process);

    this->m_endWidget = new QWidget(this);
    this->m_endWidget->setFixedSize(300, 200);
    this->m_endWidget->setHidden(true);
    this->m_endWidget->setLayout(layout_end);

    this->exec();
}

void ImgOutPortWidget::on_startBtn_clicked() 
{
	this->m_tick = clock();
    this->m_settingWidget->setHidden(true);
    this->m_processWidget->setHidden(false);

    int pattern = 0;
    if (this->m_radioButton2->isChecked()) {
        pattern = 1;
    }

    // 创建线程
    this->m_thread = new OutportImgThread(this->m_fWindow, pattern);
    // 连接信号
    connect(this->m_thread, &OutportImgThread::_signal, this, &ImgOutPortWidget::call_backlog);// 进程连接回传到GUI的事件
    // 开始线程
    this->m_thread->start();
}

void ImgOutPortWidget::on_cancellBtn_clicked()
{
	this->m_thread->m_cancell = true;
	this->close();
}

void ImgOutPortWidget::on_endBtn_clicked()
{
	this->close();
}


void ImgOutPortWidget::call_backlog(int msg)
{
    if (msg == -1) {
        delete this->m_thread;
        int cost_time = clock() - this->m_tick;
        this->m_endLabel->setText(u8"导出结束\n用时:" + TOOLS::TickTimeProcess(cost_time));
        this->m_processWidget->setHidden(true);
        this->m_endWidget->setHidden(false);
        return;
    }

    int checked_index = msg;
    int checked_length = this->m_fWindow->getCheckedList()->size();
    int file_index = this->m_fWindow->getCheckedList()->at(checked_index);
    QString file_name = this->m_fWindow->getFileList()->at(file_index)->getFileName();
    this->m_processLabel->setText(u8"导出：" + file_name);
    this->m_progressBar->setValue(int((checked_index + 1) * 100.0 / checked_length));//将线程的参数传入进度条
}
