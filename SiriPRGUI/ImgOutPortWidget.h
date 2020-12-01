#pragma once

#include <time.h>

#include <qwidget.h>
#include <qdialog.h>
#include <qthread.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlabel.h>
#include <qradiobutton.h>

#include "ImgPRWidget.h"

class OutportImgThread :public QThread
{
    Q_OBJECT

signals:
    void _signal(int str);

public:
    OutportImgThread(ImgPRWidget* parent = Q_NULLPTR,int pattern=0)
    {
        this->m_fWindow = parent;
        this->m_pattern = pattern;
        this->m_cancell = false;
    };

    ~OutportImgThread()
    {
        this->wait();
    };

    void run()
    {
        for (int i = 0; i < this->m_fWindow->getCheckedList()->size(); i++) {
            this->m_fWindow->outPutImg(this->m_fWindow->getCheckedList()->at(i), this->m_pattern);
            emit _signal(i);

            if (this->m_cancell)
                return;
        }

        emit _signal(-1);
    };

    ImgPRWidget* m_fWindow;
    int m_pattern;
    bool m_cancell;
};

class ImgOutPortWidget :
    public QDialog
{
    Q_OBJECT

    QString MAIN_STYLE = "./SiriPRGUI/qss/imgoutportwidget/main_style.qss";
    QString IOS_RADIOBTN_STYLE = "./SiriPRGUI/qss/imgoutportwidget/ios_radiobtn_style.qss";

public:
    ImgOutPortWidget(ImgPRWidget* parent = Q_NULLPTR);

public slots:
    void on_startBtn_clicked();
    void on_cancellBtn_clicked();
    void on_endBtn_clicked();

private:

    void initUI();
    void call_backlog(int msg);

    ImgPRWidget* m_fWindow;
    OutportImgThread* m_thread;
    clock_t m_tick;

    //------------------layout_setting-------------------
    QLabel* m_settingLabel;
    QRadioButton* m_radioButton1;
    QRadioButton* m_radioButton2;
    QPushButton* m_startBtn;

    //------------------layout_process-------------------
    QLabel* m_processLabel;
    QProgressBar* m_progressBar;
    QPushButton* m_cancellBtn;

    //------------------layout_end-------------------
    QLabel* m_endLabel;
    QPushButton* m_endBtn;
    QWidget* m_settingWidget;
    QWidget* m_processWidget;
    QWidget* m_endWidget;
};

