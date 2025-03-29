#ifndef GIMBAL_CONTROL_SIYI_HPP
#define GIMBAL_CONTROL_SIYI_HPP

#include <stdint.h>
// #include <string.h>
#include <vector>


#define SIYI_BUFFER_SIZE	64
#define GIMBAL_CONTROL_SIYI_VERSION "1.0.0"
#define SIYI_SERVER_IP "192.168.144.25"//"127.0.0.1"//"192.168.144.25"     // ˼����̨���������ˣ�IP
#define SIYI_SERVER_PORT 37260              // ˼����̨���������ˣ��˿�


class CGimbalControlSiYi
{
public:
    explicit CGimbalControlSiYi() {
        seq_ = 0;      // 消息序号
    }
    ~CGimbalControlSiYi() {}
    // 0x01-请求云台相机固件版本号
    std::vector<uint8_t> controlGimbalFirmware() {        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x01, data, true);
        return packet;    
    }
    // 0x02-请求云台相机硬件ID
    std::vector<uint8_t> controlGimbalHardwareID() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x02, data, true);
        return packet;
    }
    // 0x19-请求云台相机当前工作模式
    std::vector<uint8_t> controlGimbalMode() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x19, data, true);
        return packet;
    }
    // 0x05-云台相机手动变倍
    std::vector<uint8_t> controlGimbalZoom(int iZoom) {
        
        std::vector<uint8_t> data(1, 0);
        data[0] = iZoom;        
        std::vector<uint8_t> packet = generatePacket(0x05, data, true);
        return packet;
    }
    // 0x0f-云台相机绝对变倍
    std::vector<uint8_t> controlGimbalAbsoluteZoom(int iZoomI,int iZoomD) {
       
        std::vector<uint8_t> data(2, 0); //Integer and decimal parts
        data[0] = iZoomI;
        data[1] = iZoomD; 
        std::vector<uint8_t> packet = generatePacket(0x0f, data, true);
        return packet;
    }
    // 0x16-请求当前状态最大变倍值
    std::vector<uint8_t> controlGimbalMaxZoom() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x16, data, true);
        return packet;
    }
    // 0x18-请求当前变倍值
    std::vector<uint8_t> controlGimbalZoomNow() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x18, data, true);
        return packet;
    }
    // 0x07-云台转向
    std::vector<uint8_t> controlGimbalTurn(int8_t iTurn_yaw,int8_t iTurn_pitch) {
        
        std::vector<uint8_t> data(2, 0);
        data[0] = iTurn_yaw;
        data[1] = iTurn_pitch;
        std::vector<uint8_t> packet = generatePacket(0x07, data, false);
        return packet;
    }
    // 0x08-云台归零
    std::vector<uint8_t> controlGimbalZero() {
        
        std::vector<uint8_t> data(1, 0);
        data[0] = 1;
        std::vector<uint8_t> packet = generatePacket(0x08, data, true);
        return packet;
    }
    // 0x0A-请求云台配置信息
    std::vector<uint8_t> controlGimbalConfig() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x0A, data, true);
        return packet;
    }
    // 0x0B-回传功能反馈信息
    std::vector<uint8_t> controlGimbalFeedback() {
        
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x0B, data, true);
        return packet;
    }
    // 0x0C-云台拍照功能
    std::vector<uint8_t> controlGimbalSetMode(int iTakePhoto) {
        
        std::vector<uint8_t> data(1, 0);
        data[0] = iTakePhoto;
        std::vector<uint8_t> packet = generatePacket(0x0C, data, true);
        return packet;
    }
    // 0x0D-请求云台姿态数据
    std::vector<uint8_t> controlGimbalAttitude() {
       
        std::vector<uint8_t> data(0, 0);
        std::vector<uint8_t> packet = generatePacket(0x0D, data, true);
        return packet;
    }
    // 0x0E-发送控制角度到云台
    std::vector<uint8_t> controlGimbalAngle(double yaw, double pitch) {
        
        std::vector<uint8_t> data = serializeAngles(yaw, pitch);
        std::vector<uint8_t> packet = generatePacket(0x0E, data, true);
        return packet;
    }
    // 0x41-单轴姿态控制
    std::vector<uint8_t> controlGimbalSingleAxis(double iTarget, uint8_t iType) {
        
        std::vector<uint8_t> data = serializeSingleAxis(iTarget, iType);
        std::vector<uint8_t> packet = generatePacket(0x41, data, true);
        return packet;
    }
    // 0x20-请求云台相机编码参数
    std::vector<uint8_t> controlGimbalEncode(int iEncode) {
        
        std::vector<uint8_t> data(1, 0);
        data[0] = iEncode;
        std::vector<uint8_t> packet = generatePacket(0x20, data, true);
        return packet;
    }
   
    // 0x80-云台相机软重启
    std::vector<uint8_t> controlGimbalRestart(int iRestartGimbal,int iRestartCamera) {
        
        std::vector<uint8_t> data(2, 0);
        data[0] = iRestartGimbal;
        data[1] = iRestartCamera;
        std::vector<uint8_t> packet = generatePacket(0x80, data, true);
        return packet;
    }
    // 

