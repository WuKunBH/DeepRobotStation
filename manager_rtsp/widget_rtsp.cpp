#include "widget_rtsp.hpp"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QResizeEvent>

// #include "../manager_gimbal/interface_gimabal_siyi.hpp"
#include "../manager_gimbal/widget_gimbal.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
CWidgetRTSP* CWidgetRTSP::_mInstance = nullptr;
CWidgetRTSP* CWidgetRTSP::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CWidgetRTSP();
        } 
    }
    return _mInstance;
}


CWidgetRTSP::CWidgetRTSP(QWidget *parent)
    : QWidget{parent}
{


    // connect(this,&CWidgetRTSP::sigGimbalControlEvent,
    //         CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::onGimbalControlEvent);

    // connect(CInterfaceGimabalSIYI::Instance(),&CInterfaceGimabalSIYI::sigRecvGimbalData_Test,
    //         this,&CWidgetRTSP::onHandleRecvGimbalData_Test);

    // 主界面布局
    QHBoxLayout* mLayout_RTSP = new QHBoxLayout;
    mLayout_RTSP->setContentsMargins(0,0,0,0);
    mLayout_RTSP->setSpacing(0);

    // 初始化视频显示区域
    mLayout_RTSP->addWidget(this->initializeRTSPShow());
    // 初始化控制面板
    mLayout_RTSP->addWidget(this->initializeRTSPCtrl());

    // 初始化机器人控制指令
    QVBoxLayout* mLayout_main = new QVBoxLayout;
    mLayout_main->addWidget(this->initializeRobotCtrl());
    mLayout_main->setContentsMargins(0,0,0,0);
    mLayout_main->addLayout(mLayout_RTSP);

    this->setLayout(mLayout_main);

    // setupOverlay();

    

    // 开始播放
    beginPlayRTSP();
}

CWidgetRTSP::~CWidgetRTSP(){
    if (_mediaPlayer) {
        _mediaPlayer->stop();
        delete _mediaPlayer;
    }
}

// 显示视频拉流
QWidget* CWidgetRTSP::initializeRTSPShow()
{
    // 创建视频显示窗口
    _mVideoWidgetRTSP = new QVideoWidget();
    _mVideoWidgetRTSP->setAspectRatioMode(Qt::KeepAspectRatio);
    // 创建媒体播放器
    _mediaPlayer = new QMediaPlayer(this);
    _mediaPlayer->setVideoOutput(_mVideoWidgetRTSP);

    // 连接错误处理信号
    connect(_mediaPlayer, &QMediaPlayer::errorOccurred,
            this, &CWidgetRTSP::handleErrorMediaPlayer);

    // 连接状态变化信号
    connect(_mediaPlayer, &QMediaPlayer::playbackStateChanged,
            this, &CWidgetRTSP::handleStateChangedMediaPlayer);

    connect(_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, &CWidgetRTSP::handleMediaStatusChanged);

    // 监听视频窗口大小变化
    _mVideoWidgetRTSP->installEventFilter(this);

    return _mVideoWidgetRTSP;
}

// 控制视频拉流
QWidget* CWidgetRTSP::initializeRTSPCtrl()
{
    QGroupBox* rtspGroup = new QGroupBox;
    QPushButton* startButton = makeupWidgetPushBtn("开始","startButton");
    QPushButton* pauseButton = makeupWidgetPushBtn("暂停","pauseButton");
    QPushButton* stopButton = makeupWidgetPushBtn("结束","stopButton");
    QHBoxLayout* mLayout_RTSPBtn = new QHBoxLayout;
    mLayout_RTSPBtn->setContentsMargins(2,2,2,2);
    mLayout_RTSPBtn->addWidget(startButton);
    mLayout_RTSPBtn->addWidget(pauseButton);
    mLayout_RTSPBtn->addWidget(stopButton);

    _urlEdit = new QLineEdit(_currentRtspUrl);
    // // 设置URL输入框样式
    // _urlEdit->setStyleSheet(
    //     "QLineEdit { "
    //     "  background-color: rgba(255, 255, 255, 200); "
    //     "  border-radius: 2px; "
    //     "  padding: 3px; "
    //     "}"
    //     );
    QVBoxLayout* mLayout_RTSP = new QVBoxLayout;
    mLayout_RTSP->setContentsMargins(2,2,2,2);
    mLayout_RTSP->addWidget(_urlEdit);
    mLayout_RTSP->addLayout(mLayout_RTSPBtn);
    rtspGroup->setLayout(mLayout_RTSP);

    // 状态回报
    QGroupBox* statusGroup = new QGroupBox;
    _statusLabel = new QLabel;
    // _statusLabel->setStyleSheet("QLabel { color: white; font-size: 14px; font-weight: bold; }");

    QVBoxLayout* mLayout_Status = new QVBoxLayout;
    mLayout_Status->setContentsMargins(2,2,2,2);
    mLayout_Status->addWidget(_statusLabel);
    statusGroup->setLayout(mLayout_Status);

    QVBoxLayout* mLayout_RTSPUrl = new QVBoxLayout;
    mLayout_RTSPUrl->setContentsMargins(2,2,2,2);
    mLayout_RTSPUrl->addWidget(rtspGroup);
    mLayout_RTSPUrl->addWidget(statusGroup);

    mLayout_RTSPUrl->addStretch();

    _controlPanel = new QWidget();
    // 创建半透明黑色背景
    // _controlPanel->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    _controlPanel->setLayout(mLayout_RTSPUrl);
    _controlPanel->setMinimumWidth(250);
    _controlPanel->setMaximumWidth(250);
    return _controlPanel;
}


