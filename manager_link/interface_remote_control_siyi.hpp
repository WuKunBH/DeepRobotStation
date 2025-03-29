#ifndef INTERFACE_REMOTE_CONTROL_SIYI_HPP
#define INTERFACE_REMOTE_CONTROL_SIYI_HPP

#include <QObject>
#include <QSerialPort>

#include "remote_control_siyi.hpp"
class CInterfaceRemoteControlSiyi : public QObject
{
    Q_OBJECT
public:
    enum _ENUM_REMOTE_CONTROL_MODE{
        REMOTE_CONTROL_LINK_HARDWARE = 0,     // 硬件链路设置
        REMOTE_CONTROL_SYS_SETTINGS,   // 系统参数设置
        REMOTE_CONTROL_CHANNEL_DATA,      // 通道数据设置
        REMOTE_CONTROL_DATA_INFO,         // 数据链路设置
        REMOTE_CONTROL_VIDEO_INFO,        // 图传链路设置
        REMOTE_CONTROL_CHANNEL_INFO
    };
    Q_ENUM(_ENUM_REMOTE_CONTROL_MODE)
    static CInterfaceRemoteControlSiyi* Instance();
    explicit CInterfaceRemoteControlSiyi(QObject *parent = nullptr);
signals:
    void sigRemoteStateResponse(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE iTpye,QStringList mContext);
public slots:
    bool onRemoteControlEvent(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE iTpye,
        float iData0=0,float iData1=0,float iData2=0,float iData3=0);
private slots:
    void onHandleRecvRemoteData();
private:
    static CInterfaceRemoteControlSiyi* _mInstance;
    CRemoteControlSiYi* _mRomoteProtocol = nullptr;

    QSerialPort* _mSerialPort_Remote = nullptr;
    
};

#endif // INTERFACE_REMOTE_CONTROL_SIYI_HPP