private:
    uint16_t seq_;
private:
    std::vector<uint8_t> serializeSingleAxis(double iTarget, uint8_t iType) {
        if (iType == 0)  {
            // 需要控制偏航
            if (iTarget> 135.0) {
                iTarget = 135.0;
            }
            else if (iTarget < -135.0) {
                iTarget = -135.0;
            }           
        }
        else if (iType == 1)   {
            // 需要控制俯仰
            if (iTarget > 25.0) {
                iTarget = 25.0;
            }
            else if (iTarget < -90.0) {
                iTarget = -90.0;
            }
        }
        int16_t iTarget_int = static_cast<int16_t>(iTarget * 10);
        std::vector<uint8_t> data(3, 0);
        data[0] = iTarget_int & 0xFF;
        data[1] = (iTarget_int >> 8) & 0xFF;
        data[2] = iType;
        return data;
    }



    std::vector<uint8_t> serializeAngles(double yaw, double pitch) {
        double threshold_yaw = 135;     // ZR10 / A8 mini��-135.0 ~ 135.0 ��  /*ZT6 / ZR30��-270.0 ~ 270.0 ��*/
        if (yaw > threshold_yaw) {
            yaw = 270.0;
        }
        else if (yaw < -threshold_yaw) {
            yaw = -threshold_yaw;
        }
        double threshold_pitch_down = -90;	 // ZR10 / A8 mini��-90.0 ~ 25.0 ��  /*ZT6 / ZR30��-90.0 ~ 25.0 ��*/
        double threshold_pitch_up = 25;	 // ZR10 / A8 mini��-90.0 ~ 25.0 ��  /*ZT6 / ZR30��-90.0 ~ 25.0 ��*/
        if (pitch > threshold_pitch_up) {
            pitch = threshold_pitch_up;
        }
        else if (pitch < threshold_pitch_down) {
            pitch = threshold_pitch_down;
        }
        int16_t yaw_int = static_cast<int16_t>(yaw * 10);
        int16_t pitch_int = static_cast<int16_t>(pitch * 10);
        std::vector<uint8_t> data(4, 0);
        data[0] = yaw_int & 0xFF;
        data[1] = (yaw_int >> 8) & 0xFF;
        data[2] = pitch_int & 0xFF;
        data[3] = (pitch_int >> 8) & 0xFF;
        return data;
    }
    std::vector<uint8_t> generatePacket(uint8_t cmd_id,
        const std::vector<uint8_t>& data,
        bool need_ack) {
        std::vector<uint8_t> packet;
        packet.push_back(0x55);  // STX low byte
        packet.push_back(0x66);  // STX high byte

        uint8_t ctrl = need_ack ? 0x01 : 0x00;
        packet.push_back(ctrl);

        uint16_t data_len = data.size();
        packet.push_back(data_len & 0xFF);         // Data_len low byte
        packet.push_back((data_len >> 8) & 0xFF);  // Data_len high byte

        packet.push_back(seq_ & 0xFF);         // SEQ low byte
        packet.push_back((seq_ >> 8) & 0xFF);  // SEQ high byte

        packet.push_back(cmd_id);  // CMD_ID

        packet.insert(packet.end(), data.begin(), data.end());  // DATA

        uint16_t crc = CRC16_cal(packet);
        packet.push_back(crc & 0xFF);         // CRC16 low byte
        packet.push_back((crc >> 8) & 0xFF);  // CRC16 high byte

        seq_ = (seq_+1) % 65535;

        return packet;

    }
    // CRC16У�����
    uint16_t CRC16_cal(const std::vector<uint8_t>& data) const {
        uint16_t crc = 0;
        // int len = data.size();
        for (auto d : data) {
            uint8_t temp = (crc >> 8) & 0xFF;
            crc = (crc << 8) ^ crc16_tab[d ^ temp];
        }
        return crc;
    }

    // CRC16����ʼ��
    const uint16_t crc16_tab[256] = {
    0x0,    0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108,
    0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x210,
    0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
    0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x420,  0x1401,
    0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee,
    0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x630,  0x76d7, 0x66f6,
    0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d,
    0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x840,  0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5,
    0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0xa50,  0x3a33, 0x2a12, 0xdbfd, 0xcbdc,
    0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4,
    0x5cc5, 0x2c22, 0x3c03, 0xc60,  0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
    0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13,
    0x2e32, 0x1e51, 0xe70,  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a,
    0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e,
    0xe16f, 0x1080, 0xa1,   0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x2b1,
    0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb,
    0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
    0x481,  0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
    0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x691,  0x16b0, 0x6657,
    0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9,
    0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x8e1,  0x3882,
    0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0xaf1,  0x1ad0, 0x2ab3, 0x3a92, 0xfd2e,
    0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07,
    0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0xcc1,  0xef1f, 0xff3e, 0xcf5d,
    0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
    0x2e93, 0x3eb2, 0xed1,  0x1ef0 };
};





#endif // !GIMBAL_CONTROL_SIYI_HPP
