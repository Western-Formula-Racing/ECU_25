#ifndef __PWM__
#define __PWM__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "etl/array.h"
#include "CAN_Config.hpp"
#include "IO.h"
#include "driver/ledc.h"

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define PWM_FREQ_HZ    500
#define PWM_TIMER      LEDC_TIMER_0
#define PWM_MODE       LEDC_LOW_SPEED_MODE
#define PWM_CHANNEL    LEDC_CHANNEL_0
#define PWM_RESOLUTION LEDC_TIMER_13_BIT  // 13-bit resolution


void pwm_init(void);
void pwm_set_duty(int duty_cycle);


#endif