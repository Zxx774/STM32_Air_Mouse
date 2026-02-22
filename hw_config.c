#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_pwr.h"

void Set_USBClock(void) {
    // USBCLK = PLLCLK / 1.5
    // 假设系统时钟 72MHz，除以1.5 = 48MHz
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void Enter_LowPowerMode(void) { bDeviceState = SUSPENDED; }
void Leave_LowPowerMode(void) { if (bDeviceState == SUSPENDED) bDeviceState = ATTACHED; }

void USB_Interrupts_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USB_Cable_Config(FunctionalState NewState) {
    // 如果你有控制USB上拉电阻的IO口，在这里写代码
    // 比如：GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}
