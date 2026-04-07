#include "pwm.h"

void pwm_init(void)
{
    ledc_timer_config_t timer_cfg = {};
    timer_cfg.speed_mode      = PWM_MODE;
    timer_cfg.timer_num       = PWM_TIMER;
    timer_cfg.duty_resolution = PWM_RESOLUTION;
    timer_cfg.freq_hz         = PWM_FREQ_HZ;
    timer_cfg.clk_cfg         = LEDC_AUTO_CLK;
    timer_cfg.deconfigure     = false;

    ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));

    uint32_t duty = 100* (1UL << PWM_RESOLUTION) / 100;

    ledc_channel_config_t channel_cfg = {};
    channel_cfg.gpio_num   = ECU_11_IO2;
    channel_cfg.speed_mode = PWM_MODE;
    channel_cfg.channel    = PWM_CHANNEL;
    channel_cfg.intr_type  = LEDC_INTR_DISABLE;
    channel_cfg.timer_sel  = PWM_TIMER;
    channel_cfg.duty       = duty;
    channel_cfg.hpoint     = 0;
    channel_cfg.sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
    channel_cfg.flags.output_invert = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&channel_cfg));
}

void pwm_set_duty(int duty_cycle){

    duty_cycle = max(0, duty_cycle);
    duty_cycle = min(100,duty_cycle);
    uint32_t duty = duty_cycle* (1UL << PWM_RESOLUTION) / 100;

    ledc_channel_config_t channel_cfg = {};
    channel_cfg.gpio_num   = ECU_10_IO1;
    channel_cfg.speed_mode = PWM_MODE;
    channel_cfg.channel    = PWM_CHANNEL;
    channel_cfg.intr_type  = LEDC_INTR_DISABLE;
    channel_cfg.timer_sel  = PWM_TIMER;
    channel_cfg.duty       = duty;
    channel_cfg.hpoint     = 0;
    channel_cfg.sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
    channel_cfg.flags.output_invert = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&channel_cfg));

}