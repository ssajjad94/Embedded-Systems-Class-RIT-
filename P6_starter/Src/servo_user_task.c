#include "servo_user_task.h"

SERVO_USER_PARAMS_t servo_user_params;

/*
	Task initizialization.
*/
void servo_user_task_init(char *servo_computer_task_name, int servo_id)
{
	// Get pointer to THIS instance of parameters
	SERVO_USER_PARAMS_t *p = &servo_user_params;   
	
	// Initialize task parameters
	strncpy(p->task_name, servo_computer_task_name, configMAX_TASK_NAME_LEN);
	p->servo_id = servo_id;
	p->servo_position = PWM_MINIMUM;
	
	// Create the task
  xTaskCreate(servo_user_task, "USER COMMAND TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Task execution. 
*/
void servo_user_task(void *parameters)
{
	// Get pointer to THIS instance of parameters
	SERVO_USER_PARAMS_t *p = &servo_user_params;  
	
	while (1)
	{
		// Get the position from the gyroscope 
		uint8_t next_position = CalcPosition();

		// Set the position in the PWM
		if (0 == p->servo_id)
		{
			SetPWMPulseWidth1(next_position);
		}
		else
		{
			SetPWMPulseWidth2(next_position);
		}
	}
}


/*
	Uses the gyro to get the position;
 */
uint8_t CalcPosition(void)
{
	// Get the y angle
	float angle = GetAngle(1);
	
	// Convert from MIN/MAX angle to MIN/MAX PWM
	//  NewValue = ( ((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
	float position = PWM_MINIMUM + ((angle - MIN_ANGLE) * (PWM_MAXIMUM - PWM_MINIMUM) / (MAX_ANGLE - MIN_ANGLE));
	
	uint8_t pwm_position = position;
	if (pwm_position < PWM_MINIMUM)
		pwm_position = PWM_MINIMUM;
	
	if (pwm_position > PWM_MAXIMUM)
		pwm_position = PWM_MAXIMUM;
	
	return pwm_position;
}
