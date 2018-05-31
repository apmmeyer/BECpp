#include "buzzer.h"


/** CONSTRUCTORS-ASSESSORS **/

// CONSTRUCTORS

buzzer_device::buzzer_device(int pin){
    i_pin = pin;
    m_gpio_buzzer_Control = mraa_gpio_init (i_pin);
}

buzzer_device::buzzer_device(int pin, int frequency){
    i_pin = pin;
    i_frequency = frequency;
    m_gpio_buzzer_Control = mraa_gpio_init (i_pin);
}

// ASSESSORS

    // return pin value
int buzzer_device::get_pin(){
    return i_pin;
}
    // return period value
int buzzer_device::get_period(){
    return i_period_ms;
}
    // set pin value
void buzzer_device::set_pin(int pin){
    i_pin=pin;
}
    // return period value
void buzzer_device::set_period(int period_ms){
    i_period_ms=period_ms;
}

/** METHODS **/

    // set GPIO direction and mode to OUTPUT/STRONG HIGH AND LOW
mraa_result_t init_buzzer(mraa_gpio_context gpio_buzzer_Control){

    mraa_result_t error;

    // set direction to OUTPUT
    error += mraa_gpio_dir(gpio_buzzer_Control,MRAA_GPIO_OUT);

    // set mode to STRONG HIGH AND LOW
    error += mraa_gpio_mode(gpio_buzzer_Control,MRAA_GPIO_STRONG);

    return error;
}

    // initialize PWM and set PWM period (ms)
mraa_result_t init_pwm(int pin, int period_ms){

        // variables
    mraa_pwm_context m_pwm_Control;
    mraa_result_t error;

        // set pin
    m_pwm_Control = mraa_pwm_init(pin);

        // set period (ms)
    error = mraa_pwm_period_ms(m_pwm_Control, period_ms);

    return error;

}
