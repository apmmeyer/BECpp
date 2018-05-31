#ifndef BUZZER_H
#define BUZZER_H

#include <mraa/gpio.h>
#include <mraa/pwm.h>
#include "except.h"

/** CLASSES **/

class buzzer_device {

  private:

	mraa_gpio_context m_gpio_buzzer_Control;
	int i_pin;
	int i_period_ms;

  public:

    /// CONSTRUCTORS

	buzzer_device(){}; // null
	buzzer_device(int pin);
	buzzer_device(int pin, int period_ms);

    /// DESTRUCTORS

	~buzzer_device(){}; // null

    /// ASSESSORS

        // return pin value
    int get_pin();

        // return period value
    int get_period();

        // set pin value
    void set_pin(int pin);

        // return period value
    void set_period(int period_ms);

    /// METHODS

        // set GPIO direction and mode to OUTPUT/STRONG HIGH AND LOW
    mraa_result_t init_buzzer(mraa_gpio_context gpio_buzzer_Control);

        // initialize PWM and set PWM period (ms)
    mraa_result_t init_pwm(int pin, int period_ms);

};




#endif
