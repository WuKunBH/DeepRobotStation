
#include "widget_deeprobot.hpp"
#include <QHBoxLayout>

#include "interface_robot_deep.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
CWidgetDeepRobot* CWidgetDeepRobot::_mInstance = nullptr;
CWidgetDeepRobot* CWidgetDeepRobot::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CWidgetDeepRobot();
        } 
    }    
    return _mInstance;
}

CWidgetDeepRobot::CWidgetDeepRobot(QWidget *parent)
    : QWidget{parent}
{
    this->initializeUiSetupRobotDeep();
}


void CWidgetDeepRobot::initializeUiSetupRobotDeep()   // 初始化界面布局
{
    // this->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    QHBoxLayout* _mLayoutMain = new QHBoxLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(0);
    _mLayoutMain->addWidget(this->createWidgetCtrlDeep());
    _mLayoutMain->addWidget(this->createWidgetShowDeep());
    this->setLayout(_mLayoutMain);

}

QWidget* CWidgetDeepRobot::createWidgetCtrlDeep() // 创建机器狗控制
{
    QGridLayout* _mLayoutMain = new QGridLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(2);
    // ------第1行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("起立/趴下",  "_mBtnDeepRobot_StandUp"),  0, 4,1,1); // 机器狗起立/趴下
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("软急停",     "_mBtnDeepRobot_SoftStop"), 0, 3,1,1); // 机器狗软急停
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("回零",       "_mBtnDeepRobot_Reset"),    0, 2,1,1); // 机器狗回零
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("原地模式",   "_mBtnDeepRobot_Patrol"),   0, 1,1,1); // 机器狗原地模式
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("移动模式",   "_mBtnDeepRobot_Move"),     0, 0,1,1); // 机器狗移动模式
    // 
    // ------第2行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("平地低速步态","_mBtnDeepRobot_FlatWalkSlow"),        1, 0,1,1); // 机器狗平地低速步态
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("平地中速步态","_mBtnDeepRobot_FlatWalkMedium"),      1, 1,1,1); // 机器狗平地中速步态
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("平地高速步态","_mBtnDeepRobot_FlatWalkFast"),        1, 2,1,1); // 机器狗平地高速步态
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("正常/匍匐",   "_mBtnDeepRobot_NormalAndCrawling"),   1, 3,1,1); // 机器狗平地低速步态_mBtnDeepRobot_FlatWalkFast"),  1, 1,1,1); // 机器狗平地高速步态

    
    QWidget* _mWidget = new QWidget;
    _mWidget->setLayout(_mLayoutMain);
    return _mWidget;
}


QWidget* CWidgetDeepRobot::createWidgetShowDeep() // 创建机器狗数据
{

    QWidget* _mWidget = new QWidget;
    return _mWidget;
}


QPushButton* CWidgetDeepRobot::makeupWidgetPushBtn(QString iName, QString iObject){
    QPushButton* mPushBtn = new QPushButton;
    mPushBtn->setText(iName);
    mPushBtn->setObjectName(iObject);
    connect(mPushBtn, &QPushButton::clicked, this, &CWidgetDeepRobot::onPushButtonClickedEvent_DeepCtrl);
    return mPushBtn;
}

void CWidgetDeepRobot::onPushButtonClickedEvent_DeepCtrl(){
    QPushButton* _mPBtn = qobject_cast<QPushButton*>(sender());
    if(_mPBtn->objectName() == "_mBtnDeepRobot_StandUp"){
        // 机器狗起立/趴下
        emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE,0);
    }
    else if(_mPBtn->objectName() == "_mBtnDeepRobot_SoftStop"){
        // 机器狗软急停
        emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE,1);
    }
    else if(_mPBtn->objectName() == "_mBtnDeepRobot_Reset"){
        // 机器狗回零
        emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE,2);
    }
    else if(_mPBtn->objectName() == "_mBtnDeepRobot_Patrol"){
        // 机器狗原地模式
        emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE,3);
    }
    else if(_mPBtn->objectName() == "_mBtnDeepRobot_Move"){
        // 机器狗移动模式
        emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE,4);
    }


}
