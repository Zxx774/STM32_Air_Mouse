#include "stm32f10x.h"
#include "Delay.h"

extern uint32_t GetTick(void); // 引用 main.c 里的获取时间函数

// 微秒级延时：使用 CPU 空循环，不干扰 SysTick
void Delay_us(uint32_t xus)
{
    // 72MHz 主频下，简单计算一个粗略的循环次数
    uint32_t count = xus * (72000000 / 1000000) / 5; 
    while(count--);
}

// 毫秒级延时：利用全局的滴答定时器
void Delay_ms(uint32_t xms)
{
    uint32_t start = GetTick();
    while((GetTick() - start) < xms);
}

void Delay_s(uint32_t xs)
{
    Delay_ms(xs * 1000);
}
