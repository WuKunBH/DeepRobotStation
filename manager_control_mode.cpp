#include "manager_control_mode.hpp"


#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

CManagerControlMode* CManagerControlMode::_mInstance = nullptr;
CManagerControlMode* CManagerControlMode::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CManagerControlMode();
        }
    }
    return _mInstance;
}



CManagerControlMode::CManagerControlMode(QObject *parent)
    : QObject{parent}
{}



void CManagerControlMode::onHandleGimabalStateResponse(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE iTpye,QStringList mContext){

}
void CManagerControlMode::onHandleRemoteStateResponse(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE iTpye,QStringList mContext){
    if (iTpye== CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO)   {
        // 接收到遥控器信息
        // 云台控制
        int iChannel_11 = mContext.at(11).toInt()-1500;
        int iChannel_12 = mContext.at(12).toInt()-1500;  
        if(_mChannel_old[11] !=iChannel_11 || _mChannel_old[12] !=iChannel_12)  {
            _mChannel_old[11] =iChannel_11;
            _mChannel_old[12] =iChannel_12;
            emit sigGimbalControlCmd(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN,iChannel_11/5.0f,iChannel_12/5.0f);
        }    
        
        // 机器狗控制
        int iChannel_00 = mContext.at(0).toInt();
        int iChannel_01 = mContext.at(1).toInt();
        int iChannel_02 = mContext.at(2).toInt();
        int iChannel_03 = mContext.at(3).toInt();
        if(_mChannel_old[0] !=iChannel_00 || _mChannel_old[1] !=iChannel_01 || _mChannel_old[2] !=iChannel_02 || _mChannel_old[3] !=iChannel_03)  {
            _mChannel_old[0] =iChannel_00;
            _mChannel_old[1] =iChannel_01;
            _mChannel_old[2] =iChannel_02;
            _mChannel_old[3] =iChannel_03;
            emit sigRobotControlCmd(CInterfaceRobotDeep::_ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_AIXS, iChannel_00,iChannel_01,iChannel_02,iChannel_03);

        }
        
       
        //if(mContext.at())
        
    }
    

}
