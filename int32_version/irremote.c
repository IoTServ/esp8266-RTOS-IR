//#include <esp8266.h>
#include "esp_common.h"
#include "irremote.h"
#include "gpio.h"
uint8_t ir_gpio_pin;
uint16_t frequency;

void ir_set_pin(uint8_t pin) {
  ir_gpio_pin = pin;
}

void ir_set_frequency(uint16_t khz) {
  frequency = 1000 / khz / 3;
}

void ir_mark(uint32_t time) {
  if (time == 0) return;

  uint32_t start = system_get_time();
  while(system_get_time() - start < time) {
    GPIO_OUTPUT_SET(ir_gpio_pin, 1);
    os_delay_us(frequency);
    GPIO_OUTPUT_SET(ir_gpio_pin, 0);
    os_delay_us(frequency * 2);
  }
}

void ir_space(uint32_t time) {
  GPIO_OUTPUT_SET(ir_gpio_pin, 0);
  if (time == 0) return;
  
  if (time <= 65535) {
    os_delay_us(time - 100);
  }else{
    os_delay_us(65535);
    os_delay_us(time - 65535);
  }
}

void ir_send_raw(uint32_t buffer[], uint16_t bufferSize, uint16_t hz) {
  ir_set_frequency(hz);

  uint16_t i;

  for (i = 0; i < bufferSize; i++) {
    if (i & 1) {
      ir_space(buffer[i]);
    } else {
      ir_mark(buffer[i]);
    }
  }

  ir_space(0);
}
