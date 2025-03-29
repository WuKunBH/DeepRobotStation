#include "widget_link.hpp"
#include <QHBoxLayout>

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

CWidgetLink* CWidgetLink::_mInstance = nullptr;
CWidgetLink* CWidgetLink::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CWidgetLink();
        }
    }
    return _mInstance;
}


CWidgetLink::CWidgetLink(QWidget *parent)
    : QWidget{parent}
{
    this->initializeUiSetupLink();

}


void CWidgetLink::initializeUiSetupLink()   // 初始化界面布局-链路
{
    // this->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    QHBoxLayout* _mLayoutMain = new QHBoxLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(0);
    _mLayoutMain->addWidget(this->createWidgetCtrlLink());
    _mLayoutMain->addWidget(this->createWidgetShowLink());
    this->setLayout(_mLayoutMain);

}
QWidget* CWidgetLink::createWidgetCtrlLink() // 创建链路界面-控件
{
    QGridLayout* _mLayoutMain = new QGridLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(2);
    // ------第1行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("硬件", "_mBtnLink_Hardware"), 0, 0,1,1);     // 链路硬件ID
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("数传", "_mBtnLink_DataInfo"), 0, 1,1,1); // 链路数传信息
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("图传", "_mBtnLink_VideoInfo"), 0, 2,1,1); // 链路图传信息
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("信道(开)", "_mBtnLink_ChannelOpen"), 0, 3,1,1); // 链路信道信息-开
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("信道(关)", "_mBtnLink_ChannelClose"), 0,4,1,1); // 链路信道信息-关


    QWidget* _mWidget = new QWidget;
    _mWidget->setLayout(_mLayoutMain);
    return _mWidget;

}

QWidget* CWidgetLink::createWidgetShowLink() // 创建链路界面-显示
{
    QWidget* _mWidget = new QWidget;
    return _mWidget;

}

QPushButton* CWidgetLink::makeupWidgetPushBtn(QString iName, QString iObject){
    QPushButton* mPushBtn = new QPushButton;
    mPushBtn->setText(iName);
    mPushBtn->setObjectName(iObject);

    // // 设置按钮样式
    // QString buttonStyle = "QPushButton { "
    //                       "  background-color: rgba(255, 255, 255, 200); "
    //                       "  border-radius: 4px; "
    //                       "  padding: 5px; "
    //                       "  min-width: 30px; "
    //                       "} "
    //                       "QPushButton:hover { "
    //                       "  background-color: rgba(200, 200, 200, 255); "
    //                       "}";

    // mPushBtn->setStyleSheet(buttonStyle);
    connect(mPushBtn, &QPushButton::clicked, this, &CWidgetLink::onPushButtonClickedEvent_LinkCtrl);
    return mPushBtn;
}


void CWidgetLink::CWidgetLink::onPushButtonClickedEvent_LinkCtrl(){
    QPushButton* _mPBtn = qobject_cast<QPushButton*>(sender());
    if(_mPBtn->objectName() == "_mBtnLink_Hardware"){
        // 检查链路硬件ID
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_LINK_HARDWARE);
    }
    else if (_mPBtn->objectName()== "_mBtnLink_DataInfo")  {
        // 获取数传链路信息
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_DATA_INFO);
    }
    else if(_mPBtn->objectName()== "_mBtnLink_VideoInfo")  {
        // 获取图传链路信息
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_VIDEO_INFO);
    }
    else if(_mPBtn->objectName()== "_mBtnLink_ChannelOpen")  {
        // 获取信道链路信息
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO,5);
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO,5);
        emit sigRemoteControlCmd(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO,5);


        
    }

}
