#include "MainWindow.h"

#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qstatusbar.h>
#include <qicon.h>
#include <qlayout.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qdialog.h>
#include <qimage.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qfont.h>

#include "tools.hpp"

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    this->m_plateRecognize = new CPRecognize();
    
    //初始化UI界面
    this->initUI();

}

void MainWindow::initUI() 
{

    this->resize(1200, 800);//设置窗口尺寸
    this->setMinimumSize(800, 600);//设置窗口最小尺寸
    this->setStyleSheet(TOOLS::ReadQSS(this->MAINWINDOW_STYLE));//设置窗口QSS样式
    //将主窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
    this->setWindowTitle("SiriPR");//设置窗口标题
    this->setWindowIcon(QIcon("./SiriPRGUI/img/icon/siripr_icon_1000_1000.png"));//设置窗口图标
    
    //设置状态栏
    this->statusBar()->showMessage(u8"欢迎使用SiriPR");

    this->m_imgPRBtn = new QPushButton(u8"图片识别");
    this->m_imgPRBtn->setFixedHeight(30);
    this->m_imgPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_ON_STYLE));
    this->m_imgPRBtn->setStatusTip(u8"图片识别");
    connect(this->m_imgPRBtn, &QPushButton::clicked, this, &MainWindow::On_ImgRecognizeAct_clicked);

    this->m_videoPRBtn = new QPushButton(u8"视频识别");
    this->m_videoPRBtn->setFixedHeight(30);
    this->m_videoPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_videoPRBtn->setStatusTip(u8"视频识别");
    connect(this->m_videoPRBtn, &QPushButton::clicked, this, &MainWindow::On_VideoRecognizeAct_clicked);

    this->m_settingBtn = new QPushButton(u8"系统设置");
    this->m_settingBtn->setFixedHeight(30);
    this->m_settingBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_settingBtn->setStatusTip(u8"系统设置");
    connect(this->m_settingBtn, &QPushButton::clicked, this, &MainWindow::On_SettingAct_clicked);

    QHBoxLayout* topLayoyt = new QHBoxLayout();
    topLayoyt->setContentsMargins(0, 0, 0, 0);
    topLayoyt->setSpacing(0);
    topLayoyt->addWidget(this->m_imgPRBtn);
    topLayoyt->addWidget(this->m_videoPRBtn);
    topLayoyt->addWidget(this->m_settingBtn);

    this->m_ImgPRWidget = new ImgPRWidget(this);
    this->m_ImgPRWidget->move(0, 0);
    this->m_ImgPRWidget->hide();

    this->m_VideoPRWidget = new VideoPRWidget(this);
    this->m_VideoPRWidget->move(0, 0);
    this->m_VideoPRWidget->hide();

    this->m_SettingWidget = new SettingWidget(this);
    this->m_SettingWidget->move(0, 0);
    this->m_SettingWidget->hide();

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(this->m_ImgPRWidget);
    bottomLayout->addWidget(this->m_VideoPRWidget);
    bottomLayout->addWidget(this->m_SettingWidget);

    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);
    centerLayout->addLayout(topLayoyt);
    centerLayout->addLayout(bottomLayout);

    QWidget *centerWidget = new QWidget();
    centerWidget->setLayout(centerLayout);
    this->setCentralWidget(centerWidget);

    //设置菜单栏Action

    QAction* ImgRecognizeAct = new QAction(tr(u8"&图片识别"), this);
    ImgRecognizeAct->setStatusTip(tr(u8"图片识别"));
    connect(ImgRecognizeAct, &QAction::triggered, this, &MainWindow::On_ImgRecognizeAct_clicked);

    QAction* VideoRecognizeAct = new QAction(tr(u8"&视频识别"), this);
    VideoRecognizeAct->setStatusTip(tr(u8"视频识别"));
    connect(VideoRecognizeAct, &QAction::triggered, this, &MainWindow::On_VideoRecognizeAct_clicked);

    QAction* SettingAct = new QAction(tr(u8"&系统设置"), this);
    SettingAct->setStatusTip(tr(u8"系统设置"));
    connect(SettingAct, &QAction::triggered, this, &MainWindow::On_SettingAct_clicked);

    QAction* ExitAct = new QAction(tr(u8"&退出"), this);
    ExitAct->setShortcut(QKeySequence(tr("Ctrl+Q")));
    ExitAct->setStatusTip(tr(u8"退出"));
    connect(ExitAct, &QAction::triggered, QApplication::closeAllWindows);

    QAction* AboutAct = new QAction(tr(u8"&关于"), this);
    AboutAct->setStatusTip(tr(u8"关于SiriPR"));
    connect(AboutAct, &QAction::triggered, this, &MainWindow::On_AboutAct_clicked);

    QMenu* fileMenu = this->menuBar()->addMenu(tr(u8"&主菜单"));
    fileMenu->addAction(ImgRecognizeAct);
    fileMenu->addAction(VideoRecognizeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(SettingAct);
    fileMenu->addSeparator();
    fileMenu->addAction(ExitAct);

    QMenu* helpMenu = this->menuBar()->addMenu(tr(u8"&帮助"));
    helpMenu->addAction(AboutAct);

    this->m_ImgPRWidget->show();
    this->m_VideoPRWidget->hide();
    this->m_SettingWidget->hide();
}

