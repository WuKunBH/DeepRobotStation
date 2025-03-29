#include "widget_gimbal.hpp"
#include <QHBoxLayout>
#include <QGridLayout>

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include "interface_gimabal_siyi.hpp"
#include <QMetaEnum>
#include <QScrollArea>
#include <QScroller>
CWidgetGimbal* CWidgetGimbal::_mInstance = nullptr;
CWidgetGimbal* CWidgetGimbal::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CWidgetGimbal();
        } 
    }    
    return _mInstance;
}


CWidgetGimbal::CWidgetGimbal(QWidget *parent)
    : QWidget{parent}
{
    initializeUiSetup();
}

// 初始化界面布局
void CWidgetGimbal::initializeUiSetup(){
    // this->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    QHBoxLayout* _mLayoutMain = new QHBoxLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(0);
    _mLayoutMain->addWidget(this->createGimbalWidgetCtrl());
    _mLayoutMain->addWidget(this->createGimbalWidgetShow());
    this->setLayout(_mLayoutMain);

}  

// 创建云台界面-控件
QWidget* CWidgetGimbal::createGimbalWidgetCtrl(){

    QGridLayout* _mLayoutMain = new QGridLayout;
    _mLayoutMain->setContentsMargins(0,0,0,0);
    _mLayoutMain->setSpacing(2);
    // ------第1行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("固件", "_mBtn_Firmware"), 0, 0,1,1);     // 请求云台相机固件版本
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("硬件", "_mBtn_Hardware"), 0, 1,1,1);     // 请求云台相机硬件ID
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("模式", "_mBtn_GimbalMode"), 0, 2,1,1);   // 请求云台相机工作模式
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("放大", "_mBtn_GimbalZoomUp"), 0, 3,1,1);   // 控制云台相机放大
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("缩小", "_mBtn_GimbalZoomDown"), 0, 4,1,1); // 控制云台相机缩小
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("停止", "_mBtn_GimbalZoomStop"), 0, 5,1,1);   // 控制云台相机停止缩放
    // ------第2行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("×", "_mBtn_GimbalPitchStop"),    1, 0,1,1);   // 控制云台相机停止俯仰
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("↑", "_mBtn_GimbalPitchUp"),      1, 1,1,1);   // 控制云台相机俯仰
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("↓", "_mBtn_GimbalPitchDown"),    1, 2,1,1);   // 控制云台相机俯仰
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("←", "_mBtn_GimbalYawLeft"),      1, 3,1,1);   // 控制云台相机偏航
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("→", "_mBtn_GimbalYawRight"),     1, 4,1,1);   // 控制云台相机偏航
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("归中", "_mBtn_GimbalCtrlZero"),  1, 5,1,1);   // 控制云台相机归中
    // ------第3行 ------------------
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("锁定", "_mBtn_GimbalSetModeLock"), 2, 0,1,1);   // 控制云台相机-运动模式-锁定
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("跟随", "_mBtn_GimbalSetModeFollow"), 2, 1,1,1);   // 控制云台相机-运动模式-跟随
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("FPV", "_mBtn_GimbalSetModeFPV"), 2, 2,1,1);   // 控制云台相机-运动模式-FPV
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("拍照", "_mBtn_GimbalSetModePhoto"), 2, 3,1,1);   // 控制云台相机-运动模式-拍照
    _mLayoutMain->addWidget(this->makeupWidgetPushBtn("录像", "_mBtn_GimbalSetModeRecord"), 2, 4,1,1);   // 控制云台相机-运动模式-录像
    









    QWidget* _mWidget = new QWidget();
    _mWidget->setLayout(_mLayoutMain);
    return _mWidget;

}

// 创建云台界面-显示
QScrollArea* CWidgetGimbal::createGimbalWidgetShow() {
    _mapLabelGimbalState.clear();
    QVBoxLayout* _mLayoutMain = new QVBoxLayout;
    // 查询枚举类型
    const QMetaObject *_metaObject = CInterfaceGimabalSIYI::Instance()->metaObject();
    int _enumIndex = _metaObject->indexOfEnumerator("_ENUM_GIMBAL_CONTROL_MODE");
    QMetaEnum _metaEnum = _metaObject->enumerator(_enumIndex);
    for (int ii = 0; ii < _metaEnum.keyCount(); ii++)  {
        const char* key = _metaEnum.key(ii);
        int value = _metaEnum.value(ii);
        //qDebug()<<"--------------------------------------------- ============= ----------"<<key<<value;
        CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE _mEnumInxde = (CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE)value;
        if(!_mapLabelGimbalState.contains(_mEnumInxde)) {
            QLabel* _mEnumLabel = new QLabel();
            _mEnumLabel->setText(key);
            // _mEnumLabel->setPlainText(key);
            // _mEnumLabel->setStyleSheet("QLabel { color: white; font-size: 14px; font-weight: bold; }");
            // _mEnumLabel->setReadOnly(true);
            // 设置不可选中
            _mEnumLabel->setTextInteractionFlags(Qt::NoTextInteraction);
            // 加入到QMap中
            _mapLabelGimbalState.insert(_mEnumInxde, _mEnumLabel);
            _mLayoutMain->addWidget(_mEnumLabel);
        }
    }

    _mLayoutMain->addStretch();
    QWidget* _mWidget_Sub  = new QWidget();
    _mWidget_Sub->setLayout(_mLayoutMain);


    QScrollArea* _scrollArea = new QScrollArea();
    _scrollArea->setAttribute(Qt::WA_AcceptTouchEvents);
    // 启用触摸滚动
    QScroller::grabGesture(_scrollArea->viewport(), QScroller::TouchGesture);
    _scrollArea->setWidgetResizable(true); // 允许内容部件调整大小
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _scrollArea->setWidget(_mWidget_Sub);


    // // 设置滚动条样式
    // QString style = R"(
    // QScrollBar:vertical {
    //     width: 30px;
    //     background: #F0F0F0;
    // }
    // QScrollBar::handle:vertical {
    //     background: #A0A0A0;
    //     min-height: 10px;
    //     border-radius: 10px;
    // }
    // QScrollBar::add-line:vertical,
    // QScrollBar::sub-line:vertical {
    //     background: none;
    // })";
    // _scrollArea->setStyleSheet(style);
    // 滚动结束后强制刷新
    QObject::connect(QScroller::scroller(_scrollArea->viewport()),
    &QScroller::stateChanged,this,
    [=](QScroller::State state) {
        if (state == QScroller::Inactive) {
            _scrollArea->viewport()->update();
        }
    });
    return _scrollArea;
}


QPushButton* CWidgetGimbal::makeupWidgetPushBtn(QString iName, QString iObject){
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
    connect(mPushBtn, &QPushButton::clicked, this, &CWidgetGimbal::onPushButtonClickedEvent_GimbalCtrl);
    return mPushBtn;
}


void CWidgetGimbal::onPushButtonClickedEvent_GimbalCtrl(){
    QPushButton* _mPBtn = qobject_cast<QPushButton*>(sender());

    if(_mPBtn->objectName() == "_mBtn_Firmware"){
        // 检查固件版
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_FIRMWARE);        
    }
    else if (_mPBtn->objectName()== "_mBtn_Hardware")  {
        // 检查云台相机硬件ID
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_HARDWARM);
    }
    else if (_mPBtn->objectName() == "_mBtn_GimbalMode")  {
        // 检查云台相机工作模式
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_MODE);
    }
    else  if(_mPBtn->objectName() == "_mBtn_GimbalZoomUp"){
        // 云台相机放大
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,1.0);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalZoomDown"){
        // 云台相机缩小
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,-1.0);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalZoomStop"){
        // 云台相机当前焦距
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,0);
    }
    // ============== 第 2 行 =============================
    else if (_mPBtn->objectName() == "_mBtn_GimbalPitchUp") {
        // 云台相机俯仰向上
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,0,20);
    }
    else if (_mPBtn->objectName() == "_mBtn_GimbalPitchDown") {
        // 云台相机俯仰向下
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,0,-20);
    }
    else if (_mPBtn->objectName() == "_mBtn_GimbalPitchStop") {
        // 云台相机当前俯仰
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,0,0);
    }    
    else if (_mPBtn->objectName() == "_mBtn_GimbalYawLeft") {
        // 云台相机偏航向左
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,-20.0,0);
    }
    else if (_mPBtn->objectName() == "_mBtn_GimbalYawRight") {
        // 云台相机偏航向右
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,20.0,0);
    }
    else if (_mPBtn->objectName() == "_mBtn_GimbalCtrlZero") {
        // 云台相机-归中
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZERO);
    }
    // ============== 第 3 行 =============================
    else if(_mPBtn->objectName() == "_mBtn_GimbalSetModeLock") {
        // 云台-锁定
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE,3);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalSetModeFollow") {
        // 云台-跟随
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE,4);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalSetModeFPV") {
        // 云台-FPV
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE,5);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalSetModePhoto") {
        // 云台-拍照
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE,0);
    }
    else if(_mPBtn->objectName() == "_mBtn_GimbalSetModeRecord") {
        // 云台-录像
        emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE,2);
    }

    
    
    


}





void CWidgetGimbal::onHandleGimabalStateResponse(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,QStringList mContext){
    // qDebug()<<"============================"<<mContext;
    QString _mString;
    switch (iTpye) {
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_FIRMWARE:
        _mString = QString("相机固件版本号：<span style='color: yellow;'>%1</span>").arg(mContext[0])
                   + QString("<br/>")
                   + QString("云台固件版本号：<span style='color: yellow;'>%1</span>").arg(mContext[1]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_FIRMWARE]->setText(_mString);
        break;
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_HARDWARM:
        _mString = QString("云台相机型号：<span style='color: yellow;'>%1</span>").arg(mContext[2])
                   + QString("<br/>")
                   + QString("云台相机ID：<span style='color: yellow;'>%1</span>").arg(mContext[1]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_HARDWARM]->setText(_mString);
        break;
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_MODE:
        _mString = QString("云台相机工作模式：<span style='color: yellow;'>%1</span>").arg(mContext[1]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_MODE]->setText(_mString);
        break;
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL:
        _mString = QString("当前焦距变焦倍数：<span style='color: yellow;'>%1</span>").arg(mContext[0]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL]->setText(_mString);
        break;
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_ABSOLUTE:
        _mString = QString("绝对焦距变焦倍数：<span style='color: yellow;'>%1</span>").arg(mContext[1]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_ABSOLUTE]->setText(_mString);
        break;
    case CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MAX:
        _mString = QString("最大焦距变焦倍数：<span style='color: yellow;'>%1</span>").arg(mContext[1]);
        _mapLabelGimbalState[CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MAX]->setText(_mString);
        break;

        
    default:
        break;
    }
    this->update();

}
