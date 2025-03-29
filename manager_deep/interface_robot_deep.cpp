#include "interface_robot_deep.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>



CInterfaceRobotDeep* CInterfaceRobotDeep::_mInstance = nullptr;
CInterfaceRobotDeep* CInterfaceRobotDeep::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CInterfaceRobotDeep();
        }
    }
    return _mInstance;
}


CInterfaceRobotDeep::CInterfaceRobotDeep(QObject *parent)
    : QObject{parent}
{
    _mUdpSocket_RobotDeep = new QUdpSocket();
    // 监听任务机下发的机器狗状态信息
    bool bindResult = _mUdpSocket_RobotDeep->bind(QHostAddress::Any, _mRobotDeepPortRecv);
    if (!bindResult) {
        qDebug() << "UDP绑定失败----------------------------------------------------------------";
    }
    else {
        qDebug() << "UDP绑定成功--------------------------------------------------------------";
    }
    QObject::connect(_mUdpSocket_RobotDeep, &QUdpSocket::readyRead,this, &CInterfaceRobotDeep::onHandleRecvRobotData);

    
}

void CInterfaceRobotDeep::onHandleRecvRobotData(){
    while (_mUdpSocket_RobotDeep->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_mUdpSocket_RobotDeep->pendingDatagramSize());
        QHostAddress senderAddr;
        quint16 senderPort;

        // 读取数据包（含发送方信息）
        qint64 readSize = _mUdpSocket_RobotDeep->readDatagram(datagram.data(), datagram.size(), &senderAddr, &senderPort);

        if (readSize == -1) {
            qDebug() << "Error reading datagram-------------------:" << _mUdpSocket_RobotDeep->errorString();
            continue;
        }
        onProcessMessagesRobot(datagram);

        // // 处理接收到的数据（示例打印）
        qDebug() << "Received-----------------------------------------------------" << readSize << "bytes from"
                 << senderAddr.toString() << ":" << senderPort;
        qDebug() << "Data:------------------------------------------------------" << datagram.toHex(' ');

    }

}

void CInterfaceRobotDeep::onProcessMessagesRobot(QByteArray _iData){
    // 解析机器狗上传的状态信息
}



bool CInterfaceRobotDeep::onRobotControlEvent(_ENUM_ROBOT_DEEP_CONTROL_MODE _iTpye,float _iData0,float _iData1,float _iData2,float _iData3){
    bool _mResult = false;
    if (_iTpye == _ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_AIXS) {
        // 轴指令
        _STRUCT_ROBOT_DEEP_CONTROL_AIXS _mCmdAixs;
        // qDebug()<<"================================== 机器狗控制 ==================";
        _mCmdAixs._head.CMD_ID = 0x01; // 周指令
        _mCmdAixs._head.CMD_LEN = sizeof(_STRUCT_ROBOT_DEEP_CONTROL_AIXS)-4-4; // 数据长度
        _mCmdAixs.AIXS_0 = _iData0;
        _mCmdAixs.AIXS_1 = _iData1;
        _mCmdAixs.AIXS_2 = _iData2;
        _mCmdAixs.AIXS_3 = _iData3;
        _mCmdAixs.back_0 = 0x00;
        _mCmdAixs.back_1 = 0x00;
        _mCmdAixs.CRC = 0x00;
        QByteArray _mCmdAixsData;
        _mCmdAixsData.append((char*)&_mCmdAixs, sizeof(_mCmdAixs));
        _mResult = _mUdpSocket_RobotDeep->writeDatagram(_mCmdAixsData, QHostAddress(_mRobotDeepIP), _mRobotDeepPortSend);

    }

    if(_iTpye == _ENUM_ROBOT_DEEP_CONTROL_MODE::ROBOT_CMD_MODE){
        // 模式指令
        _STRUCT_ROBOT_DEEP_CONTROL_MODE _mCmdMode;
        _mCmdMode._head.CMD_ID = 0x02; // 周指令
        _mCmdMode._head.CMD_LEN = 2; // 数据长度
        _mCmdMode.CMD_MODE_0 = _iData0;
        _mCmdMode.CMD_MODE_1 = _iData1;
        _mCmdMode.CRC = 0x00;
        QByteArray _mCmdModeData;
        _mCmdModeData.append((char*)&_mCmdMode, sizeof(_mCmdMode));
        _mResult =_mUdpSocket_RobotDeep->writeDatagram(_mCmdModeData, QHostAddress(_mRobotDeepIP), _mRobotDeepPortSend);

    }
    return _mResult;
}
