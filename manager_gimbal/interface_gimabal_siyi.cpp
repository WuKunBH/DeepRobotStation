#include "interface_gimabal_siyi.hpp"
#include "gimbal_control_siyi.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>



CInterfaceGimabalSIYI* CInterfaceGimabalSIYI::_mInstance = nullptr;
CInterfaceGimabalSIYI* CInterfaceGimabalSIYI::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CInterfaceGimabalSIYI();
        }
    }
    return _mInstance;
}


CInterfaceGimabalSIYI::CInterfaceGimabalSIYI(QObject *parent)
    : QObject{parent}
{
    _mUdpSocke_Gimbal = new QUdpSocket(this);   // UDP
    bool bindResult = _mUdpSocke_Gimbal->bind(QHostAddress::Any, 37260);
    if (!bindResult) {
        qDebug() << "UDP绑定失败----------------------------------------------------------------";
    }
    else {
        qDebug() << "UDP绑定成功--------------------------------------------------------------";
    }
    QObject::connect(_mUdpSocke_Gimbal, &QUdpSocket::readyRead,this, &CInterfaceGimabalSIYI::onHandleRecvGimbalData);

    // 通信协议解析
    _mGimbalProtocol = new CGimbalControlSiYi();


}

void CInterfaceGimabalSIYI::onHandleRecvGimbalData(){
    while (_mUdpSocke_Gimbal->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_mUdpSocke_Gimbal->pendingDatagramSize());
        QHostAddress senderAddr;
        quint16 senderPort;

        // 读取数据包（含发送方信息）
        qint64 readSize = _mUdpSocke_Gimbal->readDatagram(datagram.data(), datagram.size(), &senderAddr, &senderPort);

        if (readSize == -1) {
            qDebug() << "Error reading datagram-------------------:" << _mUdpSocke_Gimbal->errorString();
            continue;
        }
        onProcessMessagesSiYi(datagram);

        // // 处理接收到的数据（示例打印）
        qDebug() << "Received-----------------------------------------------------" << readSize << "bytes from"
                 << senderAddr.toString() << ":" << senderPort;
        qDebug() << "Data:------------------------------------------------------" << datagram.toHex(' ');

    }

}


void CInterfaceGimabalSIYI::onProcessMessagesSiYi(QByteArray _iData){
    // 消息解析
    if(_iData[0] == 0x55 && _iData[1]== 0x66){
        int _mLen = _iData[3];  // 有效消息长度
        int _mType = _iData[7]; // 消息类型
        // qDebug()<<_m_len;
        if(_mType == 0x01){
            uint32_t _firmware[2];
            memcpy(_firmware,_iData.data()+8,_mLen);
            QStringList _mContext={QString::number(_firmware[0], 16).toUpper(),QString::number(_firmware[1], 16).toUpper()};
            // qDebug()<<_mContext;
            emit sigGimabalStateResponse(_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_FIRMWARE,_mContext);
        }
        else if(_mType == 0x02){
            uint8_t _hardware[12];
            // 根据编码转换（假设是UTF-8）
            QStringList _mContext = {QString::fromUtf8(_iData.mid(8, 2)),QString::fromUtf8(_iData.mid(8, _mLen))};
            qDebug()<<_iData.mid(8, 2);
            if(_iData.mid(8, 2) == "73"){ _mContext<<"A8 mini"; }
            emit sigGimabalStateResponse(_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_HARDWARM,_mContext);
        }
        else if(_mType == 0x19){
            // 获取云台相机当前工作模式
            QStringList _mContext = {QString::fromUtf8(_iData.mid(8, _mLen))};
            uint8_t _gimbal_mode;
            memcpy(&_gimbal_mode,_iData.mid(8, _mLen),sizeof(uint8_t));
            if(_gimbal_mode == 0x00){ _mContext<<"锁定模式"; }
            else if(_gimbal_mode == 0x01){ _mContext<<"跟随模式"; }
            else if(_gimbal_mode == 0x02){_mContext<<"FPV模式"; }
            emit sigGimabalStateResponse(_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_MODE,_mContext);
        }
        else if(_mType == 0x05){
            // 获取云台相机当前焦距
            // qDebug()<<_iData.toHex(' ');
            uint16_t zoom_multiple;
            memcpy(&zoom_multiple,_iData.mid(8, _mLen),sizeof(uint16_t));
            qDebug()<<zoom_multiple;
            QStringList _mContext = {QString::number(zoom_multiple/10.0, 'f',1)};
            emit sigGimabalStateResponse(_ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL,_mContext);
        }


    }



}



