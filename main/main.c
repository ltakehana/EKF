#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "include/adc_read.h"
#include "include/ekf.h"
#include "driver/spi_slave.h"
#include "esp_timer.h"

#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 2

double Vt_Estimated = 0.0, Vt_Error = 0;
double KG[2][1];
double X[2][1];
double Nc=1.425;


double P[2][2] = {{0.950000073892419, 0}, {0, 9.999996034254936}};
double Q[2][2] = {{9.999944378671664e-08, 0}, {0, 9.999998929097178}}; 
double R = 7.999987139962964;

TimerHandle_t xTimer;
QueueHandle_t xQueue;

typedef struct {
    double voltage;
    double temperature;
    double current;
    double soc;
} sensor_data_t;



void adc_task(TimerHandle_t xTimer) {
	int64_t start_time = esp_timer_get_time(); 
    int adc_val_5, adc_val_6, adc_val_7;
    double voltage;
    double temperature;
    double current;
    double time;
	sensor_data_t send_data;

    adc_oneshot_read(adc1_handle, ADC1_CH5, &adc_val_5);
    adc_oneshot_read(adc1_handle, ADC1_CH6, &adc_val_6);
    adc_oneshot_read(adc1_handle, ADC1_CH7, &adc_val_7);
    
    voltage = adc_to_voltage(adc_val_5)*2;
    
    current = adc_to_current(adc_val_6);
    
    temperature = adc_to_temperature(adc_val_7);
    
    EKF_SOC_OPT(current, voltage, temperature, Nc, X, &Vt_Estimated, &Vt_Error, P, Q, R, KG);
    

    send_data.voltage = voltage;
    send_data.temperature = temperature;
    send_data.current = current;
    send_data.soc = X[0][0];
    
    int64_t end_time = esp_timer_get_time(); 
    time = (end_time - start_time) / 1000000.0; 
    
    if (xQueueSend(xQueue, &send_data, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("Failed to send data to the queue!\n");
    }

    printf("Voltage: %lf, Temperature: %lf, Current: %lf, SoC:  %lf\n Time: %lf\n", voltage, temperature, current,X[0][0],time);
}


void spi_send_task(void *pvParameters) {
    sensor_data_t received_data;
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));

    while (1) {
        if (xQueueReceive(xQueue, &received_data, portMAX_DELAY) == pdTRUE) {
            double sendbuf[4] = {0, 0, 0, 0};
            
            sendbuf[0] = received_data.temperature;
            sendbuf[1] = received_data.voltage;
            sendbuf[2] = received_data.current;
            sendbuf[3] = received_data.soc;

            t.length = sizeof(sendbuf) * 8;
            t.tx_buffer = sendbuf;

            if (spi_slave_transmit(SPI3_HOST, &t, portMAX_DELAY) != ESP_OK) {
                printf("SPI Transmission Failed!\n");
            }

            printf("Voltage: %lf, Temperature: %lf, Current: %lf, SoC: %lf\n", 
                    received_data.voltage, received_data.temperature, 
                    received_data.current, received_data.soc);
        }
    }
}


void app_main(void) {
    setup_adc();
    X[0][0]=1;
    X[1][0]=0;
    
	spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

	
	spi_slave_interface_config_t slvcfg={
	        .mode=0,
	        .spics_io_num=GPIO_CS,
	        .queue_size=4,
	        .flags=0,
	    };
	
	
	spi_slave_initialize(SPI3_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);


    xTimer = xTimerCreate("ADC Timer", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, adc_task);

    if (xTimer != NULL) {
        xTimerStart(xTimer, 0);
    }
     
    xQueue = xQueueCreate(10, sizeof(sensor_data_t));
    if (xQueue == NULL) {
        printf("Failed to create queue!\n");
        return;
    }
    xTaskCreatePinnedToCore(spi_send_task, "SPI Send Task", 2048, NULL, 1, NULL, 1);

}
