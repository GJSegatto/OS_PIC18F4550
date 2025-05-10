#ifndef IO_H
#define	IO_H

#include <pic18f4550.h>
#include <stdint.h>
#include <xc.h>

void adc_config(void);
uint16_t adc_read(void);

void pwm_config(void);
void activate_pwm(uint16_t dc);
void stop_pwm(void);

void config_interruption(void);

#endif	/* IO_H */

