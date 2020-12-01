#pragma once

#include <vector>
#include <qwidget.h>
#include <qbasictimer.h>
#include <qlist.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qevent.h>
#include <qlayout.h>

#include "CPRecognize.h"

class MainWindow;
class ImgFileWidget;
class OutportImgThread;
class ImgOutPortWidget;

class MyComboBox :public QComboBox
{
    Q_OBJECT

signals:
    void popupAboutToBeShown();

public:
    /*MyComboBox(QWidget* parent = Q_NULLPTR)
    {
        this->QComboBox::QComboBox(parent);
    };*/

    // 重写showPopup函数
    void showPopup()
    {
        emit popupAboutToBeShown(); // 发送信号
        this->QComboBox::showPopup();// 弹出选项框
    };

};

class ImgPRWidget :
    public QWidget
{
    Q_OBJECT

    friend OutportImgThread;
    friend ImgOutPortWidget;

    QString MAIN_STYLE = "./SiriPRGUI/qss/imgprwidget/main_style.qss";
    QString SCROLLAREA_STYLE = "./SiriPRGUI/qss/imgprwidget/scrollarea_style.qss";
    QString IOS_CHECKBOX_STYLE = "./SiriPRGUI/qss/imgprwidget/ios_checkbox_style.qss";

public:
    ImgPRWidget(MainWindow* parent = Q_NULLPTR);
    
    void showImageFile(QString file_path);
    vector<int>* getCheckedList();
    vector<ImgFileWidget*>* getFileList();

    //overwrite
    void show();
    void keyPressEvent(QKeyEvent* event);

public slots:
    void on_btn_last_img_clicked();
    void on_btn_next_img_clicked();
    void on_btn_inport_clicked();
    void on_btn_remove_clicked();
    void on_btn_outport_clicked();
    void on_btn_recognize_clicked();
    void on_combobox_Plate_clicked();
    void on_btn_next_plate_clicked();
    void on_btn_last_plate_clicked();
    void on_cb_debug_clicked();
    void on_cb_label_clicked();
    void on_combobox_DetectType_currentIndexChanged();
    void on_spinbox_MaxPlates_valueChanged();
    void updateCheckedList();

private:

    void initUI();
    void initPRWidget();
    void openIMG(QString file_path);
    void setShowPIMG(QImage qimg);
    void setShowQIMG();
    void setShowPlate();
    QImage drawImgLabel(QImage imgIn,vector<siripr::CPlate>& plates,int index =-1);
    void updateFileIndex();
    void selectAllImgCheckBoxClicked();
    void outPutImg(int index, int pattern);

    cv::Mat m_cvimg;
    QImage m_qimg;
    siripr::CPlate plate;
    vector<ImgFileWidget*>* m_filelist;
    set<QString>* m_fileset;
    vector<int>* m_checkedlist;
    int m_fileindex;
    int m_plateindex;

    bool m_isCheckedAll;

    MainWindow* m_fWindow;

    QCheckBox* m_selectAllImgCheckBox;
    QPushButton* m_inportBtn;
    QPushButton* m_removeBtn;
    QPushButton* m_outportBtn;
    QVBoxLayout* m_imgListContentLayout;
    QWidget* m_imgListScrollContent;
    QScrollArea* m_imgListScrollArea;

    //-------------------- topLayout------------------------
    QLabel* m_label_RunningMSG;

    QLabel* m_imgLabel;
    QScrollArea* m_sa_img;
    //--------------------currentFilePathLayout------------------------
    QLabel* m_currentFilePathLabel;
    QPushButton* m_btn_last_img;
    QPushButton* m_btn_next_img;

    //--------------------prOPlayout------------------------
    QPushButton* m_btn_recognize;
    MyComboBox* m_combobox_Plate;
    //QComboBox* m_combobox_Plate;
    QPushButton* m_btn_last_plate;
    QPushButton* m_btn_next_plate;
    //--------------------setinglayout------------------------
    QCheckBox* m_cb_debug;
    QCheckBox* m_cb_label;
    QLabel* m_label_DetectType;
    QComboBox* m_combobox_DetectType;
    QLabel* m_label_MaxPlates;
    QSpinBox* m_spinbox_MaxPlates;

};