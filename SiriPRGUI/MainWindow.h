#pragma once

#include <QtWidgets/QMainWindow>
#include <qstring.h>
#include <qpushbutton.h>

#include "ImgPRWidget.h"
#include "VideoPRWidget.h"
#include "SettingWidget.h"
#include "CPRecognize.h"

//#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString MAINWINDOW_STYLE = "./SiriPRGUI/qss/mainwindow/mainwindow_style.qss";
    QString TOP_BTN_ON_STYLE = "./SiriPRGUI/qss/mainwindow/top_btn_on_style.qss";
    QString TOP_BTN_OFF_STYLE = "./SiriPRGUI/qss/mainwindow/top_btn_off_style.qss";

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    CPRecognize* m_plateRecognize;

private slots:
    void On_ImgRecognizeAct_clicked();
    void On_VideoRecognizeAct_clicked();
    void On_SettingAct_clicked();
    void On_AboutAct_clicked();

private:
    //Ui::MainWindowClass ui;

    void initUI();

    ImgPRWidget* m_ImgPRWidget;
    VideoPRWidget* m_VideoPRWidget;
    SettingWidget* m_SettingWidget;

    QPushButton* m_imgPRBtn;
    QPushButton* m_videoPRBtn;
    QPushButton* m_settingBtn;

};