QWidget* CWidgetRTSP::initializeRobotCtrl(){

    QHBoxLayout* _mLayout_Main = new QHBoxLayout;
    _mLayout_Main->setContentsMargins(2,2,2,2);
    _mLayout_Main->addWidget(makeupWidgetPushBtn("隐藏","hideRTSPButton"));
    // _mLayout_Main->addWidget(makeupWidgetPushBtn("←","gimbalCtrlButton_left"));
    // _mLayout_Main->addWidget(makeupWidgetPushBtn("归中","gimbalCtrlButton_Zero"));
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("放大", "_mBtn_GimbalZoomUp",1));   // 控制云台相机放大
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("缩小", "_mBtn_GimbalZoomDown",1)); // 控制云台相机缩小
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("停止", "_mBtn_GimbalZoomStop",1));   // 控制云台相机停止缩放
    _mLayout_Main->addSpacing(5);
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("×", "_mBtn_GimbalPitchStop",1));   // 控制云台相机停止俯仰
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("↑", "_mBtn_GimbalPitchUp",1));   // 控制云台相机俯仰

    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("↓", "_mBtn_GimbalPitchDown",1));   // 控制云台相机俯仰
    
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("←", "_mBtn_GimbalYawLeft",1));   // 控制云台相机偏航

    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("→", "_mBtn_GimbalYawRight",1));   // 控制云台相机偏航
    _mLayout_Main->addWidget(this->makeupWidgetPushBtn("归中", "_mBtn_GimbalCtrlZero",1));   // 控制云台相机归中


    _mLayout_Main->addStretch();








    QWidget* mWidget = new QWidget;
    // 创建半透明黑色背景
    // mWidget->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    mWidget->setLayout(_mLayout_Main);

    mWidget->setMaximumHeight(35);
    mWidget->setMinimumHeight(35);
    return mWidget;


}

void CWidgetRTSP::handleErrorMediaPlayer(QMediaPlayer::Error _iError){     // 视频流报错处置
    QString errorStr;
    switch (_iError) {
    case QMediaPlayer::NoError:
        errorStr = " ";
        break;
    case QMediaPlayer::ResourceError:
        errorStr = "无法打开媒体资源";
        break;
    case QMediaPlayer::FormatError:
        errorStr = "不支持的格式";
        break;
    case QMediaPlayer::NetworkError:
        errorStr = "网络错误";
        break;
    case QMediaPlayer::AccessDeniedError:
        errorStr = "Access denied";
        break;
    default:
        errorStr = "Unknown error";
    }
    _statusLabel->setText("Error: " + errorStr);

    // 清除当前源并停止播放
    _mediaPlayer->stop();
    _mediaPlayer->setSource(QUrl());
}

void CWidgetRTSP::handleStateChangedMediaPlayer(QMediaPlayer::PlaybackState _iState){        // 视频流状态处置
    switch (_iState) {
    case QMediaPlayer::PlayingState:
        _statusLabel->setText("播放中");
        break;
    case QMediaPlayer::PausedState:
        _statusLabel->setText("暂停");
        break;
    case QMediaPlayer::StoppedState:
        _statusLabel->setText("停止");
        break;
    }
}

