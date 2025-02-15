#include "adc_read.h"
#include <math.h>

adc_oneshot_unit_handle_t adc1_handle;

static esp_adc_cal_characteristics_t *adc_chars;

void setup_adc() {
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,   
    };

    esp_err_t ret = adc_oneshot_new_unit(&unit_config, &adc1_handle);
    if (ret != ESP_OK) {
        printf("Erro ao inicializar o ADC: %d\n", ret);
        return;
    }

    adc_oneshot_chan_cfg_t channel_config = {
        .bitwidth = ADC_WIDTH_BIT_12,  
        .atten = ADC_ATTEN_DB_12,       
    };

    adc_oneshot_config_channel(adc1_handle, ADC1_CH3, &channel_config);
    adc_oneshot_config_channel(adc1_handle, ADC1_CH5, &channel_config);  
    adc_oneshot_config_channel(adc1_handle, ADC1_CH7, &channel_config); 

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 3330, adc_chars);
}

double adc_to_voltage(int adc_value) {
    double voltage = esp_adc_cal_raw_to_voltage(adc_value, adc_chars)/1000.00;
    return voltage;
}

double adc_to_temperature(int adc_value) {
    double voltage = adc_to_voltage(adc_value)*1.9964580874;

 	double Rt, T;

    if (voltage - R1 != 0) {
        Rt = (VCC * R1) / voltage - R1;
    } else {
        Rt = 10000; 
    }

    if ((A_NTC + B_NTC * log(Rt) + C_NTC * pow(log(Rt), 3)) != 0) {
        T = 1 / (A_NTC + B_NTC * log(Rt) + C_NTC * pow(log(Rt), 3));
    } else {
        T = 273.15; 
    }

    if (isnan(T)) {
        T = 0;
    }

    T = T - 273.15;

    return T;
}


double adc_to_current(int adc_value) {
    double voltage = adc_to_voltage(adc_value);
    return (voltage - 1.536475) / 0.5;
}
