#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Kalman.h"
#include "MPU6050.h"
#include "math.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
volatile uint32_t sysTick_ms;
kalman_t kalman_pitch;
kalman_t kalman_roll;
int16_t AX,AY,AZ,GX,GY,GZ;
float Roll,Pitch;
uint32_t lasttime=0;
void SysTick_Init(void);
uint32_t GetTick(void);
float GyroZ_bias = 0.0f;
void Gyro_Init_Calibrate(void);
void Send_Mouse_Data(int8_t x, int8_t y, uint8_t button);
void USB_Port_Reset(void);
int main(void)
{
	SysTick_Init();
	USB_Port_Reset();
	Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();
	MPU6050_Init();
	Kalman_Init(&kalman_pitch);
	Kalman_Init(&kalman_roll);
	lasttime = GetTick();
	Gyro_Init_Calibrate(); 
	
	while (1)
	{
		uint32_t now = GetTick();
		float dt = (now - lasttime)/1000.00f;
		if(dt<0.002f) continue;
		lasttime = now;
		MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
		float Roll_witness = atan2f(AY,AZ) * 57.29578f; 
		float Pitch_witness = atan2f(-AX, sqrtf((float)AY*(float)AY + (float)AZ*(float)AZ)) * 57.29578f;
		float Roll_G = GX / 16.4f;
		float Pitch_G = GY / 16.4f;
		Roll = Kalman_GetAngle(&kalman_roll,Roll_witness,Roll_G,dt);
		Pitch = Kalman_GetAngle(&kalman_pitch,Pitch_witness,Pitch_G,dt);
		
		float raw_rate_yaw = GZ / 16.4f;
		float raw_rate_pitch = GY / 16.4f;
		
		if(fabsf(raw_rate_yaw - GyroZ_bias) < 0.6f)
		{
			GyroZ_bias = 0.995f * GyroZ_bias + 0.005f * raw_rate_yaw;
		}
		float rate_pitch = raw_rate_pitch - kalman_pitch.bias;
		float rate_yaw = raw_rate_yaw - GyroZ_bias;
		if (fabsf(rate_pitch) < 2.0f) rate_pitch = 0.0f;
        if (fabsf(rate_yaw) < 2.0f) rate_yaw = 0.0f;
		
		float roll_rad  = Roll * 3.1415926f / 180.0f;
		float mouse_vx = cosf(roll_rad) * rate_yaw - sinf(roll_rad) * rate_pitch;
		float mouse_vy = sinf(roll_rad) * rate_yaw + cosf(roll_rad) * rate_pitch;
		
		float sensitivity = -0.3f; // 灵敏度
        int16_t out_x = (int16_t)(mouse_vx * sensitivity);
        int16_t out_y = (int16_t)(mouse_vy * sensitivity);
        if (out_x > 127) out_x = 127; 
		if (out_x < -127) out_x = -127;
        if (out_y > 127) out_y = 127;
		if (out_y < -127) out_y = -127;
		if (bDeviceState == CONFIGURED) 
        {
            // 发送数据：X位移，Y位移，按键(暂无按键传0)
            Send_Mouse_Data((int8_t)out_x, (int8_t)out_y, 0);
        }
	}
}
void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
	
}
void SysTick_Handler(void) {
    sysTick_ms++; 
}
uint32_t GetTick(void)
{
	return sysTick_ms;
}
void Gyro_Init_Calibrate(void)
{
	long sum_Z =0;
	for(int i =0;i<50;i++)
	{
		MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
		Delay_ms(5);
	}
	for(int i = 0; i < 200; i++)
	{
        MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
        sum_Z += GZ;
        Delay_ms(5);
    }
	GyroZ_bias = (float)sum_Z / 200.0f / 16.4f;
}
void Send_Mouse_Data(int8_t x, int8_t y, uint8_t button)
{
    uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
    
    // 检查端点 1 是否空闲 (上一次的数据有没有发完)
    if (GetEPTxStatus(ENDP1) == EP_TX_NAK) 
    {
        Mouse_Buffer[0] = button; // 字节0: 按键状态 (Bit0:左键, Bit1:右键)
        Mouse_Buffer[1] = x;      // 字节1: X轴偏移量
        Mouse_Buffer[2] = y;      // 字节2: Y轴偏移量
        Mouse_Buffer[3] = 0;      // 字节3: 滚轮偏移量
        
        // 将数据拷贝到 USB 的 PMA 缓冲区
        UserToPMABufferCopy(Mouse_Buffer, GetEPTxAddr(ENDP1), 4);
        
        // 设置端点 1 状态为有效，硬件会自动把数据发给电脑
        SetEPTxValid(ENDP1);
    }
}
void USB_Port_Reset(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef USBInitStructure;
	USBInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	USBInitStructure.GPIO_Pin = GPIO_Pin_12;
	USBInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&USBInitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	Delay_ms(100);
	
	USBInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&USBInitStructure);
	Delay_ms(50);
}
