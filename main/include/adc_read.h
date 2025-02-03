#ifndef ADC_READ_H
#define ADC_READ_H

#include "esp_adc/adc_oneshot.h"
#include "esp_adc_cal.h"

#define ADC1_CH5 ADC_CHANNEL_5  // GPIO33
#define ADC1_CH6 ADC_CHANNEL_6  // GPIO34
#define ADC1_CH7 ADC_CHANNEL_7  // GPIO35

#define R1 101000 
#define VCC 5     
#define A_NTC 3.70011733e-04      
#define B_NTC 2.53164380e-04      
#define C_NTC 2.88489489e-08      

extern adc_oneshot_unit_handle_t adc1_handle;

void setup_adc();

double adc_to_voltage(int adc_value);

double adc_to_temperature(int adc_value);

double adc_to_current(int adc_value);

#endif
