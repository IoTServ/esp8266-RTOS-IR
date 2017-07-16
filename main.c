/* The classic "blink" example
 *
 * This sample code is in the public domain.
 */
#include <stdlib.h>
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp8266.h"

#include "gpio.h"

#include "irremote.h"

#define GPIO_IR_PIN 4
const int gpio = 4;
uint16_t srcdata[68] =
{4500,4550,550,600,550,1750,550,1750,550,1750,550,600,550,600,550,600,550,600,550,600,550,1750,550,1750,550,1750,550,600,550,600,550,600,550,
600,550,1750,550,1750,550,1750,550,1750,550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,600,550,1750,550,1750,550,1750,550,1750,550};

uint16_t rawdata[68] = {9000,4550,600,600,600 ,1750,600,1750,600,1750,550,600,600,600,550,600,600, 600,550,650,600,1750,550,1750,600,1750,600,600,550,650,600,600,550 ,600 ,600 ,600 ,600 ,600 ,600 ,    600 ,550 ,1750 ,600 ,1750 ,600 ,1750 ,550 ,1750 ,550 ,600 ,550 ,1750 ,600 ,1750 ,600 ,1750 ,600 ,
600 ,600 ,600 ,600 ,600 ,600 ,600 ,600 ,1750 ,600};

uint16_t ktdata[74] = {9000,4550,650,1700,650,600,650,600,650,1700,650,600,650,600,650,600,650,600,650,1700,650,1700,650,600,650,1700,650,600,650,600,650,600,650,600,
650,600,650,600,650,600,650,600,650,600,650,1700,650,600,650,600,650,600,650,600,650,600,650,600,650,1700,650,600,650,1700,650,600,650,600,650,1700,
650,600,650,};

/* This task uses the high level GPIO API (esp_gpio.h) to blink an LED.
 *
 */
void blinkenTask(void *pvParameters)
{
	// Enable GPIO pin and turn it off
	//gpio_enable(GPIO_IR_PIN, GPIO_OUTPUT);
	GPIO_OUTPUT_SET(GPIO_IR_PIN, 0);

	ir_set_pin(GPIO_IR_PIN);
    while(1) {
	printf("init ok2!");
  vTaskDelay(1000 / portTICK_RATE_MS);

	ir_send_raw(srcdata, 68, 38);
	//ir_send_raw(ktdata, 74, 38);
	vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void user_init(void)
{
    printf("init ok!");
    xTaskCreate(blinkenTask, "blinkenTask", 256, NULL, 2, NULL);
    //xTaskCreate(blinkenRegisterTask, "blinkenRegisterTask", 256, NULL, 2, NULL);
}
