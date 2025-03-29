#include "robot_ground_station.h"

#include <QHBoxLayout>
#include <QShowEvent>
#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QJniObject>
#include <QJniEnvironment>
#endif

#include <QVBoxLayout>

#include "manager_rtsp/widget_rtsp.hpp"
#include "manager_gimbal/widget_gimbal.hpp"
#include "manager_gimbal/interface_gimabal_siyi.hpp"
#include "manager_deep/widget_deeprobot.hpp"
#include "manager_deep/interface_robot_deep.hpp"
#include "manager_link/widget_link.hpp"
#include "manager_link/interface_remote_control_siyi.hpp"
#include "manager_control_mode.hpp"
CRobotGroundStation::CRobotGroundStation(QWidget *parent)
    : QMainWindow(parent)
    , mIsInitialized(false)
{
    // 构造函数中不进行UI初始化，等待showEvent
}

CRobotGroundStation::~CRobotGroundStation() {}

void CRobotGroundStation::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    
    if (!mIsInitialized) {
        // 使用QTimer延迟初始化，确保窗口已经完全创建
        QTimer::singleShot(100, this, [this]() {
            setupAndroidFullscreen();
            initializeUiSetup();
            mIsInitialized = true;
            createSignalConnectToSlots(); //实例间与槽函数绑定
        });
    }
}

bool CRobotGroundStation::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        // 当窗口激活时（比如从后台恢复），重新设置全屏
        setupAndroidFullscreen();
    }




    return QMainWindow::event(event);
}

void CRobotGroundStation::setupAndroidFullscreen()
{
#ifdef Q_OS_ANDROID
    // 获取Activity实例
    QJniObject activity = QJniObject::callStaticObjectMethod("org/qtproject/qt/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (activity.isValid()) {
        // 设置横屏模式
        activity.callMethod<void>("setRequestedOrientation", "(I)V", 0); // SCREEN_ORIENTATION_LANDSCAPE = 0

        // 获取Window对象
        QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
        if (window.isValid()) {
            // 设置全屏标志，隐藏状态栏
            const int FLAG_FULLSCREEN = 1024; // WindowManager.LayoutParams.FLAG_FULLSCREEN
            window.callMethod<void>("addFlags", "(I)V", FLAG_FULLSCREEN);

            // 获取DecorView
            QJniObject decorView = window.callObjectMethod("getDecorView", "()Landroid/view/View;");
            if (decorView.isValid()) {
                // 设置系统UI标志，隐藏导航栏和状态栏
                const int SYSTEM_UI_FLAG_HIDE_NAVIGATION = 0x00000002;
                const int SYSTEM_UI_FLAG_FULLSCREEN = 0x00000004;
                const int SYSTEM_UI_FLAG_IMMERSIVE_STICKY = 0x00001000;
                const int flags = SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                                SYSTEM_UI_FLAG_FULLSCREEN |
                                SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
                decorView.callMethod<void>("setSystemUiVisibility", "(I)V", flags);
            }
        }
    }
#endif
}

void CRobotGroundStation::initializeUiSetup()
{
    _mStackedCenter = new QStackedWidget(this);
    _mStackedCenter->insertWidget(0,CWidgetRTSP::Instance());
    _mStackedCenter->insertWidget(1,CWidgetGimbal::Instance());
    _mStackedCenter->insertWidget(2,CWidgetDeepRobot::Instance());
    _mStackedCenter->insertWidget(3,CWidgetLink::Instance());



    _mStackedCenter->setCurrentIndex(0);



    // 初始化机器人控制指令
    QVBoxLayout* mLayout_main = new QVBoxLayout;
    mLayout_main->setContentsMargins(0,0,0,0);
    mLayout_main->addWidget(_mStackedCenter);
    mLayout_main->addWidget(this->initializeRobotCtrl());


    QWidget* _mMainWidget = new QWidget(this);
    _mMainWidget->setLayout(mLayout_main);
    this->setCentralWidget(_mMainWidget);

    

}