bool CInterfaceGimabalSIYI::onGimbalControlEvent(CInterfaceGimabalSIYI::_ENUM_GIMBAL_CONTROL_MODE _iTpye,
    float _iData0,float _iData1,
    float _iData2,float _iData3)
{
    bool _flag = false;
    std::vector<uint8_t> siyi_packet;
    switch (_iTpye)
    {
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_FIRMWARE:
        // 请求云台相机固件版本号
        siyi_packet = _mGimbalProtocol->controlGimbalFirmware();
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_HARDWARM:
        // 请求云台相机硬件ID
        siyi_packet = _mGimbalProtocol->controlGimbalHardwareID();
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_MODE:
        // 请求云台相机工作模式
        siyi_packet = _mGimbalProtocol->controlGimbalMode();
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MANUAL:
        // 请求云台相机焦距
        siyi_packet = _mGimbalProtocol->controlGimbalZoom(_iData0);
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_ABSOLUTE:
        // 请求云台相机绝对焦距
        siyi_packet = _mGimbalProtocol->controlGimbalAbsoluteZoom(_iData0,_iData1);
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_MAX:
        // 请求云台相机最大焦距
        siyi_packet = _mGimbalProtocol->controlGimbalMaxZoom();
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZOOM_NOW:
        // 请求云台相机当前焦距
        siyi_packet = _mGimbalProtocol->controlGimbalZoomNow();
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_CTRL_TURN:
        // 请求云台相机偏航向左/右
        siyi_packet = _mGimbalProtocol->controlGimbalTurn(_iData0,_iData1);
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_SET_MODE:
        // 请求云台相机拍照
        siyi_packet = _mGimbalProtocol->controlGimbalSetMode(_iData0);
        _flag = true;
        break;
    case _ENUM_GIMBAL_CONTROL_MODE::GIMBAL_CMD_ZERO:
        // 请求云台相机-归中
        siyi_packet = _mGimbalProtocol->controlGimbalZero();
        _flag = true;
        break;

    default:
        break;
    }
    // ------------------ 发送消息 ==========================
    if(_flag){
        // 将vector转换为QByteArray，因为QUdpSocket使用QByteArray发送数据
        QByteArray byteArray = QByteArray(reinterpret_cast<const char*>(siyi_packet.data()), static_cast<int>(siyi_packet.size()));
        _mUdpSocke_Gimbal->writeDatagram(byteArray, siyi_packet.size(),QHostAddress(SIYI_SERVER_IP), SIYI_SERVER_PORT);
        // qDebug()<<"发送成功";

    }
    return _flag;





//     switch (_iTpye){
//     case 0:
//         siyi_packet = CGimbalControlSiYi::controlGimbalZero();
//         _flag = true;
//         break;
//     case 1:
// //     std::vector<uint8_t> siyi_packet = CGimbalControlSiYi::controlGimbalAngle(20, -10);
// //     // 将vector转换为QByteArray，因为QUdpSocket使用QByteArray发送数据
// //     QByteArray byteArray = QByteArray(reinterpret_cast<const char*>(siyi_packet.data()), static_cast<int>(siyi_packet.size()));
// //     char buffer[] = { 0x55,0x66, 0x01, 0x04, 0x00, 0x00,0x00, 0x0e,0x00, 0x00,0xff, 0xa6,0x3b, 0x11 };


// //     /*m_SocketUdpSend->writeDatagram(byteArray, siyi_packet.size(),
// // QHostAddress(SIYI_SERVER_IP), SIYI_SERVER_PORT);*/
// //     _mUdpSocke_GimbalSend->writeDatagram(buffer, siyi_packet.size(),
// //                                    QHostAddress(SIYI_SERVER_IP), SIYI_SERVER_PORT);
//         break;
//     default:
//         break;
//     }


}