void CWidgetRTSP::handleMediaStatusChanged(QMediaPlayer::MediaStatus status){
    switch (status) {
    case QMediaPlayer::LoadingMedia:
        _statusLabel->setText("正在加载媒体...");
        break;
    case QMediaPlayer::BufferingMedia:
        _statusLabel->setText("正在缓冲...");
        break;
    case QMediaPlayer::BufferedMedia:
        _statusLabel->setText("播放中");
        break;
    case QMediaPlayer::NoMedia:
        _statusLabel->setText("无媒体");
        break;
    case QMediaPlayer::InvalidMedia:
        _statusLabel->setText("无效的媒体");
        // 停止播放
        _mediaPlayer->stop();
        break;
    default:
        break;
    }
}

QPushButton* CWidgetRTSP::makeupWidgetPushBtn(QString iName, QString iObject,int iSlotType){
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
    if (iSlotType == 0)  {  
        connect(mPushBtn, &QPushButton::clicked, this, &CWidgetRTSP::onPushButtonClickedEvent);     
    }
    else if(iSlotType == 1)  {  
        connect(mPushBtn, &QPushButton::clicked,
            CWidgetGimbal::Instance(), &CWidgetGimbal::onPushButtonClickedEvent_GimbalCtrl);
    }
    
   
    return mPushBtn;
}

void CWidgetRTSP::onPushButtonClickedEvent(){
    QPushButton* _mPBtn = qobject_cast<QPushButton*>(sender());
    if(_mPBtn->objectName() == "startButton"){
        if (!_currentRtspUrl.isEmpty()) {
            _isPlaying = beginPlayRTSP();
        } else {
            _statusLabel->setText("Error: No URL set");
        }
    }
    else if(_mPBtn->objectName() == "pauseButton"){
        if (_mediaPlayer) {
            _mediaPlayer->pause();
            _isPlaying = false;
        }
    }
    else if(_mPBtn->objectName() == "stopButton"){
        if (_mediaPlayer) {
            _mediaPlayer->stop();
            _isPlaying = false;
        }
    }
    else if(_mPBtn->objectName() == "hideRTSPButton"){
        if(_controlPanel->isHidden()) {
            _controlPanel->show();
        }
        else {  // 隐藏
            _controlPanel->hide();
        }        
    }
    // else  if(_mPBtn->objectName() == "_mBtn_GimbalZoomUp"){
    //     // 云台相机放大
    //     emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,1.0);
    // }
    // else if(_mPBtn->objectName() == "_mBtn_GimbalZoomDown"){
    //     // 云台相机缩小
    //     emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,-1.0);
    // }
    // else if(_mPBtn->objectName() == "_mBtn_GimbalZoomStop"){
    //     // 云台相机当前焦距
    //     emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,0);
    // }


}

bool CWidgetRTSP::beginPlayRTSP(){
    bool m_ok = false;
    QString newUrl = _urlEdit->text().trimmed();
    if (newUrl.isEmpty() || !newUrl.startsWith("rtsp://")) {
        QMessageBox::warning(this, "Invalid URL", "Please enter a valid RTSP URL");
        return false;
    }

    _currentRtspUrl = newUrl;
    if (_mediaPlayer) {
        _mediaPlayer->stop();
        _mediaPlayer->setSource(QUrl(_currentRtspUrl));
        _mediaPlayer->play();
        m_ok = true;
    }
    return m_ok;
}



bool CWidgetRTSP::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == _mVideoWidgetRTSP && event->type() == QEvent::Resize) {
        // 当视频窗口大小改变时，调整覆盖层大小
        if (_overlayWidget) {
            _overlayWidget->resize(_mVideoWidgetRTSP->size());
        }
    }
    return QWidget::eventFilter(watched, event);
}

void CWidgetRTSP::setupOverlay(){
    if (!_mVideoWidgetRTSP) return;

    // 创建覆盖层
    _overlayWidget = new CWidgetOverlay();
    
    _overlayWidget->setParent(_mVideoWidgetRTSP);

    _overlayWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    _overlayWidget->setAttribute(Qt::WA_TranslucentBackground, true);//设置半透明
    _overlayWidget->setAttribute(Qt::WA_TransparentForMouseEvents); // 允许点击穿透

    // _overlayWidget->setGeometry(_mVideoWidgetRTSP->geometry().x(), _mVideoWidgetRTSP->geometry().y(), _mVideoWidgetRTSP->width(), _mVideoWidgetRTSP->height());
    // // _overlayWidget->setStyleSheet("border: 3px solid black;");

    // _overlayWidget->move(_mVideoWidgetRTSP->geometry().x(),_mVideoWidgetRTSP->geometry().y());
    _overlayWidget->setText("视频信息显1234");
    _overlayWidget->raise();
    _overlayWidget->show();
}

void CWidgetRTSP::onHandleRecvGimbalData_Test(QByteArray iData){
    _overlayWidget->setText(iData);

}
