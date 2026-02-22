#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "usb_istr.h"

// =================================================================
// 1. 变量定义 (修复 Undefined symbol wIstr)
// =================================================================
// wIstr 必须在这里定义
volatile uint16_t wIstr;

// wInterrupt_Mask 已经在 usb_init.c 中定义了，这里不需要定义
// 如果需要使用，直接用即可，因为 usb_lib.h -> usb_init.h 已经声明了 extern

// =================================================================
// 2. 端点中断回调函数表 (修复 Undefined symbol pEpInt_IN/OUT)
// =================================================================
// 官方库 usb_int.c 会调用这两个数组来处理端点中断。
// 对于简单的鼠标，我们不需要处理特定端点的传输完成中断，全部指向 NOP_Process (空操作) 即可。

void (*pEpInt_IN[7])(void) =
{
  NOP_Process, // Endpoint 1 IN
  NOP_Process, // Endpoint 2 IN
  NOP_Process, // Endpoint 3 IN
  NOP_Process, // Endpoint 4 IN
  NOP_Process, // Endpoint 5 IN
  NOP_Process, // Endpoint 6 IN
  NOP_Process  // Endpoint 7 IN
};

void (*pEpInt_OUT[7])(void) =
{
  NOP_Process, // Endpoint 1 OUT
  NOP_Process, // Endpoint 2 OUT
  NOP_Process, // Endpoint 3 OUT
  NOP_Process, // Endpoint 4 OUT
  NOP_Process, // Endpoint 5 OUT
  NOP_Process, // Endpoint 6 OUT
  NOP_Process  // Endpoint 7 OUT
};

// =================================================================
// 3. USB 中断服务分发函数
// =================================================================
void USB_Istr(void)
{
  wIstr = _GetISTR();

  // CTR (Correct Transfer) - 正确传输中断
  if (wIstr & IMR_MSK & CNTR_CTRM)
  {
    _SetISTR((uint16_t)CLR_CTR);
    CTR_LP(); // 调用低优先级 CTR 处理函数 (在 usb_int.c 中)
  }
  // RESET - 复位中断
  if (wIstr & IMR_MSK & CNTR_RESETM)
  {
    _SetISTR((uint16_t)CLR_RESET);
    Device_Property.Reset();
  }
  // SUSPEND - 挂起中断
  if (wIstr & IMR_MSK & CNTR_SUSPM)
  {
    _SetISTR((uint16_t)CLR_SUSP);
    Suspend();
  }
  // WAKEUP - 唤醒中断
  if (wIstr & IMR_MSK & CNTR_WKUPM)
  {
    _SetISTR((uint16_t)CLR_WKUP);
    Resume(RESUME_EXTERNAL);
  }
  // SOF - 帧首中断
  if (wIstr & IMR_MSK & CNTR_SOFM)
  {
    _SetISTR((uint16_t)CLR_SOF);
  }
  // ESOF - 预期外帧首中断
  if (wIstr & IMR_MSK & CNTR_ESOFM)
  {
    _SetISTR((uint16_t)CLR_ESOF);
  }
}
