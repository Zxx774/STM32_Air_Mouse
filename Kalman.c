#include "stm32f10x.h"                  // Device header
#include "Kalman.h"
#include "MPU6050.h"
#include <math.h>

void Kalman_Init(kalman_t *Kalman)
{
	Kalman->angle = 0.0f;
	Kalman->bias = 0.0f;
	Kalman->p[0][0] = 1.0f;
	Kalman->p[0][1] = 0.0f;
	Kalman->p[1][0] = 0.0f;
	Kalman->p[1][1] = 1.0f;
	Kalman->Q_angle = 0.001f;
	Kalman->Q_gyro = 0.003f;
	Kalman->R_angle = 0.03f;
}
float Kalman_GetAngle(kalman_t *Kalman,float newAngle,float newRate,float dt)
{
	float Rate = newRate - Kalman->bias;
	
	//X(k|k -1) = AX(k -1|k - 1) + BU(k)
	Kalman->angle += Rate*dt;
	
	//P(k|k -1) = AP(k -1|k -1)AT + Q
	//Q:过程噪声
	Kalman->p[0][0] += (Kalman->p[1][1]*dt - Kalman->p[1][0] - Kalman->p[0][1] + Kalman->Q_angle)*dt;
	Kalman->p[0][1] -= Kalman->p[1][1]*dt;
	Kalman->p[1][0] -= Kalman->p[1][1]*dt;
	Kalman->p[1][1] += Kalman->Q_gyro*dt;
	
	
	//K(k) = P(k|k - 1)HT/HP(k|k-1)HT +R
	//R:测量噪声
	float S = Kalman->p[0][0] + Kalman->R_angle;
	float k0 = Kalman->p[0][0]/S;
	float k1 = Kalman->p[1][0]/S;
	
	//X(k|k)=X(k|k-1)+K(k)[Z(k) - HX(k|k -1)]
	float delta = newAngle - Kalman->angle;
	Kalman->angle += k0*delta;
	Kalman->bias +=k1*delta;
	
	//P(k|k) = [I - K(k)H]P(k|k - 1)
	float temp_p00 = Kalman->p[0][0];
	float temp_p01 = Kalman->p[0][1];
	Kalman->p[0][0] -= temp_p00*k0;
    Kalman->p[0][1] -= temp_p01*k0;
    Kalman->p[1][0] -= temp_p00*k1;
    Kalman->p[1][1] -= temp_p01*k1;
	
	return Kalman->angle;
}

