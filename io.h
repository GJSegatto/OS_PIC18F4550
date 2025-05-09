#ifndef IO_H
#define	IO_H

#include <pic18f4550.h>
#include <xc.h>

void adc_config(void);
int adc_read(void);

void pwm_config(void);
void activate_pwm(void);
void stop_pwm(void);

void config_interruption(void);

#endif	/* IO_H */

