#include "interface_remote_control_siyi.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>



CInterfaceRemoteControlSiyi* CInterfaceRemoteControlSiyi::_mInstance = nullptr;
CInterfaceRemoteControlSiyi* CInterfaceRemoteControlSiyi::Instance(){
    if(!_mInstance){
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(!_mInstance){
            _mInstance = new CInterfaceRemoteControlSiyi();
        }
    }
    return _mInstance;
}


CInterfaceRemoteControlSiyi::CInterfaceRemoteControlSiyi(QObject *parent)
    : QObject{parent}
{
    _mSerialPort_Remote = new QSerialPort(this);
    _mSerialPort_Remote->setPortName("/dev/ttyHS0"); // 设置端口名称，例如 "COM1" 在Windows上，"/dev/ttyUSB0" 在Linux上
    _mSerialPort_Remote->setBaudRate(QSerialPort::Baud115200); // 设置波特率
    _mSerialPort_Remote->setDataBits(QSerialPort::Data8); // 设置数据位
    _mSerialPort_Remote->setParity(QSerialPort::NoParity); // 设置校验位
    _mSerialPort_Remote->setStopBits(QSerialPort::OneStop); // 设置停止位
    _mSerialPort_Remote->setFlowControl(QSerialPort::NoFlowControl); // 设置流控制

    connect(_mSerialPort_Remote, &QSerialPort::readyRead, this,&CInterfaceRemoteControlSiyi::onHandleRecvRemoteData);




    if (_mSerialPort_Remote->open(QIODevice::ReadWrite)) {
        qDebug() << "串口打开成功----------------------------------------------------------------";
    } else {
        qDebug() << "串口打开失败----------------------------------------------------------------";
    }


    // 通信协议解析
    _mRomoteProtocol = new CRemoteControlSiYi();

}


bool CInterfaceRemoteControlSiyi::onRemoteControlEvent(CInterfaceRemoteControlSiyi::_ENUM_REMOTE_CONTROL_MODE _iTpye,
    float iData0,float iData1,float iData2,float iData3)
{
    bool _flag = false;

    std::vector<uint8_t> siyi_packet;
    switch (_iTpye) {
    case _ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_LINK_HARDWARE:
        siyi_packet =_mRomoteProtocol->requestLinkHardwareID();
        _flag = true;
        break;
    case _ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_DATA_INFO:
        siyi_packet =_mRomoteProtocol->requestDataLinkInfo();
        _flag = true;
        break;
    case _ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_VIDEO_INFO:
        siyi_packet =_mRomoteProtocol->requestVideoLinkInfo();
        _flag = true;
        break;
    case _ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO:
        siyi_packet =_mRomoteProtocol->requestChannelData(iData0);
        _flag = true;
        break;
    
    default:
        break;
    }
    if(_flag){
        if(_mSerialPort_Remote->isOpen()){
            QByteArray byteArray = QByteArray(reinterpret_cast<const char*>(siyi_packet.data()), static_cast<int>(siyi_packet.size()));

            // byteArray.resize(10);
            // byteArray[0] = 0x55;
            // byteArray[1] = 0x66;
            // byteArray[2] = 0x01;
            // byteArray[3] = 0x00;
            // byteArray[4] = 0x00;
            // byteArray[5] = 0x00;
            // byteArray[6] = 0x00;
            // byteArray[7] = 0x40;
            // byteArray[8] = 0x81;
            // byteArray[9] = 0x9c;
             qDebug() << "Send Data: " << byteArray.toHex(' ');
            _mSerialPort_Remote->write(byteArray);
            _mSerialPort_Remote->waitForBytesWritten(); // 等待数据发送完成
            qDebug()<<"发送成功";
        }
    }
    return _flag;
}


void CInterfaceRemoteControlSiyi::onHandleRecvRemoteData(){

    QByteArray _iData = _mSerialPort_Remote->readAll(); // 读取所有可用数据
    // 消息解析
    if(_iData[0] == 0x55 && _iData[1]== 0x66){
        int _mLen = _iData[3];  // 有效消息长度
        int _mType = _iData[7]; // 消息类型
        if(_mType == 0x43){
            CRemoteControlSiYi::_MsgDataInfo _msgDataInfo;
            memcpy(&_msgDataInfo,_iData.data()+8,_mLen);
            qDebug()<<_msgDataInfo.freq<<_msgDataInfo.pack_loss_rate<<_msgDataInfo.data_up;
        }
        else if(_mType == 0x42){
            CRemoteControlSiYi::_MsgChannelData _msgChannelData;
            QStringList _mContext;
            memcpy(&_msgChannelData,_iData.data()+8,_mLen);
            for (int ii = 0; ii < 16; ++ii) {
                _mContext<<QString::number(_msgChannelData._Channel[ii]);
            }
            emit sigRemoteStateResponse(_ENUM_REMOTE_CONTROL_MODE::REMOTE_CONTROL_CHANNEL_INFO,_mContext);
        }

    }





}
