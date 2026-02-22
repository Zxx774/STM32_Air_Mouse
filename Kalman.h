#ifndef __KALMAN_H
#define __KALMAN_H

typedef struct{
	float angle;
	float bias;
	float p[2][2];
	float Q_angle;
	float Q_gyro;
	float R_angle;
	
}kalman_t;
typedef struct{
	float roll;
	float pitch;
	float yaw;
}result;
void Kalman_Init(kalman_t *Kalman);
float Kalman_GetAngle(kalman_t *Kalman,float newAngle,float newRate,float dt);
#endif
