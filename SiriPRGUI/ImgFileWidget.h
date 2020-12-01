#pragma once

#include <vector>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qstring>

#include "CPRecognize.h"

class ImgPRWidget;

class ImgFileWidget :
    public QWidget
{
    Q_OBJECT

    QString MAIN_STYLE = "./SiriPRGUI/qss/imgfilewidget/main_style.qss";

public:
    ImgFileWidget(ImgPRWidget* parent = Q_NULLPTR, QString path="");
    void showImg();
    QString toStr();

    QString getFilePath();
    QString getFileName();
    vector<siripr::CPlate>& getPlates();
    void setPlates(vector<siripr::CPlate> plates);

    bool getDebug();
    void setDebug(bool debug);
    bool getLabel();
    void setLabel(bool label);
    int getDetecttype();
    void setDetecttype(int detecttype);
    int getMaxPlates();
    void setMaxPlates(int maxplates);

    bool isChecked();
    void setCheckeState(Qt::CheckState state);

private:

    void initUI();
    void loadData(QString path);

    ImgPRWidget* m_fWindow;
    
    QCheckBox* m_checkbox;
    QPushButton* m_fileNameBtn;

    QString m_file_path;
    QString m_file_name;
    vector<siripr::CPlate> m_plates;

    bool m_debug_value;
    bool m_label_value;
    int m_detecttype_value;
    int m_maxplates_value;
};

