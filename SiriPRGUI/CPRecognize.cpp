#include "CPRecognize.h"

#include <qdebug.h>

CPRecognize::CPRecognize()
{
    this->m_pr = new siripr::CPlateRecognize();
    this->m_pr->setLifemode(true);
    this->SetDetectType(siripr::PR_DETECT_SOBEL);
    this->SetMaxPlates(1);
    this->SetDebug(0);
    this->LoadModel();
}

void CPRecognize::LoadModel()
{
    QSettings config("./SiriPRGUI/config/siripr.ini", QSettings::IniFormat);
    this->m_pr->LoadANN(config.value("Model/ANN").toString().toStdString());
    this->m_pr->LoadSVM(config.value("Model/SVM").toString().toStdString());
    this->m_pr->LoadChineseANN(config.value("Model/ChineseANN").toString().toStdString());
    this->m_pr->LoadGrayChANN(config.value("Model/GrayChANN").toString().toStdString());
    this->m_pr->LoadChineseMapping(config.value("Model/ChineseMapping").toString().toStdString());
}

vector<siripr::CPlate> CPRecognize::PlateRecognize(cv::Mat img)
{
    vector<siripr::CPlate> plateVec;

    this->m_pr->plateRecognize(img, plateVec);

    return plateVec;
}

void CPRecognize::SetDetectType(int type)
{
    switch (type)
    {
    case 0:
        this->m_pr->setDetectType(siripr::LocateType::SOBEL);
        break;
    case 1:
        this->m_pr->setDetectType(siripr::LocateType::COLOR);
        break;
    case 2:
        this->m_pr->setDetectType(siripr::LocateType::CMSER);
        break;
    case 3:
        this->m_pr->setDetectType(siripr::LocateType::SOBEL | siripr::LocateType::COLOR);
        break;
    case 4:
        this->m_pr->setDetectType(siripr::LocateType::SOBEL | siripr::LocateType::CMSER);
        break;
    case 5:
        this->m_pr->setDetectType(siripr::LocateType::COLOR | siripr::LocateType::CMSER);
        break;
    case 6:
    default:
        this->m_pr->setDetectType(siripr::LocateType::SOBEL | siripr::LocateType::COLOR | siripr::LocateType::CMSER);
        break;
    }
}

void CPRecognize::SetMaxPlates(int param)
{
    if (param <= 0) {
        this->m_pr->setMaxPlates(1);
    }
    else {
        this->m_pr->setMaxPlates(param);
    }
}

void CPRecognize::SetDebug(int debug)
{
    this->m_pr->setDebug(debug != 0);
}
