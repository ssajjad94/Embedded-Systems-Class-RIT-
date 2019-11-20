#include "servo_computer_task.h"

SERVO_COMPUTER_PARAMS_t servo_computer_params;

/*
	Task initizialization.
*/
void servo_computer_task_init(char *servo_computer_task_name, int servo_id)
{
	// Get pointer to THIS instance of parameters
	SERVO_COMPUTER_PARAMS_t *p = &servo_computer_params;   
	
	// Initialize task parameters
	strncpy(p->task_name, servo_computer_task_name, configMAX_TASK_NAME_LEN);
	p->servo_id = servo_id;
	p->servo_position = PWM_MINIMUM;
	
	// Create the task
  xTaskCreate(servo_computer_task, "USER COMMAND TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Task execution. 
*/
void servo_computer_task(void *parameters)
{
	SERVO_COMPUTER_PARAMS_t *p = &servo_computer_params;   
	
	while (1)
	{
		// Check the current position from the params
		uint8_t curr_positon = p->servo_position;
		
		// Set the position
		if (0 == p->servo_id)
		{
			SetPWMPulseWidth1(curr_positon);
		}
		else
		{
			SetPWMPulseWidth2(curr_positon);
		}
	}
}


/*
	Change the servo position to a random position away.
*/
void GoToRandomPosition()
{
	SERVO_COMPUTER_PARAMS_t *p = &servo_computer_params;   
	
	// Get the current position
	int curr_position = p->servo_position;
	
	// Calculate a random position away from the current position
	//		between PWM_MINIMUM and PWM_MAXIMUM
	int new_position;
	
	
	// Calculate a random displacement (change in position) from MIN to MAX
	uint32_t random_displacement;
	int min_displacement = MIN_CHANGE;
	int max_displacement = MAX_CHANGE;
	HAL_RNG_GenerateRandomNumber(&hrng, &random_displacement);
	random_displacement = min_displacement + (random_displacement % (max_displacement - min_displacement));
	
	// Calculate a random direction
	uint32_t pos_or_neg;
		// 0 for left, 1 for right
	
	// Check if we're already at the min or max
	// If so, we have to go the other way. Otherwise, do a random direction.
	if (curr_position == PWM_MAXIMUM )
	{
			pos_or_neg = 0;
	}
	else if (curr_position == PWM_MINIMUM)
	{
			pos_or_neg = 1;	
	}
	else
	{
		HAL_RNG_GenerateRandomNumber(&hrng, &pos_or_neg);
		pos_or_neg = pos_or_neg % 2;
	}
	

	if (pos_or_neg)
	{
		// Move right
		new_position = curr_position + random_displacement;
		
		if (new_position > PWM_MAXIMUM)
			new_position = PWM_MAXIMUM;
	}
	else
	{
		// Move left
		new_position = curr_position - random_displacement;
		
		if (new_position < PWM_MINIMUM)
			new_position = PWM_MINIMUM;
	}
	
	// Set that positon as new position
	p->servo_position = new_position;
}
