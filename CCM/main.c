#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "include/adc_read.h"
#include "driver/spi_slave.h"
#include "esp_timer.h"
#include "include/discrete_integrator.h"

#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 2

#define FILTER_SIZE 20


discrete_integrator_t integrator;

double currentFilter[FILTER_SIZE];
double voltageFilter[FILTER_SIZE];
double temperatureFilter[FILTER_SIZE];
double voltage;
double temperature;
double soc;
double current;
int filterIndex=0;
int filterSize=1;

double initialSoc=1;
double capacity=1.388960;



TimerHandle_t xTimer1;
TimerHandle_t xTimer2;
TimerHandle_t xTimer3;
QueueHandle_t xQueue;

typedef struct {
    double voltage;
    double temperature;
    double current;
    double soc;
} sensor_data_t;



void init_integrator(discrete_integrator_t *integrator) {
    integrator_reset(integrator);
}

void estimation_task(TimerHandle_t xTimer) {
	int64_t start_time = esp_timer_get_time();
    double time;

    integrate_trapezoidal(&integrator, current, start_time);
    soc = initialSoc - integrator.integral / (capacity * 3600);
    int64_t end_time = esp_timer_get_time();
    time = (end_time - start_time) / 1000000.0;

    printf("Voltage: %lf, Temperature: %lf, Current: %lf, SoC:  %lf\n Time: %lf\n", voltage, temperature, current,soc,time);
}

void adc_task(TimerHandle_t xTimer) {
    int adc_val_3,adc_val_5, adc_val_7;

    adc_oneshot_read(adc1_handle, ADC1_CH3, &adc_val_3);
    adc_oneshot_read(adc1_handle, ADC1_CH5, &adc_val_5);
    adc_oneshot_read(adc1_handle, ADC1_CH7, &adc_val_7);

    currentFilter[filterIndex]=0;
    temperatureFilter[filterIndex]=adc_to_temperature(adc_val_5);
    voltageFilter[filterIndex]=adc_to_voltage(adc_val_7)*2.0119873817;



    double currentSum=0;
    double voltageSum=0;
    double temperatureSum=0;

    for(int i=0;i<=filterSize;i++){
    	currentSum+=currentFilter[i];
    	voltageSum+=voltageFilter[i];
    	temperatureSum+=temperatureFilter[i];
    }
    current=adc_to_current(adc_val_3);
    voltage=voltageSum/filterSize;
    temperature=temperatureSum/filterSize;

    if(filterSize<FILTER_SIZE)
    	filterSize++;
    filterIndex++;
    if(filterIndex>=FILTER_SIZE)
    	filterIndex=0;
}


void spi_send_task(void *pvParameters) {
    sensor_data_t received_data;
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));

    while (1) {
        if (xQueueReceive(xQueue, &received_data, portMAX_DELAY) == pdTRUE) {
            double sendbuf[4] = {0, 0, 0, 0};

            sendbuf[0] = received_data.voltage;
            sendbuf[1] = received_data.temperature;
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

void spi_enqueue(TimerHandle_t xTimer){
	sensor_data_t send_data;

    send_data.voltage = voltage;
	send_data.temperature = temperature;
	send_data.current = current;
	send_data.soc = soc;

    if (xQueueSend(xQueue, &send_data, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("Failed to send data to the queue!\n");
    }
}



void app_main(void) {
    setup_adc();

    init_integrator(&integrator);

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


    xTimer1 = xTimerCreate("ADC Timer", pdMS_TO_TICKS(10), pdTRUE, (void *)0, adc_task);
    xTimer2 = xTimerCreate("Estimation Timer", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, estimation_task);
    xTimer3 = xTimerCreate("SPI Timer", pdMS_TO_TICKS(100), pdTRUE, (void *)0, spi_enqueue);

    if (xTimer1 != NULL) {
        xTimerStart(xTimer1, 0);
    }

    if (xTimer2 != NULL) {
        xTimerStart(xTimer2, 0);
    }

    if (xTimer3 != NULL) {
        xTimerStart(xTimer3, 0);
    }

    xQueue = xQueueCreate(10, sizeof(sensor_data_t));
    if (xQueue == NULL) {
        printf("Failed to create queue!\n");
        return;
    }

    xTaskCreatePinnedToCore(spi_send_task, "SPI Send Task", 2048, NULL, 1, NULL, 1);

}
