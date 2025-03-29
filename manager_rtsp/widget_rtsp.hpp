#ifndef WIDGET_RTSP_HPP
#define WIDGET_RTSP_HPP

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "widget_overlay.hpp"
#include "../manager_gimbal/interface_gimabal_siyi.hpp"

class CWidgetRTSP : public QWidget
{
    Q_OBJECT
public:
    static CWidgetRTSP* Instance();
    explicit CWidgetRTSP(QWidget *parent = nullptr);
    ~CWidgetRTSP();
    QWidget* getOverlatParentWidget(){
        return _mVideoWidgetRTSP;
    }
signals:     
    void sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,float iData0=0,float iData1=0,float iData2=0,float iData3=0);  // 云台控制指令
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;  // 事件过滤器

private:
    QWidget* initializeRTSPShow();  // 显示视频拉流
    QWidget* initializeRTSPCtrl();  // 控制视频拉流
    QWidget* initializeRobotCtrl(); // 机器人控制指令
    
    // 设置覆盖层
    void setupOverlay();

private slots:
    void handleErrorMediaPlayer(QMediaPlayer::Error error);     // 视频流报错处置
    void handleStateChangedMediaPlayer(QMediaPlayer::PlaybackState _iState);        // 视频流状态处置
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPushButtonClickedEvent();
    void onHandleRecvGimbalData_Test(QByteArray iData);

private:
    QPushButton* makeupWidgetPushBtn(QString iName, QString iObject,int iSlotType = 0);
    bool beginPlayRTSP();

private:
    static CWidgetRTSP* _mInstance;
    QVideoWidget *_mVideoWidgetRTSP = nullptr;  // RTSP视频拉流
    QMediaPlayer *_mediaPlayer = nullptr;       // 媒体播放器
    QString _currentRtspUrl = "rtsp://192.168.144.25:8554/main.264"; //"rtsp://192.168.1.18:8554/stream";//"rtsp://192.168.144.25:8554/main.264"; // RTSP地址
    // 控制面板
    QWidget *_controlPanel = nullptr;           // 控制面板主容器
    QLineEdit *_urlEdit = nullptr;             // RTSP地址输入框
    QLabel* _statusLabel = nullptr;            // 状态回报
    bool _isPlaying = false;                   // 播放状态
    // 覆盖层
    CWidgetOverlay *_overlayWidget = nullptr;  // 覆盖层widget

    
};

#endif // WIDGET_RTSP_HPP
