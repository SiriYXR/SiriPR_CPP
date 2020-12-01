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

#include "CPRecognize.h"

class MainWindow;

enum PlayState {
    PLAY,
    PAUSE,
    STOP,
};

enum VideoType {
    VIDEOFILE,
    URL,
    CAM,
    None,
};

struct VideoPRResult {
    int frame;
    int detecttype;
    int maxplates;
    vector<siripr::CPlate> plates;

    VideoPRResult(int frame, int detecttype, int maxplates, vector<siripr::CPlate> plates) :
        frame(frame), detecttype(detecttype), maxplates(maxplates), plates(plates) {};
};

class VideoPRWidget :
    public QWidget
{
    Q_OBJECT

    QString MAIN_STYLE = "./SiriPRGUI/qss/videoprwidget/main_style.qss";
    QString BTN_STYLE = "./SiriPRGUI/qss/videoprwidget/btn_style.qss";
    QString IOS_CHECKBOX_STYLE = "./SiriPRGUI/qss/videoprwidget/ios_checkbox_style.qss";

public:
    VideoPRWidget(MainWindow* parent = Q_NULLPTR);

    //overwrite
    void hide();
    void show();
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);

private slots:
    void on_btn_Open_clicked();
    void playPlayer();
    void pausePlayer();
    void stopPlayer();
    void lastFrame();
    void nextFrame();
    void frameSlidValueChange(int value);

private:

    void initData();
    void initUI();
    void initButton();

    bool OpenVideo();
    void initPlayer();
    void updateFrame();
    QImage drawImgLabel(QImage imgIn,vector<siripr::CPlate>& plates);
    QString resultStr();

    MainWindow* m_fWindow;

    cv::VideoCapture m_cap;
    QBasicTimer* m_timer;
    PlayState m_playState;
    VideoType m_videoType;
    QString m_videoFile;
    int m_fourcc;
    cv::VideoWriter m_videoWriter;

    QString m_cap_name;
    int m_cap_frames_count;
    int m_cap_currentframe;
    int m_cap_fps;
    int m_cap_width;
    int m_cap_height;

    QList<VideoPRResult>* m_resultlist_record;

    //-------------------- topLayout------------------------

    QLabel* m_label_RunningMSG;

    //-------------------imgLabel-------------------------
    QLabel* m_openLabel;
    QPushButton* m_btn_Open;
    QComboBox* m_combobox_Open;
    QLabel* m_imgLabel;

    //--------------------videoOPlayout------------------------
    QPushButton* m_btn_Play;
    QPushButton* m_btn_Pause;
    QPushButton* m_btn_Stop;
    QPushButton* m_btn_LastFrame;
    QPushButton* m_btn_NextFrame;
    QSlider* m_sld_Frame;

    //-------------------- prOPlayout------------------------
    QCheckBox* m_cb_Recognize;
    QCheckBox* m_cb_Record;
    QLabel* m_label_RecognizeMSG;

    //-------------------- prSetlayout-----------------------
    QCheckBox* m_cb_debug;
    QCheckBox* m_cb_label;
    QLabel* m_label_DetectType;
    QComboBox* m_combobox_DetectType;
    QLabel* m_label_MaxPlates;
    QSpinBox* m_spinbox_MaxPlates;
};
