#ifndef INTERFACE_ROBOT_DEEP_HPP
#define INTERFACE_ROBOT_DEEP_HPP

#include <QObject>
#include <QUdpSocket>
class CInterfaceRobotDeep : public QObject
{
    Q_OBJECT
public:
    enum _ENUM_ROBOT_DEEP_CONTROL_MODE {
        ROBOT_CMD_MODE = 0,    // 机器狗控制模式
        ROBOT_CMD_AIXS        // 机器狗控制指令-遥杆量
    };
    Q_ENUM(_ENUM_ROBOT_DEEP_CONTROL_MODE)

    struct _STRUCT_ROBOT_DEEP_CONTROL_HEAD // 机器狗控制指令头
    {
        uint8_t STX_A;    // 0xEB
        uint8_t STX_B;    // 0x90// 控制指令
        uint8_t CMD_ID;   // 控制指令ID
        uint8_t CMD_LEN;  // 控制指令长度 
        _STRUCT_ROBOT_DEEP_CONTROL_HEAD() {
            STX_A = 0xEB;
            STX_B = 0x90;
            CMD_ID = 0;
            CMD_LEN = 0;
        }
    };

    struct _STRUCT_ROBOT_DEEP_CONTROL_MODE // 机器狗控制模式
    {
        _STRUCT_ROBOT_DEEP_CONTROL_HEAD _head;
        uint8_t CMD_MODE_0;     // 控制模式0
        uint8_t CMD_MODE_1;     // 控制模式1
        uint8_t CRC;       // 校验和
        uint8_t ETX;       // 结束符

        _STRUCT_ROBOT_DEEP_CONTROL_MODE() {
            CMD_MODE_0 = 0;     // 控制模式0
            CMD_MODE_1 = 0;     // 控制模式1            
            CRC = 0;        // 校验和初始值
            ETX = 0xED;     // 结束符
        }
    };


    struct _STRUCT_ROBOT_DEEP_CONTROL_AIXS // 机器狗控制指令-遥杆量
    {
        _STRUCT_ROBOT_DEEP_CONTROL_HEAD _head;
        uint16_t AIXS_0;   // 遥杆-前后平移
        uint16_t AIXS_1;   // 遥杆-左右平移
        uint16_t AIXS_2;   // 遥杆-左右转弯
        uint16_t AIXS_3;   // 遥杆
        
        uint8_t back_0;     // 保留
        uint8_t back_1;     // 保留 
        uint8_t CRC;       // 校验和
        uint8_t ETX;       // 结束符

        _STRUCT_ROBOT_DEEP_CONTROL_AIXS() {
            AIXS_0 = 0;  // 遥杆中间位置
            AIXS_1 = 0;  // 遥杆中间位置
            AIXS_2 = 0;  // 遥杆中间位置
            AIXS_3 = 0;  // 遥杆中间位置
            back_0 = 0;     // 保留字节
            back_1 = 0;     // 保留字节
            CRC = 0;        // 校验和初始值
            ETX = 0xED;     // 结束符
        }
    };


    
    static CInterfaceRobotDeep* Instance();
    explicit CInterfaceRobotDeep(QObject *parent = nullptr);

public slots:
    // 接收控制指令→将【控制量】UDP发给机器狗
    bool onRobotControlEvent(_ENUM_ROBOT_DEEP_CONTROL_MODE iTpye,
        float iData0=0,float iData1=0,float iData2=0,float iData3=0); 


private slots:
    void onHandleRecvRobotData();
private:
    void onProcessMessagesRobot(QByteArray iData);

private:
    static CInterfaceRobotDeep* _mInstance;
    QUdpSocket* _mUdpSocket_RobotDeep = nullptr;
    QString _mRobotDeepIP = "192.168.144.110";
    quint16 _mRobotDeepPortSend = 2581;
    quint16 _mRobotDeepPortRecv = 2580;
};

#endif // INTERFACE_ROBOT_DEEP_HPP
