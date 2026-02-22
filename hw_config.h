#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "stm32f10x.h"

// --- 函数声明 ---
// 这些函数在 hw_config.c 中定义，供 main.c 调用
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config(FunctionalState NewState);

#endif /* __HW_CONFIG_H */