void MainWindow::On_ImgRecognizeAct_clicked() 
{
    this->m_ImgPRWidget->show();
    this->m_VideoPRWidget->hide();
    this->m_SettingWidget->hide();

    this->m_imgPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_ON_STYLE));
    this->m_videoPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_settingBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
}

void MainWindow::On_VideoRecognizeAct_clicked()
{
    this->m_ImgPRWidget->hide();
    this->m_VideoPRWidget->show();
    this->m_SettingWidget->hide();

    this->m_imgPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_videoPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_ON_STYLE));
    this->m_settingBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));   
}

void MainWindow::On_SettingAct_clicked()
{
    this->m_ImgPRWidget->hide();
    this->m_VideoPRWidget->hide();
    this->m_SettingWidget->show();

    this->m_imgPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_videoPRBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_OFF_STYLE));
    this->m_settingBtn->setStyleSheet(TOOLS::ReadQSS(this->TOP_BTN_ON_STYLE));
}

void MainWindow::On_AboutAct_clicked()
{
    QDialog* aboutWidget = new QDialog(this);
    aboutWidget->setAttribute(Qt::WA_DeleteOnClose);
    aboutWidget->setWindowTitle(u8"关于");
    aboutWidget->setStyleSheet("background-color: white;");
    aboutWidget->setFixedSize(350, 450);
    QDesktopWidget* desktop = QApplication::desktop();
    aboutWidget->move((desktop->width() - aboutWidget->width()) / 2, (desktop->height() - aboutWidget->height()) / 2);

    QImage *qimg = new QImage("./SiriPRGUI/img/icon/siripr_icon_1000_1000.png");
    *qimg=qimg->scaled(200, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QLabel* imgLabel = new QLabel();
    imgLabel->setPixmap(QPixmap::fromImage(*qimg));
    imgLabel->setAlignment(Qt::AlignCenter);

    QLabel* infoLabel = new QLabel();
    infoLabel->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));
    infoLabel->setText(u8"SiriPR v1.0\n\n基于 libsiripr 的中国车牌识别系统");
    infoLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QLabel* buttomLabel = new QLabel();
    buttomLabel->setFont(QFont("Microsoft YaHei", 10, QFont::Normal));
    buttomLabel->setText(u8"Copyright © 2020 Siriyang\nblog.siriyang.cn");
    buttomLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setAlignment(Qt::AlignHCenter);
    vlayout->addWidget(imgLabel);
    vlayout->addWidget(infoLabel);
    vlayout->addWidget(buttomLabel);

    aboutWidget->setLayout(vlayout);
    aboutWidget->exec();
}
