#ifndef REMOTE_CONTROL_SIYI_HPP
#define REMOTE_CONTROL_SIYI_HPP

#include <stdint.h>
#include <vector>

class CRemoteControlSiYi
{
public:
    explicit CRemoteControlSiYi() {
        seq_ = 0;      // 消息序号
    }
    ~CRemoteControlSiYi() {}
    // 0x16 请求系统设置
    std::vector<uint8_t> requestSystemSetting() {
        std::vector<uint8_t> data(0, 0);
        return generatePacket(0x16, data, true);
    }
    // 0x42 请求通道数据
    std::vector<uint8_t> requestChannelData(int iType) {
        std::vector<uint8_t> data(1, 0);
        data[0] = iType;    // 20Hz
        return generatePacket(0x42, data, true);
    }
    // 0x40 请求链路硬件ID
    std::vector<uint8_t> requestLinkHardwareID() {
        std::vector<uint8_t> data(0, 0);
        return generatePacket(0x40, data, true);
    }



    // 0x43 请求数传链路信息
    std::vector<uint8_t> requestDataLinkInfo() {
        std::vector<uint8_t> data(0, 0);
        return generatePacket(0x43, data, true);
    }   
    
    // 0x44 请求图传链路信息
    std::vector<uint8_t> requestVideoLinkInfo() {
        std::vector<uint8_t> data(0, 0);
        return generatePacket(0x44, data, true);
    }
    // 0x47 请求固件版本号
    std::vector<uint8_t> requestFirmwareVersion() {
        std::vector<uint8_t> data(0, 0);
        return generatePacket(0x47, data, true);
    }

    // ACK结构体
    struct _MsgDataInfo{
        uint16_t freq;      // 频率
        uint8_t pack_loss_rate; // 丢包率
        uint16_t real_pack; // 有效包
        uint16_t real_pack_rate; // 有效包率
        uint32_t data_up; // 数据上行// byte/s
        uint32_t data_down; // 数据下行// byte/s
    };
    struct _MsgChannelData{
        int16_t _Channel[16];
    };





private:
    uint16_t seq_; // 消息序号
private:

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




#endif // REMOTE_CONTROL_SIYI_HPP
