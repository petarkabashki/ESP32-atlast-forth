#include "atldef.h"
#include <string.h>
#include "u8g2_atl.h"
#include <u8g2.h>
// #include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "common.h"

#define GPIO_PIN_SEL(PIN)  (1ULL<<PIN)

extern u8g2_t u8g2;

prim P_system_MS()
{ // xpos ypos test --
	Sl(1);

	vTaskDelay(S0 / portTICK_RATE_MS);

	Pop;
}

prim P_system_gpio_ouput()
{ // pin iomode --
	Sl(2);

	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = S0; //GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_PIN_SEL(S0);
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	Pop2;
}

prim P_system_gpio_set()
{ // pin iomode --
	Sl(2);

	gpio_set_level(S1, S0);

	Pop2;
}

prim P_system_gpio_get()
{ // pin iomode --
	Sl(1);

	int level = gpio_get_level(S0);
	Pop;
	Push = level;
}

struct primfcn system_fcns[] = {
		{"0MS", P_system_MS},
		{"0GPIO.CONFIG", P_system_gpio_ouput},
		{"0GPIO.SET", P_system_gpio_set},
		{"0GPIO.GET", P_system_gpio_get},
		{NULL, (codeptr)0}};