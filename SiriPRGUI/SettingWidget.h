#pragma once

#include <qwidget.h>
#include <qstring.h>
#include <qsettings.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>

class MainWindow;

class SettingWidget :
    public QWidget
{
    Q_OBJECT

    QString BTN_STYLE = "./SiriPRGUI/qss/settingwidget/btn_style.qss";
    QString SCROLLAREA_STYLE = "./SiriPRGUI/qss/settingwidget/scrollarea_style.qss";

public:
    SettingWidget(MainWindow* parent = Q_NULLPTR);

    void show();

private slots:
    void on_btn_SVM_clicked();
    void on_btn_ANN_clicked();
    void on_btn_ChineseANN_clicked();
    void on_btn_GrayChANN_clicked();
    void on_btn_ChineseMapping_clicked();

    void on_btn_ImgPR_OutPutPath_clicked();
    void on_btn_VideoPR_OutPutPath_clicked();

    void on_btn_defualt_clicked();
    void on_btn_cancel_clicked();
    void on_btn_apply_clicked();
private:


    void initUI();

    void saveConfig();

    void resetData();

    void defualtData();

    MainWindow* m_fWindow;

    //------------------模型路径设置------------------
    QLabel* m_label_ModelPath;

    QLabel* m_label_SVM;
    QLineEdit* m_lineedit_SVM;
    QPushButton* m_btn_SVM;

    QLabel* m_label_ANN;
    QLineEdit* m_lineedit_ANN;
    QPushButton* m_btn_ANN;

    QLabel* m_label_ChineseANN;
    QLineEdit* m_lineedit_ChineseANN;
    QPushButton* m_btn_ChineseANN;

    QLabel* m_label_GrayChANN;
    QLineEdit* m_lineedit_GrayChANN;
    QPushButton* m_btn_GrayChANN;

    QLabel* m_label_ChineseMapping;
    QLineEdit* m_lineedit_ChineseMapping;
    QPushButton* m_btn_ChineseMapping;

    //------------------图片识别设置------------------
    QLabel* m_label_ImgPR;
    QLabel* m_label_ImgPR_debug;
    QCheckBox* m_checkbox_ImgPR_debug;
    QLabel* m_label_ImgPR_label;
    QCheckBox* m_checkbox_ImgPR_label;
    QLabel* m_label_ImgPR_detecttype;
    QComboBox* m_combobox_ImgPR_detecttype;
    QLabel* m_label_ImgPR_maxplates;
    QSpinBox* m_spinbox_ImgPR_maxplates;
    QLabel* m_label_ImgPR_OutPutPath;
    QLineEdit* m_lineedit_ImgPR_OutPutPath;
    QPushButton* m_btn_ImgPR_OutPutPath;

    //------------------视频识别设置------------------
    QLabel* m_label_VideoPR;
    QLabel* m_label_VideoPR_recognize;
    QCheckBox* m_checkbox_VideoPR_recognize;
    QLabel* m_label_VideoPR_record;
    QCheckBox* m_checkbox_VideoPR_record;
    QLabel* m_label_VideoPR_debug;
    QCheckBox* m_checkbox_VideoPR_debug;
    QLabel* m_label_VideoPR_label;
    QCheckBox* m_checkbox_VideoPR_label;
    QLabel* m_label_VideoPR_detecttype;
    QComboBox* m_combobox_VideoPR_detecttype;
    QLabel* m_label_VideoPR_maxplates;
    QSpinBox* m_spinbox_VideoPR_maxplates;
    QLabel* m_label_VideoPR_OutPutPath;
    QLineEdit* m_lineedit_VideoPR_OutPutPath;
    QPushButton* m_btn_VideoPR_OutPutPath;

    //------------------QScrollArea------------------
    QWidget* m_sa_contentWidget;
    QScrollArea* m_sa_Settings;

    //------------------------------------------------
    QPushButton* m_btn_defualt;
    QPushButton* m_btn_cancel;
    QPushButton* m_btn_apply;

    QSettings* m_config;
};

