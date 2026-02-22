#ifndef __USB_PROP_H
#define __USB_PROP_H

#include "stm32f10x.h"
#include "usb_core.h" 

// --- HID 相关宏定义 ---
#define GET_REPORT          0x01
#define GET_IDLE            0x02
#define GET_PROTOCOL        0x03

#define SET_REPORT          0x09
#define SET_IDLE            0x0A
#define SET_PROTOCOL        0x0B

#define HID_DESCRIPTOR_TYPE 0x21
#define REPORT_DESCRIPTOR   0x22

// --- 核心函数声明 ---
void Mouse_Init(void);
void Mouse_Reset(void);
void Mouse_Status_In(void);
void Mouse_Status_Out(void);

RESULT Mouse_Data_Setup(uint8_t RequestNo);
RESULT Mouse_NoData_Setup(uint8_t RequestNo);
RESULT Mouse_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);

uint8_t *Mouse_GetDeviceDescriptor(uint16_t Length);
uint8_t *Mouse_GetConfigDescriptor(uint16_t Length);
uint8_t *Mouse_GetStringDescriptor(uint16_t Length);
uint8_t *Mouse_GetReportDescriptor(uint16_t Length);
uint8_t *Mouse_GetHIDDescriptor(uint16_t Length);
uint8_t *Mouse_GetProtocolValue(uint16_t Length);

RESULT Mouse_SetProtocol(void);

// --- 标准请求回调函数 (全部修正为 void 返回类型) ---
void Mouse_GetConfiguration(void);
void Mouse_SetConfiguration(void);
void Mouse_GetInterface(void);
void Mouse_SetInterface(void);
void Mouse_GetStatus(void);
void Mouse_ClearFeature(void);
void Mouse_SetEndPointFeature(void);
void Mouse_SetDeviceFeature(void);
void Mouse_SetDeviceAddress(void);

#endif /* __USB_PROP_H */