QWidget* CRobotGroundStation::initializeRobotCtrl(){

    QHBoxLayout* _mLayout_Main = new QHBoxLayout;
    _mLayout_Main->setContentsMargins(2,2,2,2);
    _mLayout_Main->addWidget(makeupWidgetPushBtn("视频","_mBtnVideoStreaming"));
    _mLayout_Main->addWidget(makeupWidgetPushBtn("云台","_mBtnGimbalCtrlSiYi"));
    _mLayout_Main->addWidget(makeupWidgetPushBtn("平台","_mBtnPlatformDeepRobot"));
    _mLayout_Main->addWidget(makeupWidgetPushBtn("链路","_mBtnPlatformLinkSiYi"));
    _mLayout_Main->addStretch();

    QWidget* mWidget = new QWidget;
    // 创建半透明黑色背景
    // mWidget->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    mWidget->setLayout(_mLayout_Main);
    mWidget->setMaximumHeight(45);
    mWidget->setMinimumHeight(45);
    return mWidget;


}

QPushButton* CRobotGroundStation::makeupWidgetPushBtn(QString iName, QString iObject){
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
    connect(mPushBtn, &QPushButton::clicked, this, &CRobotGroundStation::onPushButtonClickedEvent);
    return mPushBtn;
}


void CRobotGroundStation::onPushButtonClickedEvent(){
    QPushButton* _mPBtn = qobject_cast<QPushButton*>(sender());
    if(_mPBtn->objectName() == "_mBtnVideoStreaming"){
        _mStackedCenter->setCurrentIndex(0);
    }
    else if(_mPBtn->objectName() == "_mBtnGimbalCtrlSiYi"){
        if(_mStackedCenter->count()>1){
            _mStackedCenter->setCurrentIndex(1);
        }
    }
    else if(_mPBtn->objectName() == "_mBtnPlatformDeepRobot"){
        if(_mStackedCenter->count()>2){
            _mStackedCenter->setCurrentIndex(2);
        }
    }
    else if(_mPBtn->objectName() == "_mBtnPlatformLinkSiYi"){
        if(_mStackedCenter->count()>3){
            _mStackedCenter->setCurrentIndex(3);
        }
    }

}

void CRobotGroundStation::createSignalConnectToSlots(){
    // ----云台控制 ---------
    connect(CWidgetGimbal::Instance(),&CWidgetGimbal::sigGimbalControlCmd,
        CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::onGimbalControlEvent);
    connect(CWidgetRTSP::Instance(),&CWidgetRTSP::sigGimbalControlCmd,
            CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::onGimbalControlEvent);


    // ----云台数据 ---------
    connect(CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::sigGimabalStateResponse,
            CWidgetGimbal::Instance(),&CWidgetGimbal::onHandleGimabalStateResponse);

    // ----- 链路【遥控器】管理 -------
    connect(CWidgetLink::Instance(),&CWidgetLink::sigRemoteControlCmd,
            CInterfaceRemoteControlSiyi::Instance(),&CInterfaceRemoteControlSiyi::onRemoteControlEvent);
    
    // -------- 控制模式管理 --------
    // ===== 云台控制 ======
    connect(CManagerControlMode::Instance(),&CManagerControlMode::sigGimbalControlCmd,CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::onGimbalControlEvent);
    connect(CInterfaceRemoteControlSiyi::Instance(),&CInterfaceRemoteControlSiyi::sigRemoteStateResponse,CManagerControlMode::Instance(),&CManagerControlMode::onHandleRemoteStateResponse);
    // ===== 机器人控制 ======
    connect(CManagerControlMode::Instance(),&CManagerControlMode::sigRobotControlCmd,CInterfaceRobotDeep::Instance(),&CInterfaceRobotDeep::onRobotControlEvent);


}
