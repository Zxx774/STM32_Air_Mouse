#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "stm32f10x.h"

// 定义描述符的大小，必须与 .c 文件中的数组大小一致
#define MOUSE_SIZ_DEVICE_DESC       18
#define MOUSE_SIZ_CONFIG_DESC       34 // 配置(9) + 接口(9) + HID(9) + 端点(7)
#define MOUSE_SIZ_REPORT_DESC       52 // 报告描述符的字节数
#define MOUSE_SIZ_STRING_LANGID     4
#define MOUSE_SIZ_STRING_VENDOR     38
#define MOUSE_SIZ_STRING_PRODUCT    38
#define MOUSE_SIZ_STRING_SERIAL     26

// 声明外部数组，供 usb_prop.c 调用
extern const uint8_t Mouse_DeviceDescriptor[MOUSE_SIZ_DEVICE_DESC];
extern const uint8_t Mouse_ConfigDescriptor[MOUSE_SIZ_CONFIG_DESC];
extern const uint8_t Mouse_ReportDescriptor[MOUSE_SIZ_REPORT_DESC];
extern const uint8_t Mouse_StringLangID[MOUSE_SIZ_STRING_LANGID];
extern const uint8_t Mouse_StringVendor[MOUSE_SIZ_STRING_VENDOR];
extern const uint8_t Mouse_StringProduct[MOUSE_SIZ_STRING_PRODUCT];
extern const uint8_t Mouse_StringSerial[MOUSE_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */
