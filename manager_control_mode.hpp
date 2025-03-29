#ifndef MANAGER_CONTROL_MODE_HPP
#define MANAGER_CONTROL_MODE_HPP

#include <QObject>

#include "manager_gimbal/interface_gimabal_siyi.hpp"
#include "manager_link/interface_remote_control_siyi.hpp"
#include "manager_deep/interface_robot_deep.hpp"

class CManagerControlMode : public QObject
{
    Q_OBJECT
public:
    static CManagerControlMode* Instance();
    explicit CManagerControlMode(QObject *parent = nullptr);
signals:
    void sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,float iData0=0,float iData1=0,float iData2=0,float iData3=0);  // 云台控制指令
    void sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE iTpye,
        float iData = 0, float iData1 = 0, float iData2 = 0, float iData3 = 0); // 机器人控制指令

public slots:    
    void onHandleGimabalStateResponse(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,QStringList mContext);
    void onHandleRemoteStateResponse(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE iTpye,QStringList mContext);
private:
    static CManagerControlMode* _mInstance;
    int _mRobotControlMode = 0; // 机器人控制模式
    int _mChannel_old[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // 通道值


};

#endif // MANAGER_CONTROL_MODE_HPP
