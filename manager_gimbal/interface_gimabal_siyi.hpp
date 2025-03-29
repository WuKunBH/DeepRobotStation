/***********************************/
// 思翼云台控制接口


/***********************************/

#ifndef INTERFACE_GIMABAL_SIYI_HPP
#define INTERFACE_GIMABAL_SIYI_HPP

#include <QObject>
#include <QUdpSocket>
#include "gimbal_control_siyi.hpp"

class CInterfaceGimabalSIYI : public QObject
{
    Q_OBJECT
public:
    enum _ENUM_GIMBAL_CONTROL_MODE
    {
        GIMBAL_CMD_FIRMWARE = 0,    // 云台相机固件版本号
        GIMBAL_CMD_HARDWARM,        // 云台相机硬件ID
        GIMBAL_CMD_CTRL_MODE,        // 云台相机模式
        GIMBAL_CMD_ZOOM_MANUAL,      // 手动变倍
        GIMBAL_CMD_ZOOM_ABSOLUTE,   // 绝对变倍
        GIMBAL_CMD_ZOOM_MAX,        // 最大变倍
        GIMBAL_CMD_ZOOM_NOW,        // 当前变倍
        GIMBAL_CMD_TURN,            // 云台控制指令-转向
        GIMBAL_CMD_ZERO,            // 云台归零
        GIMBAL_CMD_CONFIG,          // 云台配置
        GIMBAL_CMD_FEEDBACK,        // 云台反馈
        GIMBAL_CMD_CTRL_TURN,        // 云台转向
        GIMBAL_CMD_SET_MODE       // 云台【锁定、跟随、FPV】,相机【拍照、录像】
    };


    Q_ENUM(_ENUM_GIMBAL_CONTROL_MODE)


    static CInterfaceGimabalSIYI* Instance();
    explicit CInterfaceGimabalSIYI(QObject *parent = nullptr);
signals:
    void sigGimabalStateResponse(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,QStringList mContext);

public slots:
    bool onGimbalControlEvent(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,float iData0=0,float iData1=0,float iData2=0,float iData3=0);  // 云台控制指令

private slots:
    void onHandleRecvGimbalData();
private:
    void onProcessMessagesSiYi(QByteArray iData);
private:
    static CInterfaceGimabalSIYI* _mInstance;
    QUdpSocket* _mUdpSocke_Gimbal = nullptr;    // 控制云台-UDP

    CGimbalControlSiYi* _mGimbalProtocol = nullptr;

};

#endif // INTERFACE_GIMABAL_SIYI_HPP
