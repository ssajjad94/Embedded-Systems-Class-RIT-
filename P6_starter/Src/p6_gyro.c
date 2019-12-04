/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.h"

#define GYRO_THRESHOLD_DETECTION (10000)   	/* Tunnng parameters for gyroscope */

/* Gyroscope variables */
float gyro_velocity[3] = {0};       // angular velocity
int32_t gyro_angle[3] = {0, 0, 0};	// angle

void gyro_task(void* argument);  

void gyro_task_init() {
  
  if(BSP_GYRO_Init() != HAL_OK)  {
    /* Initialization Error */
    Error_Handler();
  }
  if (pdPASS != xTaskCreate (gyro_task,	"print", 256, NULL, 2, NULL)) {
    Error_Handler();
  }
}

void gyro_task(void* argument) {
	
  while(1) {

    // read angular velocity in 3D
		BSP_GYRO_GetXYZ(gyro_velocity);   // get raw values from gyro device
    
    // integrate angular velocity to get angle
    for(int ii=0; ii<3; ii++) {
      gyro_angle[ii] += (int32_t)(gyro_velocity[ii] / GYRO_THRESHOLD_DETECTION);
    }
		
    // USART_Printf("Velocity: \t(%d,\t%d,\t%d) \n\r", (int)gyro_velocity[0], (int)gyro_velocity[1], (int)gyro_velocity[2]);
		// USART_Printf("Angle: 	  \t(%d,\t%d,\t%d) \n\r", gyro_angle[0], gyro_angle[1], gyro_angle[2]);

    osDelay(50);
  }
}


// Given an angle (0 for x, 1 for y, 2 for z)
int GetAngle(int direction)
{
	if (direction >= 0 && direction < 3)
		return gyro_angle[direction];
	
	return 0;
}
