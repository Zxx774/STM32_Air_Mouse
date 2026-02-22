#ifndef __USB_PWR_H
#define __USB_PWR_H

#include "stm32f10x.h"

// --- 定义电源状态枚举 ---
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

// --- 导出全局变量 ---
// 这个变量非常关键，main函数可以通过判断它是否等于 CONFIGURED 来决定是否发送数据
extern __IO uint32_t bDeviceState; 
extern __IO bool fSuspendEnabled;

// --- 导出函数声明 ---
void Suspend(void);
void Resume_Init(void);
void Resume(RESUME_STATE eResumeSetVal);
RESULT PowerOn(void);
RESULT PowerOff(void);

#endif /* __USB_PWR_H */
