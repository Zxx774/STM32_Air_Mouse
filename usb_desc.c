#include "usb_desc.h"

// 1. 设备描述符
const uint8_t Mouse_DeviceDescriptor[MOUSE_SIZ_DEVICE_DESC] = {
    0x12,                       // bLength
    0x01,                       // bDescriptorType (Device)
    0x00, 0x02,                 // bcdUSB = 2.00
    0x00,                       // bDeviceClass
    0x00,                       // bDeviceSubClass
    0x00,                       // bDeviceProtocol
    0x40,                       // bMaxPacketSize0 (64)
    0x83, 0x04,                 // idVendor = 0x0483 (ST)
    0x10, 0x57,                 // idProduct = 0x5710 (Mouse)
    0x00, 0x02,                 // bcdDevice = 2.00
    1,                          // Index of String Vendor
    2,                          // Index of String Product
    3,                          // Index of String Serial
    0x01                        // bNumConfigurations
};

// 2. 配置描述符 (包含接口、HID、端点)
const uint8_t Mouse_ConfigDescriptor[MOUSE_SIZ_CONFIG_DESC] = {
    // --- Configuration Descriptor ---
    0x09, 0x02, 0x22, 0x00, 0x01, 0x01, 0x00, 0xA0, 0x32,
    
    // --- Interface Descriptor ---
    0x09, 0x04, 0x00, 0x00, 0x01, 0x03, 0x01, 0x02, 0x00, 
    // Class=HID(0x03), SubClass=Boot(0x01), Protocol=Mouse(0x02)

    // --- HID Descriptor ---
    0x09, 0x21, 0x10, 0x01, 0x00, 0x01, 0x22, MOUSE_SIZ_REPORT_DESC, 0x00,
    // 注意：倒数第2字节必须是 ReportDescriptor 的长度

    // --- Endpoint Descriptor ---
    0x07, 0x05, 0x81, 0x03, 0x04, 0x00, 0x0A
    // EP1 IN, Interrupt, Size=4 bytes, Interval=10ms
};

// 3. 报告描述符 (最关键：定义数据格式 [按键, X, Y, 滚轮])
const uint8_t Mouse_ReportDescriptor[MOUSE_SIZ_REPORT_DESC] = {
    0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01,
    0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03,
    0x15, 0x00, 0x25, 0x01, 0x95, 0x03, 0x75, 0x01,
    0x81, 0x02, 0x95, 0x01, 0x75, 0x05, 0x81, 0x01,
    0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x38,
    0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x03,
    0x81, 0x06, 0xC0, 0xC0
};

// 4. 字符串描述符
const uint8_t Mouse_StringLangID[MOUSE_SIZ_STRING_LANGID] = {
    MOUSE_SIZ_STRING_LANGID, 0x03, 0x09, 0x04
};
const uint8_t Mouse_StringVendor[MOUSE_SIZ_STRING_VENDOR] = {
    MOUSE_SIZ_STRING_VENDOR, 0x03,
    'S',0,'T',0,'M',0,'i',0,'c',0,'r',0,'o',0,'e',0,'l',0,'e',0,'c',0,'t',0,'r',0,'o',0,'n',0,'i',0,'c',0,'s',0
};
const uint8_t Mouse_StringProduct[MOUSE_SIZ_STRING_PRODUCT] = {
    MOUSE_SIZ_STRING_PRODUCT, 0x03,
    'S',0,'T',0,'M',0,'3',0,'2',0,' ',0,'M',0,'o',0,'u',0,'s',0,'e',0,' ',0,'H',0,'I',0,'D',0
};
const uint8_t Mouse_StringSerial[MOUSE_SIZ_STRING_SERIAL] = {
    MOUSE_SIZ_STRING_SERIAL, 0x03,
    '1',0,'.',0,'0',0,'.',0,'0',0
};
